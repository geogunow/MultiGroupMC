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

/*
 @brief     function that samples a random location within a bounding box.
 @details   a point is randomly and uniformally sampled in the bounding box 
            provided in the input.
 @param     bounds a Boundaries object containing the limits of the
            bounding box
 @return    _dist_location a vector that contains the coordinates of a point
*/
std::vector <double> Boundaries::sampleLocation(Neutron* neutron) {
    std::vector <double> _dist_location(3);
    for (int axis=0; axis<3; ++axis) {
        double width = getSurfaceCoord(axis, MAX) - getSurfaceCoord(axis, MIN);
        double coord = getSurfaceCoord(axis, MIN) + width * neutron->arand();
        _dist_location[axis] = coord;
    }
    return _dist_location;
}
