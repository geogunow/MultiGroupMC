/* 
 @file      boundaries.h
 @brief     contains the boundaries class
 @author    Luke Eure
 @date      January 6 2016
*/

#ifndef BOUNDARIES_H
#define BOUNDARIES_H

enum BoundaryType {
    VACUUM,
    REFLECTIVE,
    BOUNDARY_NONE
};

class Boundaries {

private:
    int _surface_coords[6];
    int _surface_type[6];
    bool _surface_init[6];

public:
    Boundaries();
    virtual ~Boundaries();

    float getSurfaceCoord(int axis, int side);
    float getSurfaceType(int axis, int side);
    void setSurfaceCoord(int axis, int side, float coord);
    void setSurfaceType(int axis, int side, BoundaryType type);

};

#endif
