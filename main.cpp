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
    test_boundary.setSurfaceType(0, 0, REFLECTIVE);
    test_boundary.setSurfaceType(0, 1, REFLECTIVE);
    test_boundary.setSurfaceType(1, 0, REFLECTIVE);
    test_boundary.setSurfaceType(1, 1, REFLECTIVE);
    test_boundary.setSurfaceType(2, 0, REFLECTIVE);
    test_boundary.setSurfaceType(2, 1, REFLECTIVE);

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

    // nu
    double nu = 2.4;
    
    // create materials
    Material fuel(fuel_sigma_t, fuel_sigma_s, nu, fuel_sigma_f, fuel_chi);

    // create mesh
    Mesh test_mesh(test_boundary, 4.0/9.0, 4.0/9.0, 4.0/9.0, fuel, num_groups);
    
    // debugging
    std::vector <double> position;
    position.push_back(.678);
    position.push_back(.976);
    position.push_back(1.111111111111111);  // With 16 decimal places, the cell
                                            // is 6 6 7. With less than 16, the
                                            // cell is 6 6 6.
    std::vector <double> direction;
    direction.push_back(.226);
    direction.push_back(-.116);
    direction.push_back(.967);
    std::vector <int> cell;
    cell = test_mesh.getCell(position, direction);
    std::cout << "testing cell: " << cell[0] << " " << cell[1]
        << " " << cell[2] << std::endl;

/*    
    // get neutron histories
    generateNeutronHistories(4, test_boundary, test_mesh, 1);

    // plot neutron flux
    std::vector <std::vector <std::vector <std::vector <double> > > > flux =
        test_mesh.getFlux();
    plotFlux(flux);

    std::cout << std::endl;
*/    return 0;
}
