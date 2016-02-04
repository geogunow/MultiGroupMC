/* 
 @file      tally.h
 @brief     contains Tally class
 @author    Luke Eure
 @date      January 8 2016
*/

#ifndef TALLY_H
#define TALLY_H

class Tally {
private:

    /** count stored in the tally */
    double _tally_count;
    
public:    
    Tally();
    virtual ~Tally();
    
    void add(double amt);
    void clear();
    double getCount();

};

#endif
