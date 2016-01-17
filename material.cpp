#include "material.h"

/** default constructor */
Material::Material() {}

Material::Material(std::vector <double> &sigma_t, 
        std::vector <std::vector <double> > &sigma_s, double nu, 
        std::vector <double> &sigma_f, std::vector <double> &chi) {

    /** store variables */
    _sigma_t = sigma_t;
    _sigma_s = sigma_s;
    _nu = nu;
    _sigma_f = sigma_f;
    _chi = chi;
   
    /** save number of groups */
    _num_groups = sigma_t.size();

    /** sigma_a */
    for (int i=0; i < _num_groups; ++i) {
        _s_sum = 0.0;
        for (int j=0; j < _num_groups; ++j) {
            _s_sum += _sigma_s[i][j];
        }
       _sigma_a.push_back(_sigma_t[i] - _s_sum);
    }
   
}

/** not sure if the destructor needs to do anything */
Material::~Material() {}

std::vector <double> Material::getSigmaT() {
    return _sigma_t;
}

std::vector <std::vector <double> > Material::getSigmaS()  {
    return _sigma_s;
}

double Material::getNu() {
    return _nu;
}

std::vector <double> Material::getSigmaF() {
    return _sigma_f;
}

std::vector <double> Material::getChi() {
    return _chi;
}

std::vector <double> Material::getSigmaA() {
    return _sigma_a;
}
