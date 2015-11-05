'''
 @file      coords.py
 @brief     Contains the Coords class
 @author    Geoffrey Gunow
 @date      October 7, 2015
'''

import numpy as np
from math import *

'''
 @class Coords coords.py "coords.py"
 @brief Contains a cartesian coordinate
'''
#TODO: MAKE NEUTRON CLASS
class Coords():
    def __init__(self, x, y, z):
        self._xyz = np.array([x, y, z])

    @property
    def x(self):
        return self._xyz[0]

    @property
    def y(self):
        return self._xyz[1]

    @property
    def z(self):
        return self._xyz[2]

    def move(self, direction, distance):
        self._xyz += direction * distance 
    
    def getCoordinate(self, var):
        if var == 'x':
            return self._xyz[0]
        elif var == 'y':    
            return self._xyz[1]
        elif var == 'z':
            return self._xyz[2]
        
        raise ValueError

    def getPolar(self):
        return atan(self.z / sqrt(self.x**2 + self.y**2 + self.z**2))

    def getAzimuthal(self):
        return acos(self.x / self.y)

    def getDistanceFromOrigin(self):
        return sqrt(self.x**2 + self.y**2 + self.z**2)

    def getDistance(self, other):
        return sqrt((self.x - other.x)**2 + (self.y - other.y)**2 + \
                (self.z - other.z)**2)

    def __repr__(self):
        string = "<x = " + str(self._xyz[0]) + ", y = " + str(self._xyz[1]) \
                    + ", z = " + str(self._xyz[2]) + ">"
        return string

