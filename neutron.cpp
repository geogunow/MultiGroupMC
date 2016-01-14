/** 
 @file      neutron.cpp
 @brief     contains functions for the Neutron class
 @author    Luke Eure
 @date      January 8 2016
*/

#include "neutron.h"

Neutron::Neutron(std::vector <double> &position, double theta, double phi) {
    _xyz = position;
    _neutron_alive = true;

    /** how do I call a class function from within another class function? */
    setDirection(theta, phi);

}

Neutron::~Neutron() {}

double Neutron::x() {
    return _xyz[0];
}

double Neutron::y() {
    return _xyz[1];
}

double Neutron::z() {
    return _xyz[2];
}

bool Neutron::alive(){
    return _neutron_alive;
}

int Neutron::getGroup() {
    return _neutron_group;
}

void Neutron::move(double distance) {

    for (int i=0; i<3; ++i) {
        
        /** this won't work because _xyz is a pointer and _neutron_direction is
          an array */
        _xyz[i] = _neutron_direction[i]*distance;
    }
}

void Neutron::reflect(int axis) {
    _neutron_direction[axis] *= -1;
}

void Neutron::setDirection(double theta, double phi) {
    _neutron_direction.resize(3);
    _neutron_direction[0] = sin(theta)*cos(phi);
    _neutron_direction[1] = sin(theta)*sin(phi);
    _neutron_direction[2] = cos(theta);
}

void Neutron::setPosition(int axis, double value) {
    _xyz[axis] = value;
}

void Neutron::setCell(std::vector <int> &cell_number) {
    _neutron_cell = cell_number;
}

void Neutron::kill() {
    _neutron_alive = false;
}

std::vector <int> Neutron::getCell() {
    return _neutron_cell;
}

double Neutron::getPosition(int axis) {
    return _xyz[axis];
}

std::vector <double> Neutron::getPositionVector() {
    return _xyz;
}

double Neutron::getDirection(int axis) {
    return _neutron_direction[axis];
}

std::vector <double> Neutron::getDirectionVector() {
    return _neutron_direction;
}

double Neutron::getDistance(std::vector <double> &coord) {
    return sqrt(pow(x()-coord[0], 2.0)
            + pow(y()-coord[1], 2.0)
            + pow(z()-coord[2], 2.0));
}

void Neutron::setGroup(int new_group) {
    _neutron_group = new_group;
}



