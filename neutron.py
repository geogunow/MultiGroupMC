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
#TODO: description + getters and setters
class Neutron():
    def __init__(self, position, theta, phi):
        
        self._xyz = np.array(copy(position))
        self._alive = True
        
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

    def move(self, distance):
        self._xyz += self._direction * distance 

    def reflect(self, surface):
        self._direction[self.association[surface]] *= -1

    def set_direction(self, theta, phi):
        self._direction = np.array([sin(theta)*cos(phi), sin(theta)*sin(phi),
            cos(theta)])

    def set_position(self, var, value):
        self._xyz[self.association[var]] = value
    
    def set_cell(self, cell_number):
        self._cell = cell_number

    def kill(self):
        self._alive = False

    def get_cell(self):
        return self._cell
    
    def get_position(self, var):
        return self._xyz[self.association[var]]

    def get_position_vector(self):
        return copy(self._xyz)
    
    def get_direction(self, var):
        return self._direction[self.association[var]]
        
    def get_direction_vector(self):
        return copy(self._direction)

    def get_distance_from_origin(self):
        return sqrt(self.x**2 + self.y**2 + self.z**2)

    def get_distance(self, coord):
        return sqrt((self.x - coord[0])**2 + (self.y - coord[1])**2 + \
                (self.z - coord[2])**2)

    def __repr__(self):
        string = "<Neutron at position " + str(self._xyz) + " traveling in " \
                + "direction " + str(self._direction) + ">"
        return string

