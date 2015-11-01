'''
 @file      material.py
 @brief     Contains the Boundaries class
 @author    Geoffrey Gunow
 @date      October 7, 2015
'''

'''
 @class Boundaries boundaries.py "boundaries.py"
 @brief Contains information about the boundaries of a geometry and the
        associated boundary conditions
'''
class Boundaries():
    def __init__(self, x_min, x_max, y_min, y_max, z_min, z_max):

        # create dictionary to store boundaries
        self._surfaces = dict()
        for surface in ['x', 'y', 'z']:
            self._surfaces[surface] = {'min':{}, 'max':{}}

        # store boundary locations
        self._surfaces['x']['min']['coord'] = x_min
        self._surfaces['x']['max']['coord'] = x_max
        self._surfaces['y']['min']['coord'] = y_min
        self._surfaces['y']['max']['coord'] = y_max
        self._surfaces['z']['min']['coord'] = z_min
        self._surfaces['z']['max']['coord'] = z_max
        
        # store boundary types (0 = vacuum, 1 = reflective)
        # initialize all boundaries as vacuum
        for surface in self._surfaces:
            for side in ['min', 'max']:
                self._surfaces[surface][side]['type'] = 1 

    def get_x_max(self):
        return self._surfaces['x']['max']['coord'] 

    def get_x_min(self):
        return self._surfaces['x']['min']['coord'] 

    def get_y_max(self):
        return self._surfaces['y']['max']['coord'] 

    def get_y_min(self):
        return self._surfaces['y']['min']['coord'] 

    def get_z_max(self):
        return self._surfaces['z']['max']['coord']
    
    def get_z_min(self):
        return self._surfaces['z']['min']['coord']

    # functions to get the surface types
    def get_surface_type(self, surface):
        if surface == 'x_max':
            return self._surfaces['x']['max']['type']
        if surface == 'x_min':
            return self._surfaces['x']['min']['type']
        if surface == 'y_max':
            return self._surfaces['y']['max']['type']
        if surface == 'y_min':
            return self._surfaces['y']['min']['type']
        if surface == 'z_max':
            return self._surfaces['z']['max']['type']
        if surface == 'z_min':
            return self._surfaces['z']['min']['type']
