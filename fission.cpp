/* 
 @file      fission.cpp
 @brief     contains functions for the Fission class
 @author    Luke Eure
 @date      January 9 2016
*/

#include "fission.h"

/*
 @brief constructor for Fission class
*/
Fission::Fission() {}

/*
 @brief deconstructor
*/
Fission::~Fission() {}

/*
 @brief clears the fission list  
*/
void Fission::clear() {
    _fission_locations.clear();
}

/*
 @brief  return the number of fission locations
 @return the length of the fission array 
*/
int Fission::length() {
    return _fission_locations.size();
}

/*
 @brief add a location to the fission list
 @param  a double containing a location 
*/
void Fission::add(std::vector <double> &location) {
    _fission_locations.push_back(location);
}

/*
 @brief returns a single fission location
 @param a number indexing the fission location
 @return a fission location
*/
std::vector <double> Fission::location(int num) {
    return _fission_locations[num];
}

/*
 @brief returns the first fission location then deletes that location from the
        bank
 @return the first location in the fission list
*/
std::vector <double> Fission::next() {
    _item = _fission_locations[0];
    _fission_locations.erase(_fission_locations.begin());
    return _item;
}
