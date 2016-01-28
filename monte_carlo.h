/** 
 @file      monte_carlo.h
 @brief     header file for monte_carlo.cpp
 @author    Luke Eure
 @date      January 12 2016
*/

#ifndef MONTE_CARLO_H
#define MONTE_CARLO_H

#include <iostream>
#include <vector>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <algorithm>

#include "fission.h"
#include "distributions.h"
#include "tally.h"
#include "mesh.h"
#include "neutron.h"

enum tally_names {CROWS, NUM_CROWS, LEAKS, ABSORPTIONS, FISSIONS};
enum fission_bank_names {OLD, NEW};

void generateNeutronHistories(int n_histories, Boundaries bounds,
        Mesh mesh, int num_batches);

void transportNeutron(Boundaries bounds, std::vector <Tally> &tallies,
        bool first_round, Mesh mesh,
        Fission *old_fission_bank, Fission *new_fission_bank);

#endif
