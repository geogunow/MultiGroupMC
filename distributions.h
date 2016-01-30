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

#include "material.h"
#include "boundaries.h"
#include "fission.h"

/** enumerations use when talking about boundaries */
enum min_max {MIN, MAX};

// function declarations
double urand();
double sampleAzimuthalAngle();
double samplePolarAngle();
double sampleDistance(Material mat, int group);
int sampleInteraction(Material mat, int group);
int sampleFission(Material mat, int group);
int sampleNumFission(Material mat);
int sampleNeutronEnergyGroup(std::vector <double> chi);
int sampleScatteredGroup(std::vector <std::vector <double> > &scattering_matrix,
                         int group);
std::vector <double> sampleLocation(Boundaries bounds);
std::vector <double> sampleFissionSite(Fission* fission_bank);

#endif
