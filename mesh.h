/** 
 @file      mesh.h
 @brief     contains Mesh class
 @author    Luke Eure
 @date      January 9 2016
*/

#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>
#include "material.h"
#include "boundaries.h"

class Mesh {
private:
    std::vector <double> _delta_axes,
        _boundary_mins,
        _maxes, _mins,
        _min_locations,
        _max_locations,
        _default_direction;
    std::vector <int> _axis_sizes,
        _cell_num_vector,
        _smallest_cell,
        _largest_cell;
    std::vector <std::vector <std::vector <std::vector <double> > > > _flux;
    std::vector <std::vector <std::vector <Material> > > _cell_materials;
    int _cell_num,
        _num_groups;
    bool _move_cell;

public:
    Mesh(Boundaries bounds, double delta_x, double delta_y, double delta_z,
            Material default_material, int num_groups);
    virtual ~Mesh();

    std::vector <int> getCell(std::vector <double>& position,
            std::vector <double>& direction);
    void fluxAdd(std::vector <int> &cell, double distance, int group);
    void fluxClear();
    std::vector <std::vector <std::vector <std::vector <double> > > > getFlux();
    std::vector <double> getCellMax(std::vector <int> &cell_number);
    std::vector <double> getCellMin(std::vector <int> &cell_number);
    Material getMaterial(std::vector <int> &cell_number);
    void fillMaterials(Material material_type,
            std::vector <std::vector <double> > &material_bounds);

};

#endif
