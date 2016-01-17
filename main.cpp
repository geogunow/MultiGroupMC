#include <iostream>
#include <vector>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#include "material.h"
#include "global_constants.h"
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

    /** fuel cross sections */
    static const double a_fuel_sigma_t [NUM_GROUPS] = {2.0/9.0, 5.0/6.0};
    static const double a_fuel_sigma_f [NUM_GROUPS] = {1.0/480.0, 1.0/16.0};
    static const double a_fuel_chi [NUM_GROUPS] = {1.0,0.0};
    std::vector <double> fuel_sigma_t (a_fuel_sigma_t,
            a_fuel_sigma_t + sizeof(a_fuel_sigma_t)
            / sizeof(a_fuel_sigma_t[0]));
    std::vector <double> fuel_sigma_f (a_fuel_sigma_f,
            a_fuel_sigma_f + sizeof(a_fuel_sigma_f)
            / sizeof(a_fuel_sigma_f[0]));
    std::vector <double> fuel_chi (a_fuel_chi,
            a_fuel_chi + sizeof(a_fuel_chi)
            / sizeof(a_fuel_chi[0]));
    
    /** fuel sigma_s */
    std::vector <std::vector <double> > fuel_sigma_s;
    std::vector <double> s_row1;
    std::vector <double> s_row2;
    s_row1.push_back(71.0/360.0);
    s_row1.push_back(.02);
    s_row2.push_back(0.0);
    s_row2.push_back(11.0/15.0);
    fuel_sigma_s.push_back(s_row1);
    fuel_sigma_s.push_back(s_row2);

    /** water cross sections */
    static const double a_water_sigma_t [NUM_GROUPS] = {2.0/9.0, 5.0/3.0};
    static const double a_water_sigma_f [NUM_GROUPS] = {0.0, 0.0};
    static const double a_water_chi [NUM_GROUPS] = {0.0,0.0};
    std::vector <double> water_sigma_t (a_water_sigma_t,
            a_water_sigma_t + sizeof(a_water_sigma_t)
            / sizeof(a_water_sigma_t[0]));
    std::vector <double> water_sigma_f (a_water_sigma_f,
            a_water_sigma_f + sizeof(a_water_sigma_f)
            / sizeof(a_water_sigma_f[0]));
    std::vector <double> water_chi (a_water_chi,
            a_water_chi + sizeof(a_water_chi)
            / sizeof(a_water_chi[0]));
    
    /** water sigma_s */
    std::vector <std::vector <double> > water_sigma_s;
    std::vector <double> w_s_row1;
    std::vector <double> w_s_row2;
    w_s_row1.push_back(71.0/360.0);
    w_s_row1.push_back(.025);
    w_s_row2.push_back(0.0);
    w_s_row2.push_back(47.0/30.0);
    water_sigma_s.push_back(w_s_row1);
    water_sigma_s.push_back(w_s_row2);
    
    /** nu */
    double nu = 2.4;
    
    /** create materials */
    Material fuel(fuel_sigma_t, fuel_sigma_s, nu, fuel_sigma_f, fuel_chi);
    Material water(water_sigma_t, water_sigma_s, nu, water_sigma_f, water_chi);

    /* create mesh */
    Mesh test_mesh(test_boundary, 4.0/9.0, 4.0/9.0, 4.0/9.0, water);

    /** fill mesh with some fuel */ 
    std::vector <std::vector <double > > fuel_limits;
    fuel_limits.resize(3);
    for (int i=0; i<3; ++i) {
        fuel_limits[i].resize(2);
    }

    fuel_limits[0][0] = -2.0/3.0;
    fuel_limits[0][1] = 2.0/3.0;
    fuel_limits[1][0] = -2/3.0;
    fuel_limits[1][1] = 2.0/3.0;
    fuel_limits[2][0] = -2.0;
    fuel_limits[2][1] = 2.0;
    test_mesh.fillMaterials(fuel, fuel_limits);
   
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
    
    generateNeutronHistories(1000, test_boundary, test_mesh, 10);

    std::cout << std::endl;
    return 0;
}
