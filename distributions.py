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
    return int(random.random() < sigma_s / sigma_t):

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
 @brief     Function that samples the interaction type given an absorption
            (0 = capture, 1 = fission)
 @details   Based on cross-sections, the interaction is sampled as capture
            (0) or fission (1).
 @param     sigma_a the absorption cross-section of the material region.
 @param     sigma_f the fission cross-section of the material region.
 @return    An interaction type (0 = capture, 1 = fission)
'''
def sample_fission(simga_a, sigma_f):
    return int(random.random() < sigma_f / sigma_a):


'''
 @brief     Samples the nunber of neutrons produced from a fission event
 @param     nu average number of neutrons emitted per fission
 @return    number of neutrons emitted from the sampled fission event
'''
def sample_num_fission(nu):
    lower = int(nu)
    add = int(random.random() < (nu - lower))
    return lower + add


'''
 @brief     Samples a neutron position in the fission bank
 @param     fission_bank a list of neutron locations in the fission bank
 @return    sampled neutron location
'''
def sample_fission_site(fission_bank):
    index = random.randint(0, len(fission_bank)-1)
    return copy(fission_bank[index])
