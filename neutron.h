/** 
 @file      neutron.h
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

class Neutron {
private:
    bool _neutron_alive;
    int _neutron_group;
    std::vector <double> _xyz,
        _neutron_direction;
    std::vector <int> _neutron_cell;

public:
    Neutron(std::vector <double> &position, double theta, double phi);
    virtual ~Neutron();

    double x(), 
        y(), 
        z(),
        getPosition(int axis),
        getDirection(int axis),
        getDistance(std::vector <double> &coord);
    bool alive();
    int getGroup();
    void move(double distance),
        reflect(int axis),
        setDirection(double theta, double phi),
        setPosition(int axis, double value),
        setCell(std::vector <int> &cell_number),
        setGroup(int new_group),
        kill();
    std::vector <int> getCell();
    std::vector <double> getPositionVector(),
        getDirectionVector();
};

#endif
