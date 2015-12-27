'''
 @file      distributions.py
 @brief     Functions for sampling monte carlo distributions
 @details   This file contains functions to be used in a Monte Carlo simulation
            to sample the various distributions arising in reactor physics.
 @author    Geoffrey Gunow
 @date      October 7, 2015
'''

# Import libraries and set random number seed to 0
from copy import deepcopy as copy
import random
from math import *
import numpy as np
import fission
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
            because the cosine of the polar should be unifromly sampled,
            not the actual angle directly.
 @return    A randomly sampled polar angle in [0, pi]
'''
def sample_incorrect_polar_angle():
    return pi*random.random()

'''
 @brief     Function that samples the distance of travel based on a total
            cross-section.
 @details   A distance of travel is sampled in [0, infinity) assuming an
            exponential distibution of the form sigma_t * exp(-sigma_t * d).
 @param     mat a Material object that contains information about the material
 @return    A randomly sampled distance in [0, infinity)
'''
def sample_distance(mat, group):
    return -log(random.random()) / mat.sigma_t[group]

'''
 @brief     Function that samples the interaction type (0 = scattering,
            1 = absorption)
 @details   Based on cross-sections, the interaction is sampled as scattering
            (0) or absorption (1).
 @param     mat a Material object that contains information
            about the material            
 @return    An interaction type (0 = scattering, 1 = absorption)
'''
def sample_interaction(mat, g):
    return int(random.random() < sum(mat.sigma_s[g]) / mat.sigma_t[g])

'''
 @brief     Function that samples a random location within a bounding box.
 @details   A point is randomly and uniformally sampled in the bounding box 
            provided in the input.
 @param     bounds a Boundaries object containing the limits of the
            bounding box
 @return    point a numpy array that contains the coordinates of a point
'''
def sample_location(bounds):
    location = np.empty(shape = (3))
    for i, axis in enumerate(['x', 'y', 'z']):
        location[i] = bounds.get_surface_coord(axis, 'min') \
                + (bounds.get_surface_coord(axis, 'max') - \
                bounds.get_surface_coord(axis, 'min')) * random.random()
    return location            

'''
 @brief     Function that samples the interaction type given an absorption
            (0 = capture, 1 = fission)
 @details   Based on cross-sections, the interaction is sampled as capture
            (0) or fission (1).
 @param     mat a Material object that contains information about the material
 @return    An interaction type (0 = capture, 1 = fission)
'''
def sample_fission(mat, g):
    return int(random.random() < mat.sigma_f[g] / mat.sigma_a[g])

'''
 @brief     Samples the nunber of neutrons produced from a fission event
 @param     mat a Material object that contains information about the material
 @return    number of neutrons emitted from the sampled fission event
'''
def sample_num_fission(mat):
    lower = int(mat.nu)
    add = int(random.random() < (mat.nu - lower))
    return lower + add

'''
 @brief     Samples a neutron position in the fission bank
 @param     fission_bank a Fission object containing neutron locations
 @return    sampled neutron location
'''
def sample_fission_site(fission_bank):
    if fission_bank.length >= 1:
        index = random.randint(0, fission_bank.length-1)
        point = fission_bank.location(index)
    else:
        print 'ERROR, No fission sites'
        exit()
        point = [0,0,0]
    return point

'''
 @brief     Samples an initial neutron energy group after fission
 @param     chi the neutron emission spectrum from fission
 @return    the group number of the emitted neutron
'''
def sample_neutron_energy_group(chi):
    
    # sample random number 
    r = random.random()

    # find which group the random number is in
    chi_sum = 0
    for g, chi_g in enumerate(chi):
        chi_sum += chi_g
        if r < chi_sum:
            return g

    return len(chi) - 1

'''
 @brief     Samples the neutron energy group after a scattering event
 @param     g the neutron energy group before scattering
 @param     scattering_matrix the scattering cross section matrix
 @return    the neutron group after scattering
'''
def sample_scattered_group(scattering_matrix, g):
    
    # sample random number 
    r = random.random() * sum(scattering_matrix[g])

    # find which group the random number is in
    scatter_sum = 0
    for gp, scatter_gp in enumerate(scattering_matrix[g]):
        scatter_sum += scatter_gp
        if r < scatter_sum:
            return gp

    return len(scattering_matrix) - 1
