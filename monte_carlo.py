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
import mesh
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
 @param     mesh1 a Mesh object containing information about the mesh
'''
def transport_neutron(mat, bounds, tallies, fission_banks, first_round, mesh1):
    # first round
    if first_round:
        neutron_starting_point = sample_location(bounds)
        
    # every other round
    else:
        neutron_starting_point = sample_fission_site(fission_banks['old'])
    
    # make a neutron
    neutron_lost = False
    neutron_reflected = False
    neutron_position = coords.Coords(neutron_starting_point.x,
            neutron_starting_point.y, neutron_starting_point.z) 
    '''
    current_cell_position = mesh1.get_cell(neutron_starting_point.x,
            neutron_starting_point.y, neutron_starting_point.z)
    '''

    neutron_interaction = 0
    while neutron_interaction != 1:
        neutron_distance = sample_distance(mat)
        theta = sample_polar_angle()
        phi = sample_azimuthal_angle()
    
        neutron_movement_unit_vector = coords.Coords(sin(theta)*cos(phi),
                   sin(theta)*sin(phi), cos(theta))
        
        while neutron_distance > 0: 
            boundary_max_distances = list()
            cell_max_distances = list()
            bound_names = ['x_max', 'x_min', 'y_max', 'y_min', 'z_max', 'z_min']
            axises = ['x', 'x', 'y', 'y', 'z', 'z']
            directions = ['max', 'min', 'max', 'min', 'max', 'min']
            cell_bound_is_box_bound = [False] * 6
            
            '''
            cell_max_distances.append(mesh1.get_cell_bounds('x', 'max', \
                    current_cell_position[0]) - neutron_position.x)
            cell_max_distances.append(mesh1.get_cell_bounds('x', 'min', \
                    current_cell_position[0]) - neutron_position.x)
            cell_max_distances.append(mesh1.get_cell_bounds('y', 'max', \
                    current_cell_position[1]) - neutron_position.y)
            cell_max_distances.append(mesh1.get_cell_bounds('y', 'min', \
                    current_cell_position[1]) - neutron_position.y)
            cell_max_distances.append(mesh1.get_cell_bounds('z', 'max', \
                    current_cell_position[2]) - neutron_position.z)
            cell_max_distances.append(mesh1.get_cell_bounds('z', 'min', \
                    current_cell_position[2]) - neutron_position.z)
            '''

            boundary_max_distances.append(bounds.get_x_max() - \
                    neutron_position.x)
            boundary_max_distances.append(bounds.get_x_min() - \
                    neutron_position.x)
            boundary_max_distances.append(bounds.get_y_max() - \
                    neutron_position.y)
            boundary_max_distances.append(bounds.get_y_min() - \
                    neutron_position.y)
            boundary_max_distances.append(bounds.get_z_max() - \
                    neutron_position.z)
            boundary_max_distances.append(bounds.get_z_min() - \
                    neutron_position.z)
            '''
            for i in xrange(6):
                if cell_max_distances[i] == boundary_max_distances[i]:
                    cell_bound_is_box_bound[i] = False
            '''

            # these contains the strings of the limiting boundary of the cell
            # and bounding box respectively
            cell_lim_bound = list()
            box_lim_bound = list()

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
                    box_lim_bound = []
                    box_lim_bound.append(bound_names[q])
                elif r == tempd:
                    box_lim_bound.append(bound_names[q])

            # if the neutron stays within the cell
            if box_lim_bound == []:
                neutron_distancie = 0
            neutron_position.sadd(neutron_movement_unit_vector * tempd)
            


            # TODO add behavior if the neutron hits a cell boundary
            '''
            for surface in cell_lim_bound:
                if surface == 'x_max':
                    current_cell_position[0] += 1
                if surface == 'x_min':
                    current_cell_position[0] -= 1
                if surface == 'y_max':
                    current_cell_position[1] += 1
                if surface == 'y_min':
                    current_cell_position[1] -= 1
                if surface == 'z_max':
                    current_cell_position[2] += 1
                if surface == 'z_min':
                    current_cell_position[2] -= 1
            '''   

            # if the limiting cell boundary is a box boundary, add that limit
            # to 
            '''
            for i in xrange(6):
                if cell_bound_is_box_bound[i]:
                    if bounds[i] in cell_lim_bound:
                        box_lim_bound.append(bound_names[i])
            print current_cell_position
            print cell_lim_bound
            print box_lim_bound
            '''

            for surface in box_lim_bound:  
                # if the neutron is reflected
                if (bounds.get_surface_type(surface) == 1):
                    if surface == 'x_max' or surface == 'x_min':
                        neutron_movement_unit_vector *= [-1, 1, 1]
                    if surface == 'y_max' or surface == 'y_min':
                        neutron_movement_unit_vector *= [1, -1, 1]
                    if surface == 'z_max' or surface == 'z_min':
                        neutron_movement_unit_vector *= [1, 1, -1]
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
 @param     mesh a Mesh object containing information about the mesh
 @param     num_batches the number of batches to be tested
'''
def generate_neutron_histories(n_histories, mat, bounds, mesh1, num_batches):
    
    crow_distances = tally.Tally()
    num_crow_distances = tally.Tally()
    leakage_tally = tally.Tally()
    absorption_tally = tally.Tally()
    fission_tally = tally.Tally()
    mesh_tallies = {'x': [], 'y': [], 'z': []}
    for direction in ['x', 'y', 'z']:
        for i in xrange(mesh1.get_mesh_length(direction)):
            mesh_tallies[direction].append(tally.Tally())
    
    tallies = {'crows': crow_distances, 'num_crows': num_crow_distances,
            'leaks': leakage_tally, 'absorptions': absorption_tally,
            'fissions': fission_tally, 'mesh': mesh_tallies}

    old_fission_locations = fission.Fission()
    new_fission_locations = fission.Fission()
    fission_banks = {'old': old_fission_locations,
            'new': new_fission_locations}
    
    first_round = True
    for batch in xrange(num_batches):
        
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
        print new_fission_locations.length
        print old_fission_locations.length
        print mesh1
        for i in xrange(n_histories):
            transport_neutron(mat, bounds, tallies, fission_banks, first_round, mesh1)

        print 'For batch ', batch + 1, ' k = ', \
                tallies['fissions'].count/(tallies['leaks'].count + \
                tallies['absorptions'].count)
        print tallies['fissions'].count
        print tallies['absorptions'].count
        print tallies['fissions'].count
        first_round = False

    mean_crow_distance = crow_distances.count / num_crow_distances.count
    print 'Mean crow fly distance = ', mean_crow_distance

