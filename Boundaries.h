/* 
 @file      Boundaries.h
 @brief     container for Boundary objects
 @author    Luke Eure
 @date      January 6 2016
*/

#ifndef BOUNDARIES_H
#define BOUNDARIES_H

#include "Surface.h"
#include "Neutron.h"

#include <vector>

class Boundaries {

public:
    Boundaries();
    virtual ~Boundaries();

    float getSurfaceCoord(int axis, int side);
    BoundaryType getSurfaceType(int axis, int side);
    void setSurface(Axes axis, min_max side, Surface* surface);
    std::vector <double> sampleLocation(Neutron* neutron);

private:

    /** container for Surface objects */
    Surface* _surfaces[6];
};

#endif
