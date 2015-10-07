'''
 @file      monte_carlo.py
 @brief     Utility functions for monte carlo neutron simulations
 @details   This file contains functions to be used in a Monte Carlo simulation. 
            Later, these funcitons will be re-organized and implemented as 
            objects.
 @author    Geoffrey Gunow, Luke Eure
 @date      September 30, 2015
'''

# Import libraries
from math import *
from distributions import *
from plotter import *


'''
 @brief     Function that generates a neutron and measures how 
            far it travels before being absorbed.
 @details   A neutron is created in the bounding box using
            sample_location(). It moves a distance determined by
            sample_distance(). It is then either absorbed or
            scattered as determined by sample_interaction(). When
            it is absorbed, its distance from its starting point
            is appended to crow_distances.
 @param	    sigma_t the total cross-section 
 @param     sigma_s the scattering cross-section
 @param     x_min the minimum x-coordinate in the bounding box
 @param     x_max the maximum x-coordinate in the bounding box
 @param     y_min the minimum y-coordinate in the bounding box
 @param     y_max the maximum y-coordinate in the bounding box
 @param     z_min the minimum z-coordinate in the bounding box
 @param     z_max the maximum z-coordinate in the bounding box
 @param     crow_distances a list containg the distances
            from each neutron's starting point to the point that
            it is absorbed.
'''
def transport_neutron(sigma_t, sigma_s, x_min, x_max, y_min, y_max, z_min, 
    z_max, crow_distances):
    neutron_starting_point = list(sample_location(x_min, x_max, y_min, 
        y_max, z_min, z_max))
    neutron_position = [neutron_starting_point[i] for i in range(3)] 
    neutron_interaction = 0
    while neutron_interaction != 1:
        neutron_distance = sample_distance(sigma_t)
        theta = sample_polar_angle()
        phi = sample_azimuthal_angle()
        neutron_position[0] += neutron_distance*sin(theta)*cos(phi)
        neutron_position[1] += neutron_distance*sin(theta)*sin(phi)
        neutron_position[2] += neutron_distance*cos(theta)
        neutron_interaction = sample_interaction(float(sigma_t),
            float(sigma_s))
    neutron_distance_vector = [0, 0, 0]
    for i in range(3):
        neutron_distance_vector[i] = neutron_position[i] - \
            neutron_starting_point[i]
    crow_distance = sqrt(neutron_distance_vector[0]**2 + \
        neutron_distance_vector[1]**2 + neutron_distance_vector[2]**2)
    crow_distances.append(crow_distance) 
		


'''
 @brief     Generates and transports neutron histories, calculates the mean
            crow distance
 @param     n_histories number of neutron histories to run
 @param     sigma_t total cross section of the material 
 @param     sigma_s scattering cross section of the material
'''
def generate_neutron_histories(n_histories, sigma_t, sigma_s, x_min, x_max,
        y_min, y_max, z_min, z_max):
    
    crow_distances = list()
    for i in xrange(n_histories):
        transport_neutron(sigma_t, sigma_s, x_min, x_max, y_min, y_max, z_min,
                z_max, crow_distances)

    mean_crow_distance = sum(crow_distances) / n_histories
    print "Mean crow fly distance = ", mean_crow_distance



