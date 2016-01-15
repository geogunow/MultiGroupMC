/** 
 @file      monte_carlo.cpp
 @brief     Utility functions for monte carlo neutron simulation
 @author    Luke Eure
 @date      January 12 2016
*/

#include "monte_carlo.h"
#include <iostream>

/*
 @brief     Generates and transports neutron histories, calculates the mean
            crow distance
 @param     n_histories number of neutron histories to run
 @param     mat a Material object containing information
            about the material
 @param     bounds a Boundaries object containing the limits of the
            bounding box
 @param     mesh a Mesh object containing information about the mesh
 @param     num_batches the number of batches to be tested
*/
void generateNeutronHistories(int n_histories, Boundaries bounds,
        Mesh mesh, int num_batches) {
    
    /** create arrays for tallies and fissions */
    std::vector <Tally> tallies;
    std::vector <Fission> fission_banks;
    tallies.resize(5);
    fission_banks.resize(2);
    
    bool first_round = true;

    for (int batch=1; batch <= num_batches; ++batch) {
        
        /** clear flux data */
        mesh.fluxClear();

        /** assign new fission locations to old fission locations */
        fission_banks[OLD].clear();
        for (int i=0; i<fission_banks[OLD].length(); ++i) {
            std::vector <double> _add_location = fission_banks[NEW].next();    
            fission_banks[OLD].add(_add_location);
        }
        fission_banks[NEW].clear();

        /** clear tallies for leaks absorptions and fissions */
        tallies[LEAKS].clear();
        tallies[ABSORPTIONS].clear();
        tallies[FISSIONS].clear();

        /** simulate neutron behavior */
        for (int i=0; i<n_histories; ++i) {
            transportNeutron(bounds, tallies, fission_banks, first_round,
                    mesh, batch);
        }

        /** give results */
    }

}

/**
 @brief     Function that generates a neutron and measures how 
            far it travels before being absorbed.
 @details   A neutron is created in the bounding box using
            sample_location() for the first batch and sample_fission_site()
            for the rest of the batches. It moves a distance determined by
            sample_distance(). It is then either absorbed or
            scattered as determined by sample_interaction(). When
            it is absorbed, its distance from its starting point
            is appended to crow_distances. If the absorption creates
            a fission event, the number of neutrons emited is sampled.
            The location of the fission event is added to a list of fission
            events.
 @param	    mat a Material object containing information about the material
 @param     bounds a Boundaries object containing the limits
            of the bounding box
 @param     tallies a dictionary containing tallies of crow distances,
            leakages, absorptions, and fissions
 @param     fission_banks a dictionary containing the old and new fission banks
 @param     first_round a boolean telling whenther or not this is the first
            batch to be tested
 @param     mesh a Mesh object containing information about the mesh
 @param     batch the batch number of the neutron
*/
void transportNeutron(Boundaries bounds, std::vector <Tally> &tallies,
        std::vector <Fission> &fission_banks, bool first_round, Mesh mesh,
        int batch) {
    
    /** declare variables */
    const double TINY_MOVE = 1e-10;
    std::vector <double> neutron_starting_point;
    std::vector <int> cell;
    bool neutron_lost = false;
    double theta, phi, neutron_distance;
    Material cell_mat;
    int group;

    /** get neutron starting poinit */
    if (first_round) {
        neutron_starting_point = sampleLocation(bounds);
    }
    else {
        neutron_starting_point = sampleFissionSite(fission_banks[OLD]);
    }
 
    /** initialize neutron */
    theta = samplePolarAngle();
    phi = sampleAzimuthalAngle();
    Neutron neutron(neutron_starting_point, theta, phi);
     
    /** get mesh cell */
     
    std::vector <double> neutron_direction = neutron.getDirectionVector();

    cell = mesh.getCell(neutron_starting_point,
            neutron_direction);
    neutron.setCell(cell);

    /** set neutron group */
    std::cout <<batch << std::endl;
    cell_mat = mesh.getMaterial(cell);
    group = sampleNeutronEnergyGroup(cell_mat.getChi());
    neutron.setGroup(group);
    
    /** follow neutron while it's alive */
    while (neutron.alive()) {
        cell = neutron.getCell();
        cell_mat = mesh.getMaterial(cell);
        group = neutron.getGroup();
        neutron_distance = sampleDistance(cell_mat, group);

        /** track neutron until collision or leakage */
/*        while (neutron_distance > 0) {
            cell = mesh.getCell(neutron.getPositionVector(),
                    neutron.getDirectionVector());
            neutron.setCell(cell);
        }
*/
        /** get cell boundaries */
        

   }

}
