/*
 @file      mesh.cpp
 @brief     contains functions for the Mesh class
 @author    Luke Eure
 @date      January 9 2016
*/

#include "mesh.h"

/*
 @brief constructor for Mesh class
*/
Mesh::Mesh(Boundaries bounds, double delta_x, double delta_y, double delta_z,
        Material default_material, int num_groups) {
    
    // save deltas 
    _delta_axes.push_back(delta_x);
    _delta_axes.push_back(delta_y);
    _delta_axes.push_back(delta_z);

    // save number of groups
    _num_groups = num_groups;

    // save boundary mins
    for (int axis=0; axis<3; ++axis) {
        _boundary_mins.push_back(bounds.getSurfaceCoord(axis, 0));
    }

    // save axis sizes
    for (int axis=0; axis<3; ++axis) {
        int size = (bounds.getSurfaceCoord(axis, 1) - _boundary_mins[axis])
            / _delta_axes[axis];
        _axis_sizes.push_back(size);
    }
    
    // resize _flux and set all its elements = 0
    _flux.resize(_num_groups);
    for (int i=0; i<_num_groups; ++i) {
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

    // create materials array
    _cell_materials.resize(_axis_sizes[0]);
    for (int i=0; i<_axis_sizes[0]; ++i) {
        _cell_materials[i].resize(_axis_sizes[1]);
        for (int j=0; j<_axis_sizes[1]; ++j) {
            for (int k=0; k<_axis_sizes[2]; ++k) {
                _cell_materials[i][j].push_back(default_material);
            }
        }
    }
    
    // resize vectors
    _maxes.resize(3);
    _mins.resize(3);
    _min_locations.resize(3);
    _max_locations.resize(3);
    _default_direction.resize(3);
    
}

/*
 @brief deconstructor for Mesh class
*/
Mesh::~Mesh() {}

/*
 @brief get the cell containing a neutron at a given location witha given
        direction of travel
 @param position a vector containing the location to find the cell of
 @param direction the direction the nuetron is travelling
 @return a vector denoting the cell of the location and direction
*/
std::vector <int> Mesh::getCell(std::vector <double>& position,
        std::vector <double>& direction) {
    _cell_num_vector.clear();
    for (int i=0; i<3; ++i) {
        _cell_num = (int)((position[i] - _boundary_mins[i])/_delta_axes[i]);
        
        // correct error if neutron is on upper boundary of cell
        // the rounding is neaded because decimal accuracy gets off
        _move_cell = (roundf(position[i]*1e5)/1e5 
            == roundf((_boundary_mins[i] + _cell_num
            * _delta_axes[i]) *1e5)/1e5 & direction[i] <0);
        if (_cell_num == _axis_sizes[i] | _move_cell) {
            _cell_num -= 1;
        }
        if (_cell_num == -1 ) {
            _cell_num =0 ;
        }
        _cell_num_vector.push_back(_cell_num);
    }

    return _cell_num_vector;
}

/*
 @brief add the distance a neutron has traveled within the cell to the flux
        array
 @param cell a vector containing a cell
 @param distance a distance to be added to the cell flux
 @param group a group to which this distance should be added
*/
void Mesh::fluxAdd(std::vector <int> &cell, double distance, int group) {
    _flux[group][cell[0]][cell[1]][cell[2]] += distance;
}

/*
 @brief clear the flux
*/
void Mesh::fluxClear() {

    for (int i=0; i<_num_groups; ++i) {
        for (int j=0; j<_axis_sizes[0]; ++j) {
            for (int k=0; k<_axis_sizes[1]; ++k) {
                for (int l=0; l<_axis_sizes[2]; ++l) {
                    _flux[i][j][k][l] = 0.0;
                }
            }
        }
    }
}

/*
 @brief return the flux array
 @return returns the 4d flux vector
*/

std::vector <std::vector <std::vector <std::vector <double> > > > 
        Mesh::getFlux() {
    return _flux;
}

/*
 @brief returns the coordinate for the maximum in the cell
 @param cell_cell number vector containing the number of a cell to find the 
        max of
 @return a vector containing the maximum location of that cell in each dimension
*/
std::vector <double> Mesh::getCellMax(std::vector <int> &cell_number) {
    
    for (int i=0; i<3; ++i) {
        _maxes[i] = (cell_number[i] + 1) * _delta_axes[i] + _boundary_mins[i];
    }
    return _maxes;
}

/*
 @brief returns the coordinate for the minimum in the cell
 @param cell_cell number vector containing the number of a cell to find the 
        min of
 @return a vector containing the minimum location of that cell in each dimension
*/
std::vector <double> Mesh::getCellMin(std::vector <int> &cell_number) {

    for (int i=0; i<3; ++i) {
        _mins[i] = (cell_number[i]) * _delta_axes[i] + _boundary_mins[i];
    }
    return _mins;
}

/*
 @brief returns the material of a given cell
 @param cell_number vector containing the number of a cell to find the 
        material of
 @return the material of the cell
*/
Material Mesh::getMaterial(std::vector <int> &cell_number) {
    return _cell_materials[cell_number[0]][cell_number[1]][cell_number[2]];
}

/*
 @brief fill cells with a certain material
 @param material_type a material to fill the mesh with
 @param locations should be a 3x2 vector with the maxes and mins of the area
        to be filled in each direction
*/
void Mesh::fillMaterials(Material material_type,
        std::vector <std::vector <double> > &material_bounds) {
    
    // copy the value of material_bounds to locations and nudge the upper
    // limit down so it is contained within the highest cell
    for (int i=0; i<3; ++i) {
        _min_locations[i] = material_bounds[i][0];
        _max_locations[i] = material_bounds[i][1] - _delta_axes[i]/10;
    
        // put the smallest and largest cell values along each
        // axis in a list
        _default_direction[i] = 0.0;
    }

    _smallest_cell = getCell(_min_locations, _default_direction);
    _largest_cell = getCell(_max_locations, _default_direction);
    
    // fill the cells with material_type
    for (int i=_smallest_cell[0]; i<=_largest_cell[0]; ++i) {
        for (int j=_smallest_cell[1]; j<=_largest_cell[1]; ++j) {
            for (int k=_smallest_cell[2]; k<=_largest_cell[2]; ++k) {
                _cell_materials[i][j][k] = material_type;
            }
        }
    }
}

/*
 @brief returns a boolean denoting whether or not a given position is within
        the geometry
 @param position a cartesian coordinate denoting a position in the geometry
*/
bool Mesh::positionInBounds(std::vector <double> &position) {
    for (int axis=0; axis<3; ++axis) {
        double _boundary_max = _boundary_mins[axis]
            + _delta_axes[axis] * _axis_sizes[axis];

        // check the boundaries
        if (position[axis] < _boundary_mins[axis]
                | position[axis] > _boundary_max) {
            return false;
        }
    }

    return true;

}
