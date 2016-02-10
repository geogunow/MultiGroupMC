/* 
 @file      Main.cpp
 @brief     creates geometry and materials to run a Monte Carlo simulation
 @author    Luke Eure
 @date      January 9 2016
*/

#include <iostream>
#include <vector>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#include "Material.h"
#include "Boundaries.h"
#include "Distributions.h"
#include "Tally.h"
#include "Mesh.h"
#include "Monte_carlo.h"
#include "Plotter.h"
#include "Neutron.h"

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
    std::vector <double> fuel_sigma_f (num_groups);
    fuel_sigma_f[0] = 1.0/480.0;
    fuel_sigma_f[1] = 1.0/16.0;
    std::vector <double> fuel_chi(num_groups);
    fuel_chi[0] = 1.0;
    fuel_chi[1] = 0.0;
    std::vector <double> fuel_sigma_t(num_groups);
    fuel_sigma_t[0] = 2.0/9.0;
    fuel_sigma_t[1] = 5.0/6.0;

    // create vector fuel sigma_s by first making an array
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
    std::vector <double> water_sigma_f(num_groups);
    water_sigma_f[0] = 0.0;
    water_sigma_f[1] = 0.0;
    std::vector <double> water_chi(num_groups);
    water_chi[0] = 0.0;
    water_chi[1] = 0.0;
    std::vector <double> water_sigma_t(num_groups);
    water_sigma_t[0] = 2.0/9.0;
    water_sigma_t[1] = 5.0/3.0;

    // create vector water sigma_s by first making an array
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

    // nu: the average number of neutrons released per fission event
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

    // simulate neutron histories
    int num_neutrons = 10000;
    int num_batches = 2;
    generateNeutronHistories(num_neutrons, test_boundary,
            test_mesh, num_batches, num_groups);

    // plot neutron flux
    std::vector <std::vector <std::vector <std::vector <double> > > > flux =
        test_mesh.getFlux();
    printFluxToFile(flux);

    // run python script to get flux plots
    system("python Flux_parser.py");

    std::cout << std::endl;
    return 0;
}
