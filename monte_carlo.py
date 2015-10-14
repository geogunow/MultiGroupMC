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

'''
 @brief     Function that generates a neutron and measures how 
            far it travels before being absorbed.
 @details   A neutron is created in the bounding box using
            sample_location(). It moves a distance determined by
            sample_distance(). It is then either absorbed or
            scattered as determined by sample_interaction(). When
            it is absorbed, its distance from its starting point
            is appended to crow_distances.
 @param	    mat an instance of the Material class containing information
            about the material
 @param     bounds an instance of the boundaries class containing the limits
            of the bounding box
 @param     crow_distances an instances of the Tally class containg the
            distances from each neutron's starting point to the point that
            it is absorbed.
 @param     num_crow_distances is an instance of the Tally class containing
            the number of neutrons whose crow distances have been calculated
 @param     leakage_tally an instance of teh Tally class containing the
            number of neutrons that leak out of the bounding box
 @param     absorbtion_tally an instance of the Tally class containing
            the number of neutrons absorbed
 @param     fission_tally an instance of the Tally class containing the
            number of fission occurences 


'''
def transport_neutron(mat, bounds, crow_distances, num_crow_distances, \
        leakage_tally, absorbtion_tally, fission_tally):
    
    neutron_starting_point = sample_location(bounds)
    # boolean saying if neutron has left the bounds
    neutron_lost = False
    neutron_position = coords.Coords(neutron_starting_point.x, \
            neutron_starting_point.y, neutron_starting_point.z) 
    neutron_interaction = 0
    while neutron_interaction != 1:
        neutron_distance = sample_distance(mat)
        theta = sample_polar_angle()
        phi = sample_azimuthal_angle()
        # neutron movement unit vector
        nmuv = coords.Coords( \
                sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta))

        while neutron_distance > 0:
            # neutron distance vector
            ndv = nmuv.__mul__([neutron_distance, neutron_distance, \
                    neutron_distance])
            # potential neutron position
            pnp = neutron_position.__add__(ndv)
            if (pnp[0] > bounds.get_x_max() or pnp[0] < bounds.get_x_min() or \
                    pnp[1] > bounds.get_y_max() or pnp[1] < bounds.get_y_min() or \
                    pnp[2] > bounds.get_z_max() or pnp[2] < bounds.get_z_min()):
                xmaxd = bounds.get_x_max() - neutron_position.x
                xmind = bounds.get_x_min() - neutron_position.x
                ymaxd = bounds.get_y_max() - neutron_position.y
                ymind = bounds.get_y_min() - neutron_position.y
                zmaxd = bounds.get_z_max() - neutron_position.z
                zmind = bounds.get_z_min() - neutron_position.z
               
                # lim_bound contains the string of the limiting boundary
                lim_bound = ''
                # tempd contains the current smallest r
                tempd = neutron_distance
                # r is a variable that contains the distance along the
                # direction vector to the boundary being tested 
                #test xmax
                r = xmaxd/(sin(theta)*cos(phi))
                if (0 <= r and r < neutron_distance):
                    tempd = r
                    lim_bound = 'x_max'
                #test xmin
                r = xmind/(sin(theta)*cos(phi))
                if (0 <= r and r < neutron_distance and r < tempd):
                    tempd = r
                    lim_bound = 'x_min'
                #test ymax
                r = ymaxd/(sin(theta)*sin(phi))
                if (0 <= r and r < neutron_distance and r < tempd):
                    tempd = r
                    lim_bound = 'y_max'
                #test ymin
                r = ymind/(sin(theta)*sin(phi))
                if (0 <= r and r < neutron_distance and r < tempd):
                    tempd = r
                    lim_bound = 'y_min'
                #test zmax
                r = zmaxd/cos(theta)
                if (0 <= r and r < neutron_distance and r < tempd):
                    tempd = r
                    lim_bound = 'z_max'
                #test zmin
                r = zmind/cos(theta)
                if (0 <= r and r < neutron_distance and r < tempd):
                    tempd = r
                    lim_bound = 'z_min'
                
                # if the neutron escapes
                if (bounds.get_surface_type(lim_bound) == 0):
                    neutron_lost = True
                    neutron_interaction = 1
                    neutron_distance = 0
                    leakage_tally.increment(1)
                # if the neutron is reflected
                elif (bounds.get_surface_type(lim_bound) == 1):
                    neutron_position.__sadd__(nmuv.__mul__(tempd))
                    if lim_bound == 'x_max' or lim_bound == 'x_min':
                        nmuv.__smul__([-1, 1, 1])
                    if lim_bound == 'y_max' or lim_bound == 'y_min':
                        nmuv.__smul__([1, -1, 1])
                    if lim_bound == 'z_max' or lim_bound == 'z_min':
                        nmuv.__smul__([1, 1, -1])
                    neutron_distance -= tempd
            else:
                neutron_position.__sadd__(ndv)    
                neutron_distance = 0
        if neutron_lost == False:
            neutron_interaction = sample_interaction(mat)
    # neutron_position turns into a vector pointing from the neutron's origin
    # to its absorbtion point
    if neutron_lost == False:
        absorbtion_tally.increment(1)
        neutron_position.__ssub__( \
                [neutron_starting_point.x, neutron_starting_point.y, \
                neutron_starting_point.z])
        crow_distances.increment(neutron_position.getDistance())
        num_crow_distances.increment(1)
'''
 @brief     Generates and transports neutron histories, calculates the mean
            crow distance
 @param     n_histories number of neutron histories to run
 @param     mat an instance of the Materail class containing information
            about the material
 @param     bounds an instance of the boundaries class containing the limits
            of the bounding box
'''
def generate_neutron_histories(n_histories, mat, bounds):
    
    crow_distances = tally.Tally()
    num_crow_distances = tally.Tally()
    leakage_tally = tally.Tally()
    absorbtion_tally = tally.Tally()
    fission_tally = tally.Tally()

    for i in xrange(n_histories):
        transport_neutron(mat, bounds, crow_distances, num_crow_distances, \
                leakage_tally, absorbtion_tally, fission_tally)

    mean_crow_distance = crow_distances.amt / num_crow_distances.amt
    print "Mean crow fly distance = ", mean_crow_distance
    print "Leakages: ", leakage_tally.amt
    print "Absorbtions: ", absorbtion_tally.amt
    print "Fissions: ", fission_tally.amt


'''
 @brief     Function that determines what a neutron's new position
            is, taking into account the surface
 @details   A neutron is created in the bounding box using
            sample_location(). It moves a distance determined by
            sample_distance(). It is then either absorbed or
            scattered as determined by sample_interaction(). When
            it is absorbed, its distance from its starting point
            is appended to crow_distances.
 @param	    mat an instance of the Material class containing information
            about the material
 @param     bounds an instance of the boundaries class containing the limits
            of the bounding box
 @param     crow_distances a list containg the distances
            from each neutron's starting point to the point that
            it is absorbed.
'''
