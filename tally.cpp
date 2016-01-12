/** 
 @file      tally.cpp
 @brief     contains functions for the Tally class
 @author    Luke Eure
 @date      January 8 2016
*/

#include "tally.h"

Tally::Tally() {}

Tally::~Tally() {}

void Tally::add(double amt) {
    _tally_count += amt;
}

void Tally::clear() {
    _tally_count = 0;
}
 
double Tally::getCount() {
    return _tally_count;
}
