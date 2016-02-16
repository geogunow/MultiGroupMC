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
 @brief     samples a neutron position in the fission bank
 @param     fission_bank a Fission object containing neutron locations
 @return    sampled neutron location
*/
std::vector <double> sampleFissionSite(
        std::vector <std::vector <double> > &fission_bank, Neutron *neutron) {
    int index = neutron->rand() % fission_bank.size();
    return fission_bank[index];
}

/*
 @brief     samples an initial neutron energy group after fission
 @param     chi the neutron emission spectrum from fission
 @return    the group number of the emitted neutron
*/
int sampleNeutronEnergyGroup(std::vector <double> chi, Neutron *neutron) {
    double r = neutron->arand();
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
int sampleScatteredGroup(std::vector <double> &scattering_matrix, int group,
        Neutron *neutron) {

    // get the total scattering cross-section from this group
    int num_groups = scattering_matrix.size();
    double scattering_total = 0;
    for (int g=0; g < num_groups; ++g)
        scattering_total += scattering_matrix[g];

    // sample the outgoing scattered energy group
    double r = neutron->arand() * scattering_total;
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
