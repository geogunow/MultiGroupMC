/* 
 @file      Tally.cpp
 @brief     contains functions for the Tally class
 @author    Luke Eure
 @date      January 8 2016
*/

#include "Tally.h"

/*
 @brief     constructor for Tally class
*/
Tally::Tally() {}

/*
 @brief     deconstructor
*/
Tally::~Tally() {}

/*
 @brief     add an amount to the tally and add its square to _tally_squared
 @param     tally_addition an amount to be added
*/
void Tally::add(double tally_addition) {
    _tally_count += tally_addition;
    _tally_squared += tally_addition * tally_addition;
}

/*
 @brief     sets the tally and _tally_squared equal to zero
*/
void Tally::clear() {
    _tally_count = 0;
    _tally_squared = 0;
}
 
/*
  @brief    gets the current tally count
  @return   a double: the number stored in the tally
*/
double Tally::getCount() {
    return _tally_count;
}
