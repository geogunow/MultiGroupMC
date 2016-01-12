
/** 
 @file      material.h
 @brief     contains the material class
 @author    Luke Eure
 @date      January 6 2016
*/

#ifndef MATERIAL_H
#define MATERIAL_H

#include "global_constants.h"

class Material {
private:
    double *_sigma_t;
    double **_sigma_s;
    double _nu;
    double *_sigma_f;
    double *_chi;
    double _sigma_a [NUM_GROUPS];
    double _s_sum;

public:
    
    Material(double *sigma_t, 
            double **sigma_s, double nu, 
            double *sigma_f, double *chi);
    virtual ~Material();
    
    double* getSigmaT();
    double** getSigmaS();
    double getNu();
    double* getSigmaF();
    double* getChi();
    double* getSigmaA();
};

#endif

