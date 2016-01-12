/** 
 @file      neutron.h
 @brief     contains Neutron class
 @author    Luke Eure
 @date      January 8 2016
*/

#ifndef NEUTRON_H
#define NEUTRON_H

#include <cmath>
#include <stdio.h>
#include <math.h>

class Neutron {
private:
    bool _neutron_alive;
    double* _xyz;
    int _neutron_group;
    double _neutron_direction [3];
    int* _neutron_cell;


public:
    Neutron(double *position, double theta, double phi);
    ~Neutron();

    double x();
    double y();
    double z();
    bool alive();
    int getGroup();
    void move(double distance);
    void reflect(int axis);
    void setDirection(double theta, double phi);
    void setPosition(int axis, double value);
    void setCell(int *cell_number);
    void kill();
    int* getCell();
    double getPosition(int axis);
    double* getPositionVector();
    double getDirection(int axis);
    double* getDirectionVector();
    double getDistance(double *coord);
    void setGroup(int new_group);


};

#endif
