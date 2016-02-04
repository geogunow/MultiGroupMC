/*
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

    /** the width of the cell along each axis */
    std::vector <double> _delta_axes;

    /** the minimum locations on the geometry in each direction */
    std::vector <double> _boundary_mins;

    /** cell max along each axis */
    std::vector <double> _maxes;

    /** cell min along each axis */
    std::vector <double> _mins;

    /** minimum location to be filled by a material type along each axis */ 
    std::vector <double> _min_locations;

    /** maximum location to be filled by a material type along each axis */ 
    std::vector <double> _max_locations;

    /** 0, 0, 0 direction vector */
    std::vector <double> _default_direction;

    /** the number of cells along each axis */
    std::vector <int> _axis_sizes;

    /** the cell number along each axis */
    std::vector <int> _cell_num_vector;

    /** largest cell to be filled with material */
    std::vector <int> _smallest_cell;

    /** smallest cell to be filled with material */
    std::vector <int> _largest_cell;

    /** the neutron flux through each cell */
    std::vector <std::vector <std::vector <std::vector <double> > > > _flux;
    
    /** materials of each cell */
    std::vector <std::vector <std::vector <Material> > > _cell_materials;

    /** a cell number */
    int _cell_num;

    /** the number of energy groups */
    int _num_groups;

    /** bool for use in corrections when a neutron is on a cell boundary */
    bool _move_cell;

public:
    Mesh(Boundaries bounds, double delta_x, double delta_y, double delta_z,
            Material default_material, int num_groups);
    virtual ~Mesh();

    void fluxAdd(std::vector <int> &cell, double distance, int group);
    void fluxClear();
    void fillMaterials(Material material_type,
            std::vector <std::vector <double> > &material_bounds);
    bool positionInBounds(std::vector <double> &position);
    std::vector <int> getCell(std::vector <double>& position,
            std::vector <double>& direction);
    std::vector <double> getCellMax(std::vector <int> &cell_number);
    std::vector <double> getCellMin(std::vector <int> &cell_number);
    std::vector <std::vector <std::vector <std::vector <double> > > > getFlux();
    Material* getMaterial(std::vector <int> &cell_number);
};

#endif
