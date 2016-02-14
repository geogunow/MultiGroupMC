/* 
 @file      Surface.cpp
 @brief     contains functions for the Surface class
 @author    Luke Eure
 @date      February 13 2016
*/

#include "Surface.h"

/*
 @brief constructor for the Surface class
 @param type the type of boundary
 @param position the position of the boundary
*/
Surface::Surface(BoundaryType type, double position) {
    _boundary_type = type;
    _position = position;
}

/*
 @brief deconstructor
*/
Surface::~Surface() {}

/*
 @brief returns the location of the boundary
 @return a double denoting the location of the boundary
*/
double Surface::getPosition() {
    return _position;
}

/*
 @brief returns the type of the boundary
 @return a BoundaryType denoting the type of the boundary
*/
BoundaryType Surface::getType() {
    return _boundary_type;
}
