/**
 @file      distributions.cpp
 @brief     Functions for sampling monte carlo distributions
 @details   This file contains functions to be used in a Monte Carlo simulation
            to sample the various distributions arising in reactor physics.
 @author    Luke Eure
 @date      January 8, 2016
*/
#include "distributions.h"

/**
  @brief    Returns a random number between 0 and a give valule
  @param    num a double containing the maximum value of the range
  @return   a double in the range (0, num)

*/
double random(double num) {
    return (rand() % (int)(num * 100000000) / 100000000.0);
}

/**
  @brief    Function that randomly samples an azimuthal angle                   
  @details  An azimuthal angle is uniformally sampled in [0, 2 pi]              
  @return   A randomly sampled angle in [0, 2pi] 
*/
double sampleAzimuthalAngle() {
    return 2 * M_PI*random(1.0);
}

/**
 @brief     Function that randomly samples a polar angle
 @details   The cosine of the polar angle is uniformally sampled in [-1, 1]
            and then transformed to the polar angle with an inverse cosine
 @return    A randomly sampled polar angle in [0, pi]
*/
double samplePolarAngle() {
    double cos_theta = 2*random(1.0) - 1.0;
    return acos(cos_theta);

}

/**
 @brief     Function that samples the distance of travel based on a total
            cross-section.
 @details   A distance of travel is sampled in [0, infinity) assuming an
            exponential distibution of the form sigma_t * exp(-sigma_t * d).
 @param     mat a Material object that contains information about the material
 @param     group an int denoting the energy group of the neutron
 @return    A randomly sampled distance in [0, infinity)
*/
double sampleDistance(Material mat, int group) {
    std::vector <double> _temp_sigma_t;
    _temp_sigma_t = mat.getSigmaT();
    return -log(random(1.0)) / _temp_sigma_t[group];
}

/**
 @brief     Function that samples the interaction type (0 = scattering,
            1 = absorption)
 @details   Based on cross-sections, the interaction is sampled as scattering
            (0) or absorption (1).
 @param     mat a Material object that contains information
            about the material
 @param     group an int denoting the energy group of the neutron 
 @return    An interaction type (0 = scattering, 1 = absorption)
*/
int sampleInteraction(Material mat, int group) {
    
    return (int)(random(1.0) < (mat.getSigmaA()[group] /
                mat.getSigmaT()[group]));
}

/**
 @brief     Function that samples a random location within a bounding box.
 @details   A point is randomly and uniformally sampled in the bounding box 
            provided in the input.
 @param     bounds a Boundaries object containing the limits of the
            bounding box
 @return    _dist_location a vector that contains the coordinates of a point
*/
std::vector <double> sampleLocation(Boundaries bounds) {
    
    std::vector <double> _dist_location;
    for (int i=0; i<3; ++i) {
       
        /** is this the correct way to sample a rondom location? */
        _dist_location.push_back(bounds.getSurfaceCoord(i, 0) +
            bounds.getSurfaceCoord(i, 1) - bounds.getSurfaceCoord(i, 0)
            * random(1.0));
    }
    return _dist_location;
}

/**
 @brief     Function that samples the interaction type given an absorption
            (0 = capture, 1 = fission)
 @details   Based on cross-sections, the interaction is sampled as capture
            (0) or fission (1).
 @param     mat a Material object that contains information about the material
 @param     group contains the energy group of the neutron
 @return    An interaction type (0 = capture, 1 = fission)
*/
int sampleFission(Material mat, int group) {
    return (int)(random(1.0) <
            (mat.getSigmaF()[group] / mat.getSigmaA()[group]));
}

/**
 @brief     Samples the nunber of neutrons produced from a fission event
 @param     mat a Material object that contains information about the material
 @return    number of neutrons emitted from the sampled fission event
*/
int sampleNumFission(Material mat) {
    int lower = (int)(mat.getNu());
    int add = (int)(random(1.0) < (mat.getNu()-lower));
    return lower + add;
}

/**
 @brief     Samples a neutron position in the fission bank
 @param     fission_bank a Fission object containing neutron locations
 @return    sampled neutron location
*/
std::vector <double> sampleFissionSite(Fission fission_bank) {
    int index = rand() % fission_bank.length();
    return fission_bank.location(index);
}

/**
 @brief     Samples an initial neutron energy group after fission
 @param     chi the neutron emission spectrum from fission
 @return    the group number of the emitted neutron
*/
int sampleNeutronEnergyGroup(std::vector <double> chi) {
    double r = random(1.0);
    double chi_sum = 0.0;
    for (int g=0; g<chi.size(); ++g) {
        chi_sum += chi[g];
        if (r<chi_sum) {
            return g;
        }
    }
    return chi.size() - 1;
}

/**
 @brief     Samples the neutron energy group after a scattering event
 @param     scattering_matrix the scattering cross section matrix
 @param     group the neutron energy group before scattering
 @return    the neutron group after scattering
*/
int sampleScatteredGroup(std::vector <std::vector <double> > &scattering_matrix,
        int group) {
    int num_groups = scattering_matrix.size();
    double r = random(1.0) * num_groups;
    double scatter_sum = 0.0;
    for (int g=0; g<num_groups; ++g) {
        scatter_sum += scattering_matrix[group][g];
        if (r<scatter_sum) {
            return g;
        }
    }
    return num_groups - 1;
}
