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
        Material _default_material) {
    
    /** save deltas */
    _delta_axes[0] = delta_x;
    _delta_axes[1] = delta_y;
    _delta_axes[2] = delta_z;

    /** save boundary mins */
    for (int i=0; i<3; ++i) {
        _boundary_mins[i]=  bounds.getSurfaceCoord(i, 0);
    }

    /** save axis sizes */
    for (int i=0; i<3; ++i) {
        _axis_sizes[i] = (int)((bounds.getSurfaceCoord(i, 1)
                    - _boundary_mins[i])/_delta_axes[i]);
    }

    /** creat flux array */
    double _flux[NUM_GROUPS][_axis_sizes[0]][_axis_sizes[1]][_axis_sizes[2]];
   
    // set flux = 0 
    for (int i=0; i<NUM_GROUPS; ++i) {
        for (int j=0; j<_axis_sizes[0]; ++j) {
            for (int k=0; k<_axis_sizes[1]; ++k) {
                for (int ii=0; ii<_axis_sizes[2]; ++ii) {
                    std::cout << i << " " << j << " " << k << " " << ii << std::endl;
                    _flux[i][j][k][ii] = 0.0;
                    std::cout << "flux value: " << _flux[i][j][k][ii];
                }
            }
        }
    }

    
    /** creat materials array */
    
    Material _cell_materials[_axis_sizes[0]][_axis_sizes[1]][_axis_sizes[2]];
    for (int i=0; i<_axis_sizes[0]; ++i) {
        for (int j=0; j<_axis_sizes[1]; ++j) {
            for (int k=0; k<_axis_sizes[2]; ++k) {
                _cell_materials[i][j][k] = _default_material;
    
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
int * Mesh::getCell(double position[3], double direction[3]) {

/** debugging */


    for (int i=0; i<3; ++i) {
        _cell_num = (int)((position[i] - _boundary_mins[i])/_delta_axes[i]);
        std::cout << "working "  << _delta_axes[i] << std::endl;

        /** correct error if neutron is on upper boundary of cell */
        _move_cell = position[i] == _boundary_mins[i] + _cell_num
            * _delta_axes[i] & direction[i] <0;
        if (_cell_num == _axis_sizes[i] | _move_cell) {
            _cell_num -= 1;
        }
        
        _cell_num_vector[i] = _cell_num;
    }

    std::cout << "cell:" << std::endl;
    for (int i=0; i<3; ++i) {
        std::cout << _cell_num_vector[i] << std::endl;
    }
    return _cell_num_vector;
}

/**
 @brief add the distance a neutron has traveled within the cell to the flux
        array
*/
void Mesh::fluxAdd(int *cell, double distance, int group) {
    _flux[group][cell[0]][cell[1]][cell[2]] += distance;
}

/**
 @brief clear the flux
*/
void Mesh::fluxClear() {

    /** this might not work */
    std::cout << "clearing flux" << std::endl;
    for (int i=0; i<NUM_GROUPS; ++i) {
        for (int j=0; j<_axis_sizes[0]; ++j) {
            for (int k=0; k<_axis_sizes[1]; ++k) {
                for (int ii=0; ii<_axis_sizes[2]; ++ii) {
                    std::cout << i << " " << j << " " << k << " " << ii << std::endl;
                    //_flux[i][j][k][ii] = 0.0;
                    std::cout << "flux value: " << _flux[0][0][0][0];
                    std::cout << "flux value: " << _flux[i][j][k][ii];
                }
            }
        }
    }
}

/**
 @brief return the flux array
*/
double**** Mesh::getFlux() {

    /** _flux might have to be public for this to work */
    return _flux;
}

/**
 @brief returns the coordinate for the maximum in the cell
*/
double* Mesh::getCellMax(int cell_number [3]) {
    
    /** maxes might have to be public as well */
    for (int i=0; i<3; ++i) {
        _maxes[i] = (cell_number[i] + 1) * _delta_axes[i] + _boundary_mins[i];
    }
    return _maxes;
}

/**
 @brief returns the coordinate for the minimum in the cell
*/
double* Mesh::getCellMin(int cell_number [3]) {

    /** mins might have to be public */
    for (int i=0; i<3; ++i) {
        _mins[i] = (cell_number[i]) * _delta_axes[i] + _boundary_mins[i];
    }
    return _mins;
}

/**
 @brief returns the material of a given cell
*/
Material Mesh::getMaterial(int cell_number [3]) {
    return _cell_materials[cell_number[0]][cell_number[1]][cell_number[2]];
}

/**
 @brief fill cells with a certain material
 @param locations should b be a 3x2 array with the maxes and mins of the area
        to be filled in each direction
*/
void Mesh::fillMaterials(Material material_type, double material_bounds[3][2]) {
    
   
    /** copy the value of material_bounds to locations and nudge the upper
     limit down so it is contained within the highest cell */
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
