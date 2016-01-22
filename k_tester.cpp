#include <iostream>
#include <vector>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#include "material.h"
#include "fission.h"
#include "boundaries.h"
#include "distributions.h"
#include "tally.h"
#include "mesh.h"
#include "monte_carlo.h"

int main() {

    srand(time(NULL));

    /** create geometry */
    Boundaries test_boundary;
    test_boundary.setSurfaceCoord(0, 0, -2.0);
    test_boundary.setSurfaceCoord(0, 1, 2.0);
    test_boundary.setSurfaceCoord(1, 0, -2.0);
    test_boundary.setSurfaceCoord(1, 1, 2.0);
    test_boundary.setSurfaceCoord(2, 0, -2.0);
    test_boundary.setSurfaceCoord(2, 1, 2.0);
    test_boundary.setSurfaceType(0, 0, REFLECTIVE);
    test_boundary.setSurfaceType(0, 1, REFLECTIVE);
    test_boundary.setSurfaceType(1, 0, REFLECTIVE);
    test_boundary.setSurfaceType(1, 1, REFLECTIVE);
    test_boundary.setSurfaceType(2, 0, REFLECTIVE);
    test_boundary.setSurfaceType(2, 1, REFLECTIVE);

    const int num_groups = 1;

    /** fuel cross sections */
    std::vector <double> fuel_sigma_f;
    fuel_sigma_f.push_back(1.0/16.0);
    std::vector <double> fuel_chi;
    fuel_chi.push_back(1.0);
    std::vector <double> fuel_sigma_t;
    fuel_sigma_t.push_back(5.0/6.0);

    /** fuel sigma_s */
    static const double a_fuel_sigma_s [num_groups*num_groups] =
        { 11.0/15.0 };
    std::vector <std::vector <double> > fuel_sigma_s (num_groups, 
            (std::vector <double> (num_groups)));
    for (int g=0; g<num_groups; ++g) {
        for (int gp=0; gp<num_groups; ++gp) {
            fuel_sigma_s[g][gp] = a_fuel_sigma_s[g*num_groups+gp];
        }
    }

    /** nu */
    double nu = 2.4;
    
    /** create materials */
    Material fuel(fuel_sigma_t, fuel_sigma_s, nu, fuel_sigma_f, fuel_chi);

    /* create mesh */
    Mesh test_mesh(test_boundary, 4.0/9.0, 4.0/9.0, 4.0/9.0, fuel, num_groups);

/**
    std::vector <int> testing_cell;
    std::vector <double> cell_mins;
    cell_mins.resize(3);
    testing_cell.push_back(6);
    testing_cell.push_back(4);
    testing_cell.push_back(2);
    cell_mins = test_mesh.getCellMax(testing_cell);
    std::cout << "cell mins ";
    for (int i=0; i<3; i++) {
        std::cout << cell_mins[i] << " ";
    }
   */
    
    generateNeutronHistories(10000, test_boundary, test_mesh, 5);

    std::cout << "\nk_tester has run " << std::endl;
    return 0;
}

/**
    2 group:
    k = [nu*sigma_f1 + nu*sigma_f2(sigmaS(1-2)/sigma_a2) ]/ sigma_a1
*/
