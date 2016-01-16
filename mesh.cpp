/** 
 @file      mesh.cpp
 @brief     contains functions for the Mesh class
 @author    Luke Eure
 @date      January 9 2016
*/

#include "mesh.h"

/**
 @brief constructor for Mesh class
*/
Mesh::Mesh(Boundaries bounds, double delta_x, double delta_y, double delta_z,
        Material default_material) {
    
    /** save deltas */
    _delta_axes.push_back(delta_x);
    _delta_axes.push_back(delta_y);
    _delta_axes.push_back(delta_z);

    /** save boundary mins */
    for (int i=0; i<3; ++i) {
        _boundary_mins.push_back(bounds.getSurfaceCoord(i, 0));
    }

    /** save axis sizes */
    for (int i=0; i<3; ++i) {
        _axis_sizes.push_back((int)((bounds.getSurfaceCoord(i, 1)
                    - _boundary_mins[i])/_delta_axes[i]));
    }
    
    /** resize _flux and set all its elements = 0 */
    _flux.resize(NUM_GROUPS);
    for (int i=0; i<NUM_GROUPS; ++i) {
        _flux[i].resize(_axis_sizes[0]);
        for (int j=0; j<_axis_sizes[0]; ++j) {
            _flux[i][j].resize(_axis_sizes[1]);
            for (int k=0; k<_axis_sizes[1]; ++k) {
                for (int l=0; l<_axis_sizes[2]; ++l) {
                    _flux[i][j][k].push_back(0.0);
                }
            }
        }
    }

    /** create materials array */
    _cell_materials.resize(_axis_sizes[0]);
    for (int i=0; i<_axis_sizes[0]; ++i) {
        _cell_materials[i].resize(_axis_sizes[1]);
        for (int j=0; j<_axis_sizes[1]; ++j) {
            for (int k=0; k<_axis_sizes[2]; ++k) {
                _cell_materials[i][j].push_back(default_material);
            }
        }
    }
}

/**
 @brief deconstructor for Mesh class
*/
Mesh::~Mesh() {}

/**
 @brief get the cell containing a neutron at a given location witha given
        direction of travel
*/
std::vector <int> Mesh::getCell(std::vector <double>& position,
        std::vector <double>& direction) {

    for (int i=0; i<3; ++i) {
        _cell_num = (int)((position[i] - _boundary_mins[i])/_delta_axes[i]);

        /** correct error if neutron is on upper boundary of cell */
        _move_cell = position[i] == _boundary_mins[i] + _cell_num
            * _delta_axes[i] & direction[i] <0;
        if (_cell_num == _axis_sizes[i] | _move_cell) {
            _cell_num -= 1;
        }
        
        _cell_num_vector.push_back(_cell_num);
    }

    return _cell_num_vector;
}

/**
 @brief add the distance a neutron has traveled within the cell to the flux
        array
*/
void Mesh::fluxAdd(std::vector <int> &cell, double distance, int group) {
    _flux[group][cell[0]][cell[1]][cell[2]] += distance;
}

/**
 @brief clear the flux
*/
void Mesh::fluxClear() {

    for (int i=0; i<NUM_GROUPS; ++i) {
        for (int j=0; j<_axis_sizes[0]; ++j) {
            for (int k=0; k<_axis_sizes[1]; ++k) {
                for (int l=0; l<_axis_sizes[2]; ++l) {
                    _flux[i][j][k].push_back(0.0);
                }
            }
        }
    }
}

/**
 @brief return the flux array
*/
std::vector <std::vector <std::vector <std::vector <double> > > >
        Mesh::getFlux() {

    /** _flux might have to be public for this to work */
    return _flux;
}

/**
 @brief returns the coordinate for the maximum in the cell
*/
std::vector <double> Mesh::getCellMax(std::vector <int> &cell_number) {
    
    /** maxes might have to be public as well */
    _maxes.resize(3);
    for (int i=0; i<3; ++i) {
        _maxes[i] = (cell_number[i] + 1) * _delta_axes[i] + _boundary_mins[i];
    }
    return _maxes;
}

/**
 @brief returns the coordinate for the minimum in the cell
*/
std::vector <double> Mesh::getCellMin(std::vector <int> &cell_number) {

    /** mins might have to be public */
    _mins.resize(3);
    for (int i=0; i<3; ++i) {
        _mins[i] = (cell_number[i]) * _delta_axes[i] + _boundary_mins[i];
    }
    return _mins;
}

/**
 @brief returns the material of a given cell
*/
Material Mesh::getMaterial(std::vector <int> &cell_number) {
    return _cell_materials[cell_number[0]][cell_number[1]][cell_number[2]];
}

/**
 @brief fill cells with a certain material
 @param locations should b be a 3x2 array with the maxes and mins of the area
        to be filled in each direction
*/
void Mesh::fillMaterials(Material material_type,
        std::vector <std::vector <double> > &material_bounds) {
    
    /** copy the value of material_bounds to locations and nudge the upper
    limit down so it is contained within the highest cell */
    _min_locations.resize(3);
    _max_locations.resize(3);
    _default_direction.resize(3);
    for (int i=0; i<3; ++i) {
        _min_locations[i] = material_bounds[i][0];
        _max_locations[i] = material_bounds[i][1] - _delta_axes[i]/10;
    
        /** put the smallest and largest cell values along each
            axis in a list */
        _default_direction[i] = 0.0;
    }

    _smallest_cell = getCell(_min_locations, _default_direction);
    _largest_cell = getCell(_max_locations, _default_direction);

    /** fill the cells with material_type */
    for (int i=_smallest_cell[0]; i<_largest_cell[0]; ++i){
        for (int j=_smallest_cell[1]; j<_largest_cell[1]; ++j){
            for (int k=_smallest_cell[2]; k<_largest_cell[2]; ++i){
                _cell_materials[i][j][k] = material_type;
            }
        }
    }
}
