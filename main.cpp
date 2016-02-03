/* 
 @file      main.cpp
 @brief     creates geometry and materials to run a Monte Carlo simulation
 @author    Luke Eure
 @date      January 9 2016
*/

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
#include "plotter.h"

int main() {

    srand(time(NULL));

    // create geometry
    Boundaries test_boundary;
    test_boundary.setSurfaceCoord(0, 0, -2.0);
    test_boundary.setSurfaceCoord(0, 1, 2.0);
    test_boundary.setSurfaceCoord(1, 0, -2.0);
    test_boundary.setSurfaceCoord(1, 1, 2.0);
    test_boundary.setSurfaceCoord(2, 0, -2.0);
    test_boundary.setSurfaceCoord(2, 1, 2.0);
    test_boundary.setSurfaceType(0, 0, VACUUM);
    test_boundary.setSurfaceType(0, 1, VACUUM);
    test_boundary.setSurfaceType(1, 0, VACUUM);
    test_boundary.setSurfaceType(1, 1, VACUUM);
    test_boundary.setSurfaceType(2, 0, VACUUM);
    test_boundary.setSurfaceType(2, 1, VACUUM);

    const int num_groups = 2;

    // fuel cross sections
    std::vector <double> fuel_sigma_f;
    fuel_sigma_f.push_back(1.0/480.0);
    fuel_sigma_f.push_back(1.0/16.0);
    std::vector <double> fuel_chi;
    fuel_chi.push_back(1.0);
    fuel_chi.push_back(0.0);
    std::vector <double> fuel_sigma_t;
    fuel_sigma_t.push_back(2.0/9.0);
    fuel_sigma_t.push_back(5.0/6.0);

    // fuel sigma_s
    static const double a_fuel_sigma_s [num_groups*num_groups] =
    { 71.0/360.0,   .02,
        0.0,        11.0/15.0 };
    std::vector <std::vector <double> > fuel_sigma_s (num_groups, 
            (std::vector <double> (num_groups)));
    for (int g=0; g<num_groups; ++g) {
        for (int gp=0; gp<num_groups; ++gp) {
            fuel_sigma_s[g][gp] = a_fuel_sigma_s[g*num_groups+gp];
        }
    }

    // water cross sections
    std::vector <double> water_sigma_f;
    water_sigma_f.push_back(0.0);
    water_sigma_f.push_back(0.0);
    std::vector <double> water_chi;
    water_chi.push_back(0.0);
    water_chi.push_back(0.0);
    std::vector <double> water_sigma_t;
    water_sigma_t.push_back(2.0/9.0);
    water_sigma_t.push_back(5.0/3.0);

    // water sigma_s
    static const double a_water_sigma_s [num_groups*num_groups] =
    { 71.0/360.0,   .025,
        0.0,        47.0/30.0 };
    std::vector <std::vector <double> > water_sigma_s (num_groups, 
            (std::vector <double> (num_groups)));
    for (int g=0; g<num_groups; ++g) {
        for (int gp=0; gp<num_groups; ++gp) {
            water_sigma_s[g][gp] = a_water_sigma_s[g*num_groups+gp];
        }
    }

    // nu
    double nu = 2.4;
    
    // create materials
    Material fuel(fuel_sigma_t, fuel_sigma_s, nu, fuel_sigma_f, fuel_chi);
    Material water(water_sigma_t, water_sigma_s, nu, water_sigma_f, water_chi);

    // create mesh
    Mesh test_mesh(test_boundary, 4.0/9.0, 4.0/9.0, 4.0/9.0, water, num_groups);

    // fill mesh with some material
    static const double a_fuel_limits [6] =
    { -2.0/3.0,     2.0/3.0,
      -2.0/3.0,     2.0/3.0,
      -2.0,         2.0 };
    std::vector <std::vector <double> > fuel_limits (3,
            (std::vector <double> (2)));
    for (int i=0; i<3; ++i) {
        for (int j=0; j<2; ++j) {
            fuel_limits[i][j] = a_fuel_limits[i*2+j];
        }
    }
    test_mesh.fillMaterials(fuel, fuel_limits);

    // get neutron histories
    generateNeutronHistories(100000, test_boundary, test_mesh, 10);

    

    // plot neutron flux
    std::vector <std::vector <std::vector <std::vector <double> > > > flux =
        test_mesh.getFlux();
    plotFlux(flux);

    std::cout << std::endl;
    return 0;
}
