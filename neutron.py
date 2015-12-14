'''
 @file      neutron.py
 @brief     Contains the Neutron class
 @author    Geoffrey Gunow
 @date      October 7, 2015
'''

import numpy as np
from math import *
from copy import deepcopy as copy

'''
 @class Neutron neutron.py "neutron.py"
 @brief Contains neutron position and direction information
'''
class Neutron():
    def __init__(self, position, theta, phi, group):
        
        self._xyz = np.array(copy(position))
        self._alive = True
        self._group = group
        self.set_direction(theta, phi)
        self.association = {'x': 0, 'y':1, 'z':2}

    @property
    def x(self):
        return self._xyz[0]

    @property
    def y(self):
        return self._xyz[1]

    @property
    def z(self):
        return self._xyz[2]

    @property
    def alive(self):
        return self._alive

    @property
    def group(self):
        return self._group
    '''
     @brief move a neutron a given distance along its direction of travel
    '''
    def move(self, distance):
        self._xyz += self._direction * distance 

    '''
     @brief reverse the direction of travel along a given axis
    '''
    def reflect(self, axis):
        self._direction[self.association[axis]] *= -1

    '''
     @brief set the neutron's direction of travel given its angle in sphereical
            coordinates
    '''
    def set_direction(self, theta, phi):
        self._direction = np.array([sin(theta)*cos(phi), sin(theta)*sin(phi),
            cos(theta)])

    '''
     @brief set the neuton's position
    '''
    def set_position(self, var, value):
        self._xyz[self.association[var]] = value
    
    '''
     @brief set the neutron's cell
    '''
    def set_cell(self, cell_number):
        self._cell = cell_number

    '''
     @brief kill the neutron
    '''
    def kill(self):
        self._alive = False

    '''
     @brief return the cell of the neutron
    '''
    def get_cell(self):
        return self._cell
    
    '''
     @brief return the position of the neutron along a given axis
    '''
    def get_position(self, var):
        return self._xyz[self.association[var]]

    '''
     @brief return the position of the neutron in all three dimensions
    '''
    def get_position_vector(self):
        return copy(self._xyz)
    
    '''
     @brief return the neutron's direction of travel along a given axis
    '''
    def get_direction(self, var):
        return self._direction[self.association[var]]
        
    '''
     @brief return the neutron's direction of travel in all three dimensions
    '''
    def get_direction_vector(self):
        return copy(self._direction)

    '''
     @brief return the neutron's distance from the origin
    '''
    def get_distance_from_origin(self):
        return sqrt(self.x**2 + self.y**2 + self.z**2)

    '''
     @brief return the neutron's distance from a given coordinate
    '''
    def get_distance(self, coord):
        return sqrt((self.x - coord[0])**2 + (self.y - coord[1])**2 + \
                (self.z - coord[2])**2)

    '''
     @brief return's data about the neutron
    '''
    def __repr__(self):
        string = "<Neutron at position " + str(self._xyz) + " traveling in " \
                + "direction " + str(self._direction) + ">"
        return string

    '''
     @brief set the neutron's energy group
    '''
    def set_group(self, new_group):
        self._group = new_group
