/* 
 @file      boundaries.cpp
 @brief     contains functions for the Boundaries class
 @author    Luke Eure
 @date      January 9 2016
*/

#include "boundaries.h"

Boundaries::Boundaries() {

    for (int i=0; i<6; ++i) {
        _surface_coords[i] = 0;
        _surface_init[i] = false;
        _surface_type[i] = BOUNDARY_NONE;
    }
}

Boundaries::~Boundaries() {}

float Boundaries::getSurfaceCoord(int axis, int side) {
    return _surface_coords[axis*2+side];
}

float Boundaries::getSurfaceType(int axis, int side) {
    return _surface_type[axis*2+side];
}

void Boundaries::setSurfaceCoord(int axis, int side, float coord) {

    _surface_coords[axis*2+side] = coord;
    _surface_init[axis*2+side] = true;
}

void Boundaries::setSurfaceType(int axis, int side, BoundaryType type) {
    _surface_type[axis*2+side] = type;
}
