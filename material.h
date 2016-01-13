
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

    //What type should _sigma_s be?
    double _sigma_s [][2];
    double _nu;
    double *_sigma_f;
    double *_chi;
    double *_sigma_a;
    double _s_sum;

public:
   
    Material();
    Material(double sigma_t[NUM_GROUPS], 
            double sigma_s[][NUM_GROUPS], double nu, 
            double sigma_f[NUM_GROUPS], double chi[NUM_GROUPS]);
    virtual ~Material();
    
    double* getSigmaT();
    double** getSigmaS();
    double getNu();
    double* getSigmaF();
    double* getChi();
    double* getSigmaA();
};

#endif

