'''
 @file      mesh.py
 @brief     Contains the Mesh class
 @author    Luke Eure
 @date      October 22, 2015
'''

import numpy as np
import plotter

'''
 @class Mesh mesh.py "mesh.py"
 @brief Tally contains information about a mesh that divides up the reactor
        core
'''
class Mesh():
    def __init__(self, bounds, delta_x, delta_y, delta_z):
        self._delta_axes = {'x': delta_x, 'y': delta_y, 'z': delta_z}
        self._boundary_mins = {'x': bounds.get_surface_coord('x', 'min'),
                'y': bounds.get_surface_coord('y', 'min'),
                'z': bounds.get_surface_coord('z', 'min')}
        self._flux = np.zeros(( \
                (bounds.get_surface_coord('x', 'max') \
                - self._boundary_mins['x'])/delta_x,
                (bounds.get_surface_coord('y', 'max') \
                - self._boundary_mins['y'])/delta_y,
                (bounds.get_surface_coord('z', 'max') \
                - self._boundary_mins['z'])/delta_z))

    def get_cell(self, position):
        cell_num_vector = list()
        for num, axis in zip(xrange(3), ['x', 'y', 'z']):
            cell_num = int((position[num]-self._boundary_mins[axis]) \
                    / self._delta_axes[axis])

            # correct error if neutron is on upper boundary of the geometry
            if cell_num == self._flux.shape[num]:
                cell_num -= 1
            cell_num_vector.append(cell_num)
        return cell_num_vector

    def flux_add(self, cell, distance):
        self._flux[cell[0]][cell[1]][cell[2]] += distance
    
    def display_flux(self, index):
        plotter.plot_heat_map(self._flux, index)

    def get_cell_max(self, position):
        cell_number = self.get_cell(position)
        maxes = dict()
        for num, axis in zip(xrange(3), ['x', 'y', 'z']):
            maxes.update({axis:((cell_number[num] + 1) * \
                    self._delta_axes[axis] + self._boundary_mins[axis])})
        return maxes

    def get_cell_min(self, position):
        cell_number = self.get_cell(position)
        mins = dict()
        for num, axis in zip(xrange(3), ['x', 'y', 'z']):
            mins.update({axis:(cell_number[num] * self._delta_axes[axis] \
                    + self._boundary_mins[axis])})
        return mins
