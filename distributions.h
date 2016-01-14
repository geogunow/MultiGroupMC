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

/** fucntion declarations */
double random(double num),
    sampleAzimuthalAngle(),
    samplePolarAngle(),
    sampleDistance(Material mat, int group);
int sampleInteraction(Material mat, int group),
    sampleFission(Material mat, int group),
    sampleNumFission(Material mat),
    sampleNeutronEnergyGroup(std::vector <double> chi),
    sampleScatteredGroup(double **scattering_matrix, int group);
std::vector <double> sampleLocation(Boundaries bounds),
    sampleFissionSite(Fission fission_bank);

#endif
