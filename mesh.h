/** 
 @file      mesh.h
 @brief     contains Mesh class
 @author    Luke Eure
 @date      January 9 2016
*/

#ifndef MESH_H
#define MESH_H

#include "material.h"
#include "boundaries.h"

class Mesh {
private:
    double _delta_axes [3];
    double _boundary_mins [3];
    int _axis_sizes [3];
    double ****_flux;
    Material ***_cell_materials;
    int _cell_num_vector [3];
    int _cell_num;
    bool _move_cell;
    double *_maxes;
    double *_mins;
    double _min_locations [3];
    double _max_locations [3];
    int *_smallest_cell;
    int *_largest_cell;
    double _default_direction [3];


public:
    Mesh(Boundaries bounds, double delta_x, double delta_y, double delta_z,
            Material _default_material);
    virtual ~Mesh();

    int * getCell(double position[3], double direction[3]);
    void fluxAdd(int *cell, double distance, int group);
    void fluxClear();
    double**** getFlux();
    double* getCellMax(int *cell_number);
    double *getCellMin(int *cell_number);
    Material getMaterial(int *cell_number);
    void fillMaterials(Material material_type, double material_bounds[3][2]);

};

#endif
