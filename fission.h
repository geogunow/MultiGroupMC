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

    std::vector<std::vector <double> > _fission_locations;
    std::vector <double> _item;

public:
    
    Fission();
    virtual ~Fission();
    
    void clear();
    void add(std::vector <double> &location);
    int length();
    std::vector <double> location(int num);
    std::vector <double> next();

};

#endif
