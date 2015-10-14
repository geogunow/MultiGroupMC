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
class Coords():
    def __init__(self, x, y, z):
        self._data = np.array([x, y, z])
    
    @property
    def x(self):
        return self._data[0]

    @property
    def y(self):
        return self._data[1]

    @property
    def z(self):
        return self._data[2]

    def getPolar(self):
        return atan(self.z / sqrt(self.x**2 + self.y**2 + self.z**2))

    def getAzimuthal(self):
        return acos(self.x / self.y)

    def getDistance(self):
        return sqrt(self.x**2 + self.y**2 + self.z**2)

    # overloading operators for use as Coords (*/+/-) value
    def __add__(self, other):
        return self._data + other

    def __radd__(self, other):
        return self._data + other
    
    def __sub__(self, other):
        return self._data - other
    
    def __rsub__(self, other):
        return other - self._data

    def __mul__(self, other):
        return self._data * other

    def __rmul__(self, other):
        return self._data * other

    
    # functions that edit  existing instances of Coord
    def __sadd__(self, other):
        self._data += other
    def __ssub__(self, other):
        self._data -= other
    def __smul__(self, other):
        self._data = self._data * other
