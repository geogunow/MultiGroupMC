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
from neutron import *
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
 @param     mesh a Mesh object containing information about the mesh
 @param     batch the batch number of the neutron
'''
def transport_neutron(bounds, tallies, fission_banks, first_round,
        mesh, batch):

    TINY_MOVE = 1e-10
    
    # initial guess of neutron source distribution
    if first_round:
        neutron_starting_point = sample_location(bounds)
        
    # subsequent approximations using previous fission sites
    else:
        neutron_starting_point = sample_fission_site(fission_banks['old'])
    
    # initialize neutron
    neutron_lost = False
    theta = sample_polar_angle()
    phi = sample_azimuthal_angle()
    neutron = Neutron(neutron_starting_point, theta, phi)

    # get mesh cell
    cell = mesh.get_cell(neutron.get_position_vector(),
            neutron.get_direction_vector())
    neutron.set_cell(cell)
    
    # set neutron group
    cell_mat = mesh.get_material(neutron.get_cell())
    group = sample_neutron_energy_group(cell_mat.chi)
    neutron.set_group(group)
    axes = ['x','y', 'z']
    
    # follow neutron while alive
    while neutron.alive:
        
        # sample neutron distance to collision
        neutron_distance = sample_distance( \
                mesh.get_material(neutron.get_cell()), neutron.group)
    
        # track neutron until collision or leakage
        while neutron_distance > 0:

            cell = mesh.get_cell(neutron.get_position_vector(),
                    neutron.get_direction_vector())
            neutron.set_cell(cell)
           
            # correction in the case that the neutron was nudged out of bounds
            for axis in axes:
                min_bound = bounds.get_surface_coord(axis, 'min')
                max_bound = bounds.get_surface_coord(axis, 'max')
                neutron_coord = neutron.get_position(axis)
                if neutron_coord < min_bound:
                    neutron.set_position(axis, min_bound)
                if neutron_coord > max_bound:
                    neutron.set_position(axis, max_bound)

            # get cell boundaries
            cell_boundaries = { \
                    'min': mesh.get_cell_min(neutron.get_position_vector(),
                        direction=neutron.get_direction_vector()),
                    'max': mesh.get_cell_max(neutron.get_position_vector(),
                        direction=neutron.get_direction_vector())}
            
            # calculate distances to cell boundaries
            distance_to_cell_edge = dict()
            for axis in axes:
                distance_to_cell_edge[axis] = dict()
                for side in ['min', 'max']:
                    distance_to_cell_edge[axis][side] = \
                            cell_boundaries[side][axis] \
                            - neutron.get_position(axis)
            # these contains the strings of the limiting boundary of the cell
            # and bounding box respectively
            cell_lim_bound = list()
            box_lim_bound = list()

            # tempd contains the current smallest r
            tempd = neutron_distance
                
            # r is a variable that contains the distance along the
            # direction vector to the boundary being tested  
            # test each boundary
            for axis in axes:
                for side in ['min', 'max']:
                    r = distance_to_cell_edge[axis][side] \
                            / neutron.get_direction(axis)
                    
                    if (r > 0 and r < tempd):
                        tempd = r
                        cell_lim_bound = []
                        cell_lim_bound.append((axis, side))
                    elif r == tempd:
                        cell_lim_bound.append((axis, side))
            # move neutron
            neutron.move(tempd)

            # determine boundary status
            for axis, side in cell_lim_bound:
                if cell_boundaries[side][axis] == \
                        bounds.get_surface_coord(axis, side):
                    box_lim_bound.append((axis, side))

            # check boundary conditions on all hit surfaces
            for surface, side in box_lim_bound:  
                
                # if the neutron is reflected
                if bounds.get_surface_type(surface, side) == 1:
                    neutron.reflect(surface)

                    #place neutron on boundary to eliminate roundoff error
                    bound_val = bounds.get_surface_coord(surface, side)
                    neutron.set_position(surface, bound_val)
                # if the neutron escapes
                elif bounds.get_surface_type(surface, side) == 0:
                    neutron.kill()
                    neutron_distance = tempd
                    tallies['leaks'].add(1)
            
            # shorten neutron distance to collision
            neutron_distance -= tempd

            # add distance to cell flux
            mesh.flux_add(cell, tempd, neutron.group)
            
        # check interaction
        if neutron.alive:
            mat = mesh.get_material(neutron.get_cell())

            # sample what the interaction will be
            neutron_interaction = sample_interaction(mat,
                    neutron.group)

            # scattering event
            if neutron_interaction == 0:
                
                # sample scattered direction
                theta = sample_polar_angle()
                phi = sample_azimuthal_angle()

                # sample what group the neutron will scatter to
                new_group = sample_scattered_group(mat.sigma_s, group)

                # set new group
                neutron.set_group(new_group)

                # set direction
                neutron.set_direction(theta, phi)

                # reassign cell location
                cell = mesh.get_cell(neutron.get_position_vector(),
                        neutron.get_direction_vector())
                neutron.set_cell(cell)

            # absorption event
            else:

                # tally absorption
                tallies['absorptions'].add(1)
                
                # sample for fission event
                mat = mesh.get_material(neutron.get_cell())
                if sample_fission(mat, neutron.group) == 1:
                    
                    # sample number of neutrons
                    for j in xrange(sample_num_fission( \
                            mesh.get_material(neutron.get_cell()))):
                        fission_banks['new'].add(neutron.get_position_vector())
                        tallies['fissions'].add(1)
                # end neutron history
                neutron.kill()

    # tally crow distance
    crow_distance = neutron.get_distance(neutron_starting_point)
    tallies['crows'].add(crow_distance)
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
def generate_neutron_histories(n_histories, bounds,
        mesh, num_batches):
    
    crow_distances = tally.Tally()
    num_crow_distances = tally.Tally()
    leakage_tally = tally.Tally()
    absorption_tally = tally.Tally()
    fission_tally = tally.Tally()
    mesh_tallies = {'x': [], 'y': [], 'z': []}
    tallies = {'crows': crow_distances, 'num_crows': num_crow_distances,
            'leaks': leakage_tally, 'absorptions': absorption_tally,
            'fissions': fission_tally}
    old_fission_locations = fission.Fission()
    new_fission_locations = fission.Fission()
    fission_banks = {'old': old_fission_locations,
            'new': new_fission_locations}
    first_round = True
    for batch in xrange(num_batches):
       
        #clear flux data
        mesh.flux_clear()

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
            transport_neutron(bounds, tallies, fission_banks,
                    first_round, mesh, batch)

        # give results
        print 'For batch ', batch + 1, ' k = ', \
                tallies['fissions'].count/(tallies['leaks'].count + \
                tallies['absorptions'].count)
        first_round = False
    mean_crow_distance = crow_distances.count / num_crow_distances.count
    print 'Mean crow fly distance = ', mean_crow_distance
