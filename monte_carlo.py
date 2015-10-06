'''
 @file      monte_carlo.py
 @brief     Utility functions for monte carlo neutron simulations
 @details   This file contains functions to be used in a Monte Carlo simulation. 
            Later, these funcitons will be re-organized and implemented as 
            objects. In this first assignment, fill in the missing detail 
            described by the TODO statements below.
 @author    Geoffrey Gunow
 @date      September 30, 2015
'''

# Import libraries and set random number seed to 0
import random
from math import *
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
random.seed(0)



'''
 @brief     Function that randomly samples an azimuthal angle
 @details   An azimuthal angle is uniformally sampled in [0, 2 pi]
 @return    A randomly sampled angle in [0, 2pi]
'''
def sample_azimuthal_angle():
    return 2*pi*random.random()


'''
 @brief     Function that randomly samples a polar angle
 @details   The cosine of the polar angle is uniformally sampled in [-1, 1]
            and then transformed to the polar angle with an inverse cosine
 @return    A randomly sampled polar angle in [0, pi]
'''
def sample_polar_angle():
    cos_theta = 2*random.random() - 1
    return acos(cos_theta)


'''
 @brief     Function that incorrecly randomly samples a polar angle
 @details   A polar angle is sampled uniformly in [0, pi]. This is incorrect
            because the cosine of the polar should be unifromly sampled, not the 
            actual angle directly.
 @return    A randomly sampled polar angle in [0, pi]
'''
def sample_incorrect_polar_angle():
    return pi*random.random()


'''
 @brief     Function that samples the distance of travel based on a total
            cross-section.
 @details   A distance of travel is sampled in [0, infinity) assuming an
            exponential distibution of the form sigma_t * exp(-sigma_t * d).
 @param     sigma_t the total cross-section of the material region.
 @return    A randomly sampled distance in [0, infinity)
'''
def sample_distance(sigma_t):
    return -log(random.random()) / sigma_t


'''
 @brief     Function that samples the interaction type (0 = scattering,
            1 = absorption)
 @details   Based on cross-sections, the interaction is sampled as scattering
            (0) or absorption (1).
 @param     sigma_t the total cross-section of the material region.
 @param     sigma_s the scattering cross-section of the material region.
 @return    An interaction type (0 = scattering, 1 = absorption)
'''
def sample_interaction(sigma_t, sigma_s):
    if (random.random() < sigma_s / sigma_t):
        return 0;
    else:
        return 1;


'''
 @brief     Function that samples a random location within a bounding box.
 @details   A point is randomly and uniformally sampled in the bounding box 
            provided in the input.
 @param     x_min the minimum x-coordinate in the bounding box
 @param     x_max the maximum x-coordinate in the bounding box
 @param     y_min the minimum y-coordinate in the bounding box
 @param     y_max the maximum y-coordinate in the bounding box
 @param     z_min the minimum z-coordinate in the bounding box
 @param     z_max the maximum z-coordinate in the bounding box
 @return    A tuple containing the x, y, and z coordinates of the sampled 
            location
'''
def sample_location(x_min, x_max, y_min, y_max, z_min, z_max):
    x_pos = x_min + (x_max - x_min) * random.random()
    y_pos = y_min + (y_max - y_min) * random.random()
    z_pos = z_min + (z_max - z_min) * random.random()
    return (x_pos, y_pos, z_pos)

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
    neutron_position = [neutron_starting_point[i] for i in [0,1,2]] 
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
    for i in [0,1,2]:
        neutron_distance_vector[i] = neutron_position[i] - \
        neutron_starting_point[i]
    crow_distance = sqrt(neutron_distance_vector[0]**2 + \
    neutron_distance_vector[1]**2+neutron_distance_vector[2]**2)
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


'''
 @brief     Creates a scatter plot of 3D points
 @details   Input data referring to point locations is plotted on a 3D graph.
            A new figure is created to plot the data and after plotting, the
            show() function is called, suspending execution.
 @param     xdata the x-coordinates of the data points
 @param     ydata the y-coordinates of the data points
 @param     zdata the z-coordinates of the data points
'''
def plot_3D_points(xdata, ydata, zdata):
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(xdata, ydata, zdata, s=10, c='k')
    plt.show()


