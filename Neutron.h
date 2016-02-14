/* 
 @file      Neutron.h
 @brief     contains Neutron class
 @author    Luke Eure
 @date      January 8 2016
*/

#ifndef NEUTRON_H
#define NEUTRON_H

#include <vector>
#include <cmath>
#include <stdio.h>
#include <math.h>
#include <iostream>

#include "Distributions.h"
#include "Surface.h"

class Neutron {
public:
    Neutron(std::vector <double> &position, double theta, double phi);
    virtual ~Neutron();
    void move(double distance);
    void reflect(int axis);
    void setDirection(double theta, double phi);
    void setPosition(int axis, double value);
    void setCell(std::vector <int> &cell_number);
    void setGroup(int new_group);
    void kill();
    void changeCell(int axis, min_max side);
    double x();
    double y();
    double z();
    double getPosition(int axis);
    double getDirection(int axis);
    double getDistance(std::vector <double> &coord);
    bool alive();
    int getGroup();
    std::vector <int> getCell();
    std::vector <double> getPositionVector();
    std::vector <double> getDirectionVector();

private:
    
    /** tells if the neutron is alive */
    bool _neutron_alive;

    /** energy group of the neutron */
    int _neutron_group;

    /** position of the neutron */
    std::vector <double> _xyz;

    /** direction of travel of the neutron */
    std::vector <double> _neutron_direction;

    /** cell of the neutron */
    std::vector <int> _neutron_cell;
};

#endif
