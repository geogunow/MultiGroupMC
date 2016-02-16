/* 
 @file      Distributions.h
 @brief     header file for distribution.cpp
 @author    Luke Eure
 @date      January 8 2016
*/

#ifndef DISTRIBUTIONS_H
#define DISTRIBUTIONS_H

#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <vector>

#include "Neutron.h"

// function declarations
int sampleNeutronEnergyGroup(std::vector <double> chi, Neutron *neutron);
int sampleScatteredGroup(std::vector <double> &scattering_matrix, int group,
        Neutron *neutron);
std::vector <double> sampleFissionSite(
        std::vector <std::vector <double> > &fission_bank, Neutron *neutron);

#endif
