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
#include <math.h>

class Mesh {
private:
    std::vector <double> _delta_axes;
    std::vector <double> _boundary_mins;
    std::vector <double> _maxes;
    std::vector <double> _mins;
    std::vector <double> _min_locations;
    std::vector <double> _max_locations;
    std::vector <double> _default_direction;
    std::vector <int> _axis_sizes;
    std::vector <int> _cell_num_vector;
    std::vector <int> _smallest_cell;
    std::vector <int> _largest_cell;
    std::vector <std::vector <std::vector <std::vector <double> > > > _flux;
    std::vector <std::vector <std::vector <Material> > > _cell_materials;
    int _cell_num;
    int _num_groups;
    bool _move_cell;

public:
    Mesh(Boundaries bounds, double delta_x, double delta_y, double delta_z,
            Material default_material, int num_groups);
    virtual ~Mesh();

    void fluxAdd(std::vector <int> &cell, double distance, int group);
    void fluxClear();
    void fillMaterials(Material material_type,
            std::vector <std::vector <double> > &material_bounds);
    std::vector <int> getCell(std::vector <double>& position,
            std::vector <double>& direction);
    std::vector <double> getCellMax(std::vector <int> &cell_number);
    std::vector <double> getCellMin(std::vector <int> &cell_number);
    std::vector <std::vector <std::vector <std::vector <double> > > > getFlux();
    Material getMaterial(std::vector <int> &cell_number);

};

#endif
