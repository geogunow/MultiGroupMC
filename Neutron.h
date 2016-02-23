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
#include <time.h>
#include <stdlib.h>

#include "Surface.h"

class Neutron {
public:
    Neutron(int neutron_num);
    virtual ~Neutron();

    void changeCell(int axis, min_max side);
    void kill();
    void move(double distance);
    void reflect(int axis);
    void setCell(std::vector <int> &cell_number);
    void setGroup(int new_group);
    void setPosition(int axis, double value);
    void setPositionVector(std::vector <double> &position);
    void sampleDirection();
    double arand();
    double getDirection(int axis);
    double getDistance(std::vector <double> &coord);
    double getPosition(int axis);
    double x();
    double y();
    double z();
    bool alive();
    int getGroup();
    int rand();
    int sampleNeutronEnergyGroup(std::vector <double> chi);
    int sampleScatteredGroup(std::vector <double> &scattering_matrix,
            int group);
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

    /** identification number */
    int _id;

    /** to be used in calling rand_r() */
    unsigned int _seed;
};

#endif
