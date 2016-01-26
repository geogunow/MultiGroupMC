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
    std::vector <double> _xyz;
    std::vector <double> _neutron_direction;
    std::vector <int> _neutron_cell;

public:
    Neutron(std::vector <double> &position, double theta, double phi);
    virtual ~Neutron();

    double x(); 
    double y(); 
    double z();
    double getPosition(int axis);
    double getDirection(int axis);
    double getDistance(std::vector <double> &coord);
    bool alive();
    int getGroup();
    void move(double distance);
    void reflect(int axis);
    void setDirection(double theta, double phi);
    void setPosition(int axis, double value);
    void setCell(std::vector <int> &cell_number);
    void setGroup(int new_group);
    void kill();
    std::vector <int> getCell();
    std::vector <double> getPositionVector();
    std::vector <double> getDirectionVector();
};

#endif
