/** 
 @file      distributions.h
 @brief     header file for distribution.cpp
 @author    Luke Eure
 @date      January 8 2016
*/


#ifndef DISTRIBUTIONS_H
#define DISTRIBUTIONS_H

#include <time.h>
#include <math.h>
#include <stdlib.h>

#include "global_constants.h"
#include "material.h"
#include "boundaries.h"
#include "fission.h"


double random(double num);

double sampleAzimuthalAngle();

double samplePolarAngle();

double sampleDistance(Material mat, int group);

int sampleInteraction(Material mat, int group);

double* sampleLocation(Boundaries bounds);

int sampleFission(Material mat, int group);

int sampleNumFission(Material mat);

double* sampleFissionSite(Fission fission_bank);

int sampleNeutronEnergyGroup(double *chi);

int sampleScatteredGroup(double **scattering_matrix, int group);


#endif
