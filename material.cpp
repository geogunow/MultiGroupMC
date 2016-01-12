#include <iostream>

#include "material.h"

Material::Material(double *sigma_t, 
        double **sigma_s, double nu, 
        double *sigma_f, double *chi) {

    /** store variables */
    _sigma_t = sigma_t;
    _sigma_s = sigma_s;
    _nu = nu;
    _sigma_f = sigma_f;
    _chi = chi;
    
    /** sigma_a */
    for (int i=0; i < NUM_GROUPS; ++i) {
        _s_sum = 0.0;
        for (int j=0; j < NUM_GROUPS; ++j) {
            _s_sum += _sigma_s[i][j];
        }
       _sigma_a[i] =  _sigma_t[i] - _s_sum;

    }
}

/** not sure if the destructor needs to do anything */
Material::~Material() {}

double* Material::getSigmaT() {
    return _sigma_t;
}

double** Material::getSigmaS()  {
    return _sigma_s;
}

double Material::getNu() {
    return _nu;
}

double* Material::getSigmaF() {
    return _sigma_f;
}

double* Material::getChi() {
    return _chi;
}

double* Material::getSigmaA() {
    return _sigma_a;
}
