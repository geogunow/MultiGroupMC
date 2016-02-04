/*
 @file      monte_carlo.cpp
 @brief     Utility functions for monte carlo neutron simulation
 @author    Luke Eure
 @date      January 12 2016
*/

#include "monte_carlo.h"

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
        Mesh &mesh, int num_batches) {

    // create arrays for tallies and fissions
    std::vector <Tally> tallies(5);

    Fission* old_fission_bank = new Fission;
    Fission* new_fission_bank =  new Fission;
    Fission temp_fission_bank;
    
    bool first_round = true;

    for (int batch=1; batch <= num_batches; ++batch) {

        // clear flux data
        mesh.fluxClear();

        // assign new fission locations to old fission locations
        *old_fission_bank = *new_fission_bank;
        *new_fission_bank = temp_fission_bank;
        new_fission_bank->clear();

        // clear tallies for leaks absorptions and fissions
        tallies[LEAKS].clear();
        tallies[ABSORPTIONS].clear();
        tallies[FISSIONS].clear();

        // simulate neutron behavior
        for (int i=0; i<n_histories; ++i) {
            transportNeutron(bounds, tallies, first_round,
                    mesh, old_fission_bank, new_fission_bank);
        }

        // give results
        std::cout << "For batch " << batch << ", k = "
            << tallies[FISSIONS].getCount()/(tallies[LEAKS].getCount() +
                    tallies[ABSORPTIONS].getCount()) << std::endl;
        first_round = false;
    }
    double mean_crow_distance = tallies[CROWS].getCount()
        / tallies[NUM_CROWS].getCount();
    std::cout << "Mean crow fly distance = " << mean_crow_distance;

    delete old_fission_bank;
    delete new_fission_bank;
}

/*
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
*/
void transportNeutron(Boundaries bounds, std::vector <Tally> &tallies,
        bool first_round, Mesh &mesh,
        Fission *old_fission_bank, Fission *new_fission_bank) {
    
    // declare variables
    const double TINY_MOVE = 1e-10;
    double theta;
    double phi;
    double neutron_distance;
    double tempd;
    double r;
    double bound_val;
    double crow_distance;
    int group;
    int new_group;
    int neutron_interaction;
    std::vector <double> neutron_starting_point;
    std::vector <double> neutron_position;
    std::vector <double> neutron_direction;
    std::vector <double> cell_mins;
    std::vector <double> cell_maxes;
    std::vector <std::vector <double> > distance_to_cell_edge;
    std::vector <std::vector <double> > temp_sigma_s;
    std::vector <int> cell;
    std::vector <int> cell_lim_bound;
    std::vector <int> box_lim_bound;
    bool neutron_lost = false;
    Material cell_mat;

    // get neutron starting poinit
    if (first_round) {
        neutron_starting_point = sampleLocation(bounds);
    }
    else {
        neutron_starting_point = sampleFissionSite(old_fission_bank);
    }

    // initialize neutron
    theta = samplePolarAngle();
    phi = sampleAzimuthalAngle();
    Neutron neutron(neutron_starting_point, theta, phi);
     
    // get mesh cell
    neutron_direction = neutron.getDirectionVector();
    cell = mesh.getCell(neutron_starting_point, neutron_direction);
    neutron.setCell(cell);

    // set neutron group
    cell_mat = mesh.getMaterial(cell);
    group = sampleNeutronEnergyGroup(cell_mat.getChi());
    neutron.setGroup(group);
    
    // follow neutron while it's alive
    while (neutron.alive()) {

        cell_mat = mesh.getMaterial(cell);
        group = neutron.getGroup();
        neutron_distance = sampleDistance(cell_mat, group);
    
        // track neutron until collision or leakage
        while (neutron_distance > 0) {
            neutron_position = neutron.getPositionVector();

            // get cell boundaries
            cell_mins = mesh.getCellMin(cell);
            cell_maxes = mesh.getCellMax(cell);

            // calculate distances to cell boundaries
            distance_to_cell_edge.resize(3);
            for (int axis=0; axis<3; ++axis) {
                distance_to_cell_edge[axis].resize(2);
                distance_to_cell_edge[axis][0] =
                    cell_mins[axis] - neutron.getPosition(axis);
                distance_to_cell_edge[axis][1] =
                    cell_maxes[axis] - neutron.getPosition(axis);
            }

            // tempd contains the current smallest r
            tempd = neutron_distance;

            // clear lim_bounds
            cell_lim_bound.clear();
            box_lim_bound.clear();

            // test each boundary
            for (int axis=0; axis<3; ++axis) {
                for (int side=0; side<2; ++side) {
                    
                    // r is variable that contains the distance
                    // along the direction vector to the boundary being tested.
                    r = distance_to_cell_edge[axis][side]
                        / neutron.getDirection(axis);
                    if (r >0 & r < tempd) {
                        tempd = r;
                        cell_lim_bound.clear();
                        cell_lim_bound.push_back(axis*2+side);
                    }
                    else if (r == tempd) {
                        cell_lim_bound.push_back(axis*2+side);
                    }
                }
            }

            // move neutron
            neutron.move(tempd);
            
            // add distance to cell flux
            group = neutron.getGroup();
            mesh.fluxAdd(cell, tempd, group);
           
            // determine boundary status
            for (int sur_side=0; sur_side <6; ++sur_side) {
                int axis = sur_side/2;
                int side = sur_side%2;

                // if sur_side is in cell_lim_bound
                if (std::find(cell_lim_bound.begin(),
                            cell_lim_bound.end(),sur_side)
                        != cell_lim_bound.end()) {
                    if (cell_mins[axis] == bounds.getSurfaceCoord(axis, side)
                            | cell_maxes[axis] == 
                            bounds.getSurfaceCoord(axis, side)) {
                        box_lim_bound.push_back(sur_side);
                    }
                }
            }

            // check boundary conditions on all hit surfaces
            for (int sur_side=0; sur_side <6; ++sur_side) {
                int axis = sur_side/2;
                int side = sur_side%2;
  
                // if sur_side is in box_lim_bound
                if (std::find(box_lim_bound.begin(),
                            box_lim_bound.end(),sur_side)
                        != box_lim_bound.end()) {

                    // if the neutron is reflected
                    if (bounds.getSurfaceType(axis, side) == 1) {
                        neutron.reflect(axis);

                        // place neutron on boundary to eliminate 
                        //    roundoff error
                        bound_val = bounds.getSurfaceCoord(axis, side);
                        neutron.setPosition(axis, bound_val);
                    }

                    // if the neutron escapes
                    if (bounds.getSurfaceType(axis, side) == 0) {
                        neutron.kill();
                        neutron_distance = tempd;
                        tallies[LEAKS].add(1);
                    }
                }
            }
 
            // shorten neutron distance to collision
            neutron_distance -= tempd;

            // get new neutron cell
            if (neutron_distance != 0.0) {
                neutron_direction = neutron.getDirectionVector();
                cell = mesh.getCell(neutron_position, neutron_direction);

                // nudge neutron
                neutron.move(TINY_MOVE);
                neutron_position = neutron.getPositionVector();
                if (mesh.positionInBounds(neutron_position)) {
                    cell = mesh.getCell(neutron_position, neutron_direction);
                }
                neutron.move(-TINY_MOVE);
                neutron.setCell(cell);
            }
        }

        // check interaction
        if (neutron.alive()) {

            cell_mat = mesh.getMaterial(cell);
            group = neutron.getGroup();

            // sample what the interaction will be
            neutron_interaction = sampleInteraction(cell_mat, group);

            // scattering event
            if (neutron_interaction ==0) {

                // sample scattered direction
                theta = samplePolarAngle();
                phi = sampleAzimuthalAngle();

                // sample new energy group
                temp_sigma_s = cell_mat.getSigmaS();
                new_group = sampleScatteredGroup(temp_sigma_s, group);

                // set new group
                neutron.setGroup(new_group);

                // set new direction
                neutron.setDirection(theta, phi);
            }

            // absorption event
            else {

                // tally absorption
                tallies[ABSORPTIONS].add(1);

                // sample for fission event
                group = neutron.getGroup();
                cell = neutron.getCell();
                cell_mat = mesh.getMaterial(cell);
                neutron_position = neutron.getPositionVector();
                if (sampleFission(cell_mat, group) == 1) {

                    // sample number of neutrons
                    for (int i=0; i<sampleNumFission(cell_mat); ++i) {
                        new_fission_bank->add(neutron_position);
                        tallies[FISSIONS].add(1);
                    }
                }

                // end neutron history
                neutron.kill();
            }
        }
    }

    // tally crow distance
    crow_distance = neutron.getDistance(neutron_starting_point);
    tallies[CROWS].add(crow_distance);
    tallies[NUM_CROWS].add(1);
}
