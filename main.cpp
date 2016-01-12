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

int main() {
    

    srand(time(NULL));

    /** create geometry */
  
    Boundaries test_boundary;
    test_boundary.setSurfaceCoord(0, 0, -1.0);
    test_boundary.setSurfaceCoord(0, 1, 1.0);
    test_boundary.setSurfaceCoord(1, 0, -1.0);
    test_boundary.setSurfaceCoord(1, 1, 1.0);
    test_boundary.setSurfaceCoord(2, 0, -1.0);
    test_boundary.setSurfaceCoord(2, 1, 1.0);

    /** fuel cross sections */
   
    double fuel_sigma_t [NUM_GROUPS] = {2.0/9.0, 5.0/6.0};
    
    /** sigma_s */
        double i [NUM_GROUPS] = {71.0/360.0, .02};
        double j [NUM_GROUPS] = {0.0, 11.0/15.0};
        double* fuel_sigma_s [NUM_GROUPS] ={i,j};
    double fuel_sigma_f [NUM_GROUPS] = {1.0/480.0, 1.0/16.0};
    double fuel_chi [NUM_GROUPS] = {1.0,0.0};

    /** water cross sections */
    double water_sigma_t [NUM_GROUPS] = {2/9, 5/3};
    double water_sigma_f [NUM_GROUPS] = {0, 0};
    
    /** sigma_s */
        double ii [NUM_GROUPS] = {71/360, .025};
        double jj [NUM_GROUPS] = {0, 47/30};
        double* water_sigma_s [NUM_GROUPS] ={ii,jj};
    double water_chi [NUM_GROUPS] = {0,0};

    /** nu */
    double nu = 2.4;

    /** create materials */
    Material fuel(fuel_sigma_t, fuel_sigma_s, nu, fuel_sigma_f, fuel_chi);
    Material water(water_sigma_t, water_sigma_s, nu, water_sigma_f, water_chi);

    /* create mesh */
    Mesh test_mesh(test_boundary, 4/9, 4/9, 4/9, water);

    /** fill mesh with some fuel */ 
    double fuel_limits[3][2];
    fuel_limits[0][0] = -2/3;
    fuel_limits[0][1] = 2/3;
    fuel_limits[1][0] = -2/3;
    fuel_limits[1][1] = 2/3;
    fuel_limits[2][0] = -2;
    fuel_limits[2][1] = 2;
    test_mesh.fillMaterials(fuel, fuel_limits);
    
    return 0;
}
