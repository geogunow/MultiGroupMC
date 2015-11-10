'''
 @file      mesh.py
 @brief     Contains the Mesh class
 @author    Luke Eure
 @date      October 22, 2015
'''

import numpy as np

'''
 @class Mesh mesh.py "mesh.py"
 @brief Tally contains information about a mesh that divides up the reactor
        core
'''
class Mesh():
    def __init__(self, bounds, delta_x, delta_y, delta_z):
        self._delta_axes = {'x': delta_x, 'y': delta_y, 'z': delta_z}
        self._delta_x = delta_x
        self._delta_y = delta_y
        self._delta_z = delta_z
        self._boundary_mins = {'x': bounds.get_x_min(),
                'y': bounds.get_y_min(), 'z': bounds.get_z_min()}
        self._x_min = bounds.get_x_min()
        self._y_min = bounds.get_y_min()
        self._z_min = bounds.get_z_min()
        self._flux = np.zeros(( \
                (bounds.get_x_max()-bounds.get_x_min())/delta_x, \
                (bounds.get_y_max()-bounds.get_y_min())/delta_y, \
                (bounds.get_z_max()-bounds.get_z_min())/delta_z))

    # Should I run this function as a loop or explicitly assign each value?
    def get_cell(self, position):
        x = int((position[0]-self._x_min)/self._delta_x)
        y = int((position[1]-self._y_min)/self._delta_y)
        z = int((position[2]-self._z_min)/self._delta_z)
        '''
        cell_num = list()
        for axis in ['x', 'y', 'z']:
        ''' 
        return [x, y, z]

    def flux_add(self, cell, distance):
        self._flux[cell[0]][cell[1]][cell[2]] += distance
    
    def display_flux_sum(self):
        total = 0
        for i in self._flux:
            for j in i:
                total += sum(j)
        print total

    def display_flux(self):
        print self._flux

    def get_cell_max(self, position):
        cell_number = self.get_cell(position)
        x_max = (cell_number[0] + 1) * self._delta_x + self._x_min
        y_max = (cell_number[1] + 1) * self._delta_y + self._y_min
        z_max = (cell_number[2] + 1) * self._delta_z + self._z_min
        return {'x': x_max, 'y': y_max, 'z': z_max}

    def get_cell_min(self, position):
        cell_number = self.get_cell(position)
        x_min = cell_number[0]*self._delta_x + self._x_min
        y_min = cell_number[1]*self._delta_y + self._y_min
        z_min = cell_number[2]*self._delta_z + self._z_min
        return {'x': x_min, 'y': y_min, 'z': z_min}
