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
        self._delta_x = delta_x
        self._delta_y = delta_y
        self._delta_z = delta_z
        self._x_min = bounds.get_x_min()
        self._y_min = bounds.get_y_min()
        self._z_min = bounds.get_z_min()
        '''
        self._flux = [[[0]*int((bounds.get_z_max()-bounds.get_z_min())/delta_z)] \
                * int((bounds.get_y_max()-bounds.get_y_min())/delta_y)] \
                * int((bounds.get_x_max()-bounds.get_x_min())/delta_x)
        '''
        self._flux = np.zeros(( \
                (bounds.get_x_max()-bounds.get_x_min())/delta_x, \
                (bounds.get_y_max()-bounds.get_y_min())/delta_y, \
                (bounds.get_z_max()-bounds.get_z_min())/delta_z))

    def get_cell(self, position):
        x = int((position[0]-self._x_min)/self._delta_x)
        y = int((position[1]-self._y_min)/self._delta_y)
        z = int((position[2]-self._z_min)/self._delta_z)
        return [x, y, z]

    def flux_add(self, cell, distance):
        self._flux[cell[0]][cell[1]][cell[2]] += distance
        
    def display_flux(self):
        print self._flux

    def get_cell_max(self, position):
        cell_number = self.get_cell(position)
        x_max = (cell_number[0] + 1) * self._delta_x
        y_max = (cell_number[1] + 1) * self._delta_y
        z_max = (cell_number[2] + 1) * self._delta_z
        return [x_max, y_max, z_max]

    def get_cell_min(self, position):
        cell_number = self.get_cell(position)
        x_min = cell_number[0]*self._delta_x
        y_min = cell_number[1]*self._delta_y
        z_min = cell_number[2]*self._delta_z
        return [x_min, y_min, z_min]
