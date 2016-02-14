/* 
 @file      Material.cpp
 @brief     contains functions for the Material class
 @author    Luke Eure
 @date      January 9 2016
*/

#include "Material.h"

/*
 @brief     constructor for Material class
 @param     sigma_t a vector constaining the total cross section
 @param     sigma_s a 2d vector containing the scattering cross section 
 @param     nu a double containing the average number of neutrons produced 
            per fission
 @param     sigma_f a vector containing the fission cross section
 @param     chi a vector the initial energy distribution of neutrons
*/
Material::Material(std::vector <double> &sigma_t, 
        std::vector <std::vector <double> > &sigma_s, double nu, 
        std::vector <double> &sigma_f, std::vector <double> &chi) {

    // store variables
    _sigma_t = sigma_t;
    _sigma_s = sigma_s;
    _nu = nu;
    _sigma_f = sigma_f;
    _chi = chi;
   
    // save number of groups
    _num_groups = sigma_t.size();
    _sigma_a.resize(_num_groups);

    // calculate and save the value for absorption cross sections
    for (int i=0; i < _num_groups; ++i) {
        _s_sum = 0.0;
        for (int j=0; j < _num_groups; ++j) {
            _s_sum += _sigma_s[i][j];
        }
       _sigma_a[i] = _sigma_t[i] - _s_sum;
    }
}

/*
 @brief     deconstructor
*/
Material::~Material() {}

/*
 @brief     returns sigma_t for the material, a standard vector
            containing the total cross section for each energy group
 @param     group the energy group of the neutron
 @return    sigma_t, the total cross section
*/
double Material::getSigmaT(int group) {
    return _sigma_t[group];
}

/*
 @brief     returns sigma_s for the material, a standard vector containing the
            scattering cross section for each energy group
 @param     group the energy group of the neutron
 @return    sigma_s, the scatttering cross section
*/
std::vector <double> Material::getSigmaS(int group)  {
    return _sigma_s[group];
}

/*
 @brief     returns nu for the material: the average number of neutrons released
            per fission event
 @return    nu, the average number of neutrons released per fission event
*/
double Material::getNu() {
    return _nu;
}

/*
 @brief     returns sigma_f for the material, a standard vector containing the 
            fission cross section for each energy group
 @param     group the energy group of the neutron
 @return    sigma_f, the fission cross section
*/
double Material::getSigmaF(int group) {
    return _sigma_f[group];
}

/*
 @brief     returns chi for the material, a standard vector containing the
            initial energy distribution of neutrons
 @param     group the energy group of the neutron
 @return    chi, the neutron emission spectrum
*/
double Material::getChi(int group) {
    return _chi[group];
}

/*
 @brief     returns sigma_a for the material, a standard vector containing the
            absorption cross section for each energy group
 @param     group the energy group of the neutron
 @return    sigma_a, the absorption cross section
*/
double Material::getSigmaA(int group) {
    return _sigma_a[group];
}

/*
 @brief     function that samples the interaction type (0 = scattering,
            1 = absorption)
 @details   based on cross-sections, the interaction is sampled as scattering
            (0) or absorption (1).
 @param     group an int denoting the energy group of the neutron 
 @return    an interaction type (0 = scattering, 1 = absorption)
*/
int Material::sampleInteraction(int group) {
    return (int) (urand() < _sigma_a[group] / _sigma_t[group]);
}

/*
 @brief     function that samples the distance of travel based on a total
            cross-section.
 @details   a distance of travel is sampled in [0, infinity) assuming an
            exponential distibution of the form sigma_t * exp(-sigma_t * d).
 @param     group an int denoting the energy group of the neutron
 @return    a randomly sampled distance in [0, infinity)
*/
double Material::sampleDistance(int group) {
    return -log(urand()) / _sigma_t[group];
}

/*
 @brief     function that samples the interaction type given an absorption
            (0 = capture, 1 = fission)
 @details   based on cross-sections, the interaction is sampled as capture
            (0) or fission (1).
 @param     group contains the energy group of the neutron
 @return    an interaction type (0 = capture, 1 = fission)
*/
int Material::sampleFission(int group) {
    int fission = urand() < _sigma_f[group] / _sigma_a[group];
    return fission;
}

/*
 @brief     samples the nunber of neutrons produced from a fission event
 @return    number of neutrons emitted from the sampled fission event
*/
int Material::sampleNumFission() {
    int lower = (int) _nu;
    int add = (int) (urand() < _nu -lower);
    return lower + add;
}
