/* 
 @file      Material.cpp
 @brief     contains functions for the Material class
 @author    Luke Eure
 @date      January 9 2016
*/

#include "Material.h"

/*
 @brief constructor for Material class
 @param sigma_t a vector constaining the total cross section
 @param sigma_s a 2d vector containing the scattering cross section 
 @param nu a double containing the average number of neutrons produced 
        per fission
 @param sigma_f a vector containing the fission cross section
 @param chi a vector the initial energy distribution of neutrons
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

    // sigma_a
    for (int i=0; i < _num_groups; ++i) {
        _s_sum = 0.0;
        for (int j=0; j < _num_groups; ++j) {
            _s_sum += _sigma_s[i][j];
        }
       _sigma_a.push_back(_sigma_t[i] - _s_sum);
    }
}

/*
 @brief deconstructor
*/
Material::~Material() {}

/*
 @brief returns sigma_t for the material, a standard vector containing the total
        cross section for each energy group
 @return sigma_t
*/
double Material::getSigmaT(int group) {
    return _sigma_t[group];
}

/*
 @brief returns sigma_s for the material, a standard vector containing the
        scattering cross section for each energy group
 @return sigma_s
*/
std::vector <double> Material::getSigmaS(int group)  {
    return _sigma_s[group];
}

/*
 @brief returns nu for the material: the average number of neutrons released
        per fission event
 @return nu
*/
double Material::getNu() {
    return _nu;
}

/*
 @brief returns sigma_f for the material, a standard vector containing the 
        fission cross section for each energy group
 @return sigma_f
*/
double Material::getSigmaF(int group) {
    return _sigma_f[group];
}

/*
 @brief returns chi for the material, a standard vector containing the
        initial energy distribution of neutrons
 @return chi
*/
std::vector <double> Material::getChi() {
    return _chi;
}

/*
 @brief returns sigma_a
 @brief returns sigma_a for the material, a standard vector containing the
        absorption cross section for each energy group
 @return sigma_a
*/
double Material::getSigmaA(int group) {
    return _sigma_a[group];
}
