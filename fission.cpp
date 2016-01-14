/** 
 @file      fission.cpp
 @brief     contains functions for the Fission class
 @author    Luke Eure
 @date      January 9 2016
*/

#include "fission.h"

Fission::Fission() {}

Fission::~Fission() {}
    
void Fission::clear() {
    _fission_locations.clear();
}

int Fission::length() {
    return _fission_locations.size();
}

void Fission::add(std::vector <double> &location) {
    _fission_locations.push_back(location);
}

std::vector <double> Fission::location(int num) {
    return _fission_locations[num];
}

std::vector <double> Fission::next() {
    _item =_fission_locations[0];
    _fission_locations.erase(_fission_locations.begin());
    return _item;
}
