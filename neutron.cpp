/** 
 @file      neutron.cpp
 @brief     contains functions for the Neutron class
 @author    Luke Eure
 @date      January 8 2016
*/

#include "neutron.h"


/**
 @brief constructor for Neutron class
 @param position a vector containing the starting position of the neutron
 @param theta the polar angle of the neutron's initial direction
 @param phi the azimuthal angle of the neutron's initial direction
*/
Neutron::Neutron(std::vector <double> &position, double theta, double phi) {
    _xyz = position;
    _neutron_alive = true;
    _neutron_direction.resize(3);
    setDirection(theta, phi);
}

/**
 @brief deconstructor for Neutron class
*/
Neutron::~Neutron() {}

/**
 @brief tells if the neutron is alive
 @return a bool (true=neutron alive. false=neutron dead)
*/
bool Neutron::alive(){
    return _neutron_alive;
}

/**
 @brief returns the energy group of the neutron
 @return an int: the energy group of the neutron
*/
int Neutron::getGroup() {
    return _neutron_group;
}

/**
 @brief moves the neutron a given distance
 @param distance the distance the neutron should be moved
*/
void Neutron::move(double distance) {
    for (int i=0; i<3; ++i) {
        _xyz[i] += _neutron_direction[i]*distance;
    }
}

/**
 @brief moves the neutron a given distance
 @param distance the distance the neutron should be moved
*/
void Neutron::reflect(int axis) {
    _neutron_direction[axis] *= -1;
}

/**
 @brief sets the neutron's direction of travel
 @param theta the polar angle
 @param phi the azimuthal angle
*/
void Neutron::setDirection(double theta, double phi) {
    _neutron_direction[0] = sin(theta)*cos(phi);
    _neutron_direction[1] = sin(theta)*sin(phi);
    _neutron_direction[2] = cos(theta);
}

/**
 @brief sets the neutron's position along an axis
 @param axis the axis along which the position will be set
 @param value the value to which the position will be set
*/
void Neutron::setPosition(int axis, double value) {
    _xyz[axis] = value;
}

/**
 @brief sets the cell of the neutron
 @param cell_number the cell to which the nuetron will be set
*/
void Neutron::setCell(std::vector <int> &cell_number) {
    _neutron_cell = cell_number;
}

/**
 @brief kills the neutron
*/
void Neutron::kill() {
    _neutron_alive = false;
}

/**
 @brief returns the neutron's cell
 @return the cell in which the neutron resides
*/
std::vector <int> Neutron::getCell() {
    return _neutron_cell;
}

/**
 @brief gets the position of the neutron along a certain axis
 @param axis an int containing the axis along which the position will be
        returned
 @return a double denoting the neutron's position along axis
*/
double Neutron::getPosition(int axis) {
    return _xyz[axis];
}

/**
 @brief gets the position vector of the neutron
 @return a vector containing the neutron's position
*/
std::vector <double> Neutron::getPositionVector() {
    return _xyz;
}

/**
 @brief gets the direction of the neutron along a certain axis
 @param axis an int containing the axis along which the direction will be
        returned
 @return a double denoting the neutron's direction along axis
*/
double Neutron::getDirection(int axis) {
    return _neutron_direction[axis];
}

/**
 @brief gets the direction vector of the neutron
 @return a vector containing the neutron's direction
*/
std::vector <double> Neutron::getDirectionVector() {
    return _neutron_direction;
}

/**
 @brief gest the neutron's distance from a given point
 @param coord a vector denoting the point to find the neutron's distance from
 @return the neutron's distance from that point
*/
double Neutron::getDistance(std::vector <double> &coord) {
    return sqrt(pow(getPosition(0)-coord[0], 2.0)
            + pow(getPosition(1)-coord[1], 2.0)
            + pow(getPosition(2)-coord[2], 2.0));
}

/**
 @brief set the neutron's group
 @param new_group the new energy group of the neutron
*/
void Neutron::setGroup(int new_group) {
    _neutron_group = new_group;
}
