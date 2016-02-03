/*
 @file      distributions.cpp
 @brief     Functions for sampling monte carlo distributions
 @details   This file contains functions to be used in a Monte Carlo simulation
            to sample the various distributions arising in reactor physics.
 @author    Luke Eure
 @date      January 8, 2016
*/
#include "distributions.h"

/*
  @brief    Returns a uniform random number between 0 and 1
  @return   a double in the range (0, 1)
*/
double urand() {
    return (double) rand() / (double) RAND_MAX;
}

/*
  @brief    Function that randomly samples an azimuthal angle                   
  @details  An azimuthal angle is uniformally sampled in [0, 2 pi]              
  @return   A randomly sampled angle in [0, 2pi] 
*/
double sampleAzimuthalAngle() {
    return 2 * M_PI*urand();
}

/*
 @brief     Function that randomly samples a polar angle
 @details   The cosine of the polar angle is uniformally sampled in [-1, 1]
            and then transformed to the polar angle with an inverse cosine
 @return    A randomly sampled polar angle in [0, pi]
*/
double samplePolarAngle() {
    double cos_theta = 2*urand() - 1.0;
    return acos(cos_theta);

}

/*
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
    return -log(urand()) / _temp_sigma_t[group];
}

/*
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
    
    return (int)(urand() < mat.getSigmaA()[group] /
                mat.getSigmaT()[group]);
}

/*
 @brief     Function that samples a random location within a bounding box.
 @details   A point is randomly and uniformally sampled in the bounding box 
            provided in the input.
 @param     bounds a Boundaries object containing the limits of the
            bounding box
 @return    _dist_location a vector that contains the coordinates of a point
*/
std::vector <double> sampleLocation(Boundaries bounds) {
    std::vector <double> _dist_location;
    double width;
    double coord;
    for (int axis=0; axis<3; ++axis) {
        width = bounds.getSurfaceCoord(axis, 1)
            - bounds.getSurfaceCoord(axis, 0);
        coord = bounds.getSurfaceCoord(axis, 0) + width * urand();
        _dist_location.push_back(coord);
    }
    return _dist_location;
}

/*
 @brief     Function that samples the interaction type given an absorption
            (0 = capture, 1 = fission)
 @details   Based on cross-sections, the interaction is sampled as capture
            (0) or fission (1).
 @param     mat a Material object that contains information about the material
 @param     group contains the energy group of the neutron
 @return    An interaction type (0 = capture, 1 = fission)
*/
int sampleFission(Material mat, int group) {
    int fission = urand() < mat.getSigmaF()[group] / mat.getSigmaA()[group];
    return fission;
}

/*
 @brief     Samples the nunber of neutrons produced from a fission event
 @param     mat a Material object that contains information about the material
 @return    number of neutrons emitted from the sampled fission event
*/
int sampleNumFission(Material mat) {
    int lower = (int)mat.getNu();
    int add = (int)(urand() < mat.getNu()-lower);
    return lower + add;
}

/*
 @brief     Samples a neutron position in the fission bank
 @param     fission_bank a Fission object containing neutron locations
 @return    sampled neutron location
*/
std::vector <double> sampleFissionSite(Fission* fission_bank) {
    int index = rand() % fission_bank->length();
    return fission_bank->location(index);
}

/*
 @brief     Samples an initial neutron energy group after fission
 @param     chi the neutron emission spectrum from fission
 @return    the group number of the emitted neutron
*/
int sampleNeutronEnergyGroup(std::vector <double> chi) {
    double r = urand();
    double chi_sum = 0.0;
    for (int g=0; g<chi.size(); ++g) {
        chi_sum += chi[g];
        if (r<chi_sum) {
            return g;
        }
    }
    return chi.size() - 1;
}

/*
 @brief     Samples the neutron energy group after a scattering event
 @param     scattering_matrix the scattering cross section matrix
 @param     group the neutron energy group before scattering
 @return    the neutron group after scattering
*/
int sampleScatteredGroup(std::vector <std::vector <double> > &scattering_matrix,
        int group) {

    // Get the total scattering cross-section from this group
    int num_groups = scattering_matrix.size();
    double scattering_total = 0;
    for (int g=0; g < num_groups; ++g)
        scattering_total += scattering_matrix[group][g];

    // Sample the outgoing scattered energy group
    double r = urand() * scattering_total;
    double scatter_sum = 0.0;
    for (int g=0; g<num_groups; ++g) {
        scatter_sum += scattering_matrix[group][g];
        if (r<scatter_sum) {
            return g;
        }
    }

    // Return the last group if no group has been found yet 
    return num_groups - 1;
}
