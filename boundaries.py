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
        for surface in self._surfaces:
            for side in ['min', 'max']:
                self._surfaces[surface][side]['type'] = 1 

    '''
     @brief returns the coordinate of the maximum or minimum boundary for a
            given axis
    '''
    def get_surface_coord(self, axis, side):
        return self._surfaces[axis][side]['coord']

    '''
     @brief returns the material type for a given boundary
    '''
    def get_surface_type(self, axis, side):
        return self._surfaces[axis][side]['type']
