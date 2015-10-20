'''
 @file      monte_carlo.py
 @brief     Utility functions for monte carlo neutron simulations
 @details   This file contains functions to be used in a Monte Carlo
            simulation. 
            Later, these funcitons will be re-organized and implemented as 
            objects.
 @author    Geoffrey Gunow, Luke Eure
 @date      September 30, 2015
'''

# Import libraries
from math import *
from distributions import *
from plotter import *
import copy
import coords
import tally
import fission

'''
 @brief     Function that generates a neutron and measures how 
            far it travels before being absorbed.
 @details   A neutron is created in the bounding box using
            sample_location() for the first batch and sample_fission_site()
            for the rest of the batches. It moves a distance determined by
            sample_distance(). It is then either absorbed or
            scattered as determined by sample_interaction(). When
            it is absorbed, its distance from its starting point
            is appended to crow_distances. If the absorption creates
            a fission event, the number of neutrons emited is sampled.
            The location of the fission event is added to a list of fission
            events.
 @param	    mat a Material object containing information about the material
 @param     bounds a Boundaries object containing the limits
            of the bounding box
 @param     tallies a dictionary containing tallies of crow distances,
            leakages, absorptions, and fissions
 @param     fission_banks a dictionary containing the old and new fission banks
 @param     first_round a boolean telling whenther or not this is the first
            batch to be tested
'''
def transport_neutron(mat, bounds, tallies, fission_banks, first_round):
    # first round
    if first_round:
        neutron_starting_point = sample_location(bounds)
        
    # every other round
    else:
        neutron_starting_point = sample_fission_site(fission_banks['old'])
        
    neutron_lost = False
    neutron_reflected = False
    neutron_position = coords.Coords(neutron_starting_point.x,
            neutron_starting_point.y, neutron_starting_point.z) 
    neutron_interaction = 0
    while neutron_interaction != 1:
        neutron_distance = sample_distance(mat)
        theta = sample_polar_angle()
        phi = sample_azimuthal_angle()
    
        # neutron movement unit vector
        neutron_movement_unit_vector = coords.Coords(sin(theta)*cos(phi),
                   sin(theta)*sin(phi), cos(theta))
        
        while neutron_distance > 0: 
            
            boundary_max_distances = list()
            axises = ['x', 'x', 'y', 'y', 'z', 'z']
            directions = ['max', 'min', 'max', 'min', 'max', 'min']
            
            boundary_max_distances.append(bounds.get_x_max() - neutron_position.x)
            boundary_max_distances.append(bounds.get_x_min() - neutron_position.x)
            boundary_max_distances.append(bounds.get_y_max() - neutron_position.y)
            boundary_max_distances.append(bounds.get_y_min() - neutron_position.y)
            boundary_max_distances.append(bounds.get_z_max() - neutron_position.z)
            boundary_max_distances.append(bounds.get_z_min() - neutron_position.z)
           

            # lim_bound contains the string of the limiting boundary
            lim_bound = list()
                
            # tempd contains the current smallest r
            tempd = neutron_distance
                
            # r is a variable that contains the distance along the
            # direction vector to the boundary being tested  
            
            # test each boundary
            for q in xrange(6):
                if (q == 0 or q == 1):
                    r = boundary_max_distances[q]/(sin(theta)*cos(phi))
                if (q == 2 or q == 3):
                    r = boundary_max_distances[q]/(sin(theta)*sin(phi))
                if (q == 4 or q == 5):
                    r = boundary_max_distances[q]/(cos(theta))
                if (0 <= r and r < tempd and not r == -0.0):
                    tempd = r
                    lim_bound = []
                    lim_bound.append(axises[q] + '_' + directions[q])
                elif r == tempd:
                    lim_bound.append(axises[q] + '_' + directions[q])

            # if the neutron stays within the bounding box
            if lim_bound == []:
                neutron_distance = 0
            neutron_position.sadd(neutron_movement_unit_vector * tempd)

            for surface in lim_bound:  
                # if the neutron is reflected
                if (bounds.get_surface_type(surface) == 1):
                    if surface == 'x_max' or surface == 'x_min':
                        neutron_movement_unit_vector.smul([-1, 1, 1])
                    if surface == 'y_max' or surface == 'y_min':
                        neutron_movement_unit_vector.smul([1, -1, 1])
                    if surface == 'z_max' or surface == 'z_min':
                        neutron_movement_unit_vector.smul([1, 1, -1])
                    neutron_reflected = True
    
                # if the neutron escapes
                elif (bounds.get_surface_type(surface) == 0):
                    neutron_lost = True
                    neutron_interaction = 1
                    neutron_distance = 0
                    tallies['leaks'].add(1)
            if neutron_reflected:
                neutron_distance -= tempd
                
        if not neutron_lost:
            neutron_interaction = sample_interaction(mat)

    # neutron_position turns into a vector pointing from the neutron's origin
    # to its absorption point
    if not neutron_lost:
        tallies['absorptions'].add(1)
        if sample_fission(mat) == 1:
            for j in xrange(sample_num_fission(mat)):
                fission_banks['new'].add([neutron_position.x,
                    neutron_position.y, neutron_position.z])
                tallies['fissions'].add(1) 
        neutron_position.ssub([neutron_starting_point.x,
            neutron_starting_point.y, neutron_starting_point.z])
        tallies['crows'].add(neutron_position.getDistance())
        tallies['num_crows'].add(1)

'''
 @brief     Generates and transports neutron histories, calculates the mean
            crow distance
 @param     n_histories number of neutron histories to run
 @param     mat a Material object containing information
            about the material
 @param     bounds a Boundaries object containing the limits of the
            bounding box
'''
def generate_neutron_histories(n_histories, mat, bounds):
    
    crow_distances = tally.Tally()
    num_crow_distances = tally.Tally()
    leakage_tally = tally.Tally()
    absorption_tally = tally.Tally()
    fission_tally = tally.Tally()
    
    tallies = {'crows': crow_distances, 'num_crows': num_crow_distances,
            'leaks': leakage_tally, 'absorptions': absorption_tally,
            'fissions': fission_tally}

    old_fission_locations = fission.Fission()
    new_fission_locations = fission.Fission()
    fission_banks = {'old': old_fission_locations,
            'new': new_fission_locations}
    
    first_round = True
    for batch in xrange(10):
        
        # assign the new fission locations to the old fission list
        old_fission_locations.clear()
        for num in xrange(new_fission_locations.length):
            old_fission_locations.add(new_fission_locations.next())
        new_fission_locations.clear()
        
        # clear the tallies for leaks absorptions and fissions
        tallies['leaks'].clear()
        tallies['absorptions'].clear()
        tallies['fissions'].clear()

        # simulate the neutron behavior
        for i in xrange(n_histories):
            transport_neutron(mat, bounds, tallies, fission_banks, first_round)

        print 'For batch ', batch + 1, ' k = ', \
                tallies['fissions'].count/(tallies['leaks'].count + \
                tallies['absorptions'].count)
        first_round = False

    mean_crow_distance = crow_distances.count / num_crow_distances.count
    print 'Mean crow fly distance = ', mean_crow_distance
