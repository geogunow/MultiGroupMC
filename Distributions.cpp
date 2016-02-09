/*
 @file      Distributions.cpp
 @brief     functions for sampling monte carlo distributions
 @details   this file contains functions to be used in a Monte Carlo simulation
            to sample the various distributions arising in reactor physics.
 @author    Luke Eure
 @date      January 8, 2016
*/

#include "Distributions.h"

/*
  @brief    returns a uniform random number between 0 and 1
  @return   a double in the range (0, 1)
*/
double urand() {
    return (double) rand() / (double) RAND_MAX;
}

/*
  @brief    function that randomly samples an azimuthal angle                   
  @details  an azimuthal angle is uniformally sampled in [0, 2 pi]              
  @return   a randomly sampled angle in [0, 2pi] 
*/
double sampleAzimuthalAngle() {
    return 2 * M_PI * urand();
}

/*
 @brief     function that randomly samples a polar angle
 @details   the cosine of the polar angle is uniformally sampled in [-1, 1]
            and then transformed to the polar angle with an inverse cosine
 @return    a randomly sampled polar angle in [0, pi]
*/
double samplePolarAngle() {
    double cos_theta = 2 * urand() - 1.0;
    return acos(cos_theta);
}

/*
 @brief     function that samples the distance of travel based on a total
            cross-section.
 @details   a distance of travel is sampled in [0, infinity) assuming an
            exponential distibution of the form sigma_t * exp(-sigma_t * d).
 @param     mat a Material object that contains information about the material
 @param     group an int denoting the energy group of the neutron
 @return    a randomly sampled distance in [0, infinity)
*/
double sampleDistance(Material* mat, int group) {
    return -log(urand()) / mat->getSigmaT(group);
}

/*
 @brief     function that samples the interaction type (0 = scattering,
            1 = absorption)
 @details   based on cross-sections, the interaction is sampled as scattering
            (0) or absorption (1).
 @param     mat a Material object that contains information
            about the material
 @param     group an int denoting the energy group of the neutron 
 @return    an interaction type (0 = scattering, 1 = absorption)
*/
int sampleInteraction(Material* mat, int group) {
    return (int) (urand() < mat->getSigmaA(group)
                / mat->getSigmaT(group));
}

/*
 @brief     function that samples a random location within a bounding box.
 @details   a point is randomly and uniformally sampled in the bounding box 
            provided in the input.
 @param     bounds a Boundaries object containing the limits of the
            bounding box
 @return    _dist_location a vector that contains the coordinates of a point
*/
std::vector <double> sampleLocation(Boundaries bounds) {
    std::vector <double> _dist_location(3);
    for (int axis=0; axis<3; ++axis) {
        double width = bounds.getSurfaceCoord(axis, 1)
            - bounds.getSurfaceCoord(axis, 0);
        double coord = bounds.getSurfaceCoord(axis, 0) + width * urand();
        _dist_location[axis] = coord;
    }
    return _dist_location;
}

/*
 @brief     function that samples the interaction type given an absorption
            (0 = capture, 1 = fission)
 @details   based on cross-sections, the interaction is sampled as capture
            (0) or fission (1).
 @param     mat a Material object that contains information about the material
 @param     group contains the energy group of the neutron
 @return    an interaction type (0 = capture, 1 = fission)
*/
int sampleFission(Material* mat, int group) {
    int fission = urand() < mat->getSigmaF(group) / mat->getSigmaA(group);
    return fission;
}

/*
 @brief     samples the nunber of neutrons produced from a fission event
 @param     mat a Material object that contains information about the material
 @return    number of neutrons emitted from the sampled fission event
*/
int sampleNumFission(Material* mat) {
    int lower = (int) mat->getNu();
    int add = (int) (urand() < mat->getNu()-lower);
    return lower + add;
}

/*
 @brief     samples a neutron position in the fission bank
 @param     fission_bank a Fission object containing neutron locations
 @return    sampled neutron location
*/
std::vector <double> sampleFissionSite(
        std::vector <std::vector <double> > &fission_bank) {
    int index = rand() % fission_bank.size();
    return fission_bank[index];
}

/*
 @brief     samples an initial neutron energy group after fission
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
 @brief     samples the neutron energy group after a scattering event
 @param     scattering_matrix the scattering cross section matrix
 @param     group the neutron energy group before scattering
 @return    the neutron group after scattering
*/
int sampleScatteredGroup(std::vector <double> &scattering_matrix, int group) {

    // get the total scattering cross-section from this group
    int num_groups = scattering_matrix.size();
    double scattering_total = 0;
    for (int g=0; g < num_groups; ++g)
        scattering_total += scattering_matrix[g];

    // sample the outgoing scattered energy group
    double r = urand() * scattering_total;
    double scatter_sum = 0.0;
    for (int g=0; g<num_groups; ++g) {
        scatter_sum += scattering_matrix[g];
        if (r<scatter_sum) {
            return g;
        }
    }

    // return the last group if no group has been found yet 
    return num_groups - 1;
}
