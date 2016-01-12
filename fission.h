/** 
 @file      fision.h
 @brief     contains the fission class
 @author    Luke Eure
 @date      January 6 2016
*/

#ifndef FISSION_H
#define FISSION_H

#include <vector>

class Fission {
private:

    // the 50 place needs to be expandable
    std::vector<double*> _fission_locations;
    double *item;

public:
    
    Fission();
    virtual ~Fission();
    
    void clear();
    int length();
    void add(double location[3]);

    /** these should actually return cartesian coordinates */
    double* location(int num);
    double* next();

};

#endif
