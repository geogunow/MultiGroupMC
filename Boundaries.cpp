/* 
 @file      Boundaries.cpp
 @brief     contains functions for the Boundaries class
 @author    Luke Eure
 @date      January 9 2016
*/

#include "Boundaries.h"

/*
 @brief     constructor for Boundaries class
*/
Boundaries::Boundaries() {}

/*
 @brief     deconstructor
*/
Boundaries::~Boundaries() {}

/*
 @brief     set the surfaces into the geometry
 @paraam    axis 0, 1, or 2 corresponding to x y and z
 @param     side 0 or 1 corresponding to the minimum or maximum of the geometry
*/
void Boundaries::setSurface(Axes axis, min_max side, Surface* surface) {
    _surfaces[2*axis + side] = surface;
}

/*
 @brief     return the position of the surface
 @paraam    axis 0, 1, or 2 corresponding to x y and z
 @param     side 0 or 1 corresponding to the minimum or maximum of the geometry
 @return    the position of the surface within the geometry
*/
float Boundaries::getSurfaceCoord(int axis, int side) {
    return _surfaces[axis*2+side]->getPosition();
}

/*
 @brief     return the type of a surface
 @paraam    axis 0, 1, or 2 corresponding to x y and z
 @param     side 0 or 1 corresponding to the minimum or maximum of the geometry
 @return    the type of the surface, 0 = vacuum 1 = reflective
*/
BoundaryType Boundaries::getSurfaceType(int axis, int side) {
    return _surfaces[axis*2+side]->getType();
}
