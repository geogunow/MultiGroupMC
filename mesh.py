'''
 @file      mesh.py
 @brief     Contains the Mesh class
 @author    Luke Eure
 @date      October 22, 2015
'''

import numpy as np
import copy

'''
 @class Mesh mesh.py "mesh.py"
 @brief Tally contains information about a mesh that divides up the reactor
        core
'''
class Mesh():
    def __init__(self, bounds, delta_x = 0, delta_y=0, delta_z=0,
            default_material='water'):
        self._delta_axes = {'x': delta_x, 'y': delta_y, 'z': delta_z}
        self._boundary_mins = {'x': bounds.get_surface_coord('x', 'min'),
                'y': bounds.get_surface_coord('y', 'min'),
                'z': bounds.get_surface_coord('z', 'min')}
        
        # create flux array
        self._axis_sizes = {'x': 0, 'y': 0, 'z': 0}
        for i, axis in enumerate(['x', 'y', 'z']):
            self._axis_sizes[axis] = int(( \
                    bounds.get_surface_coord(axis, 'max') - \
                    self._boundary_mins[axis])/self._delta_axes[axis])

        self._flux = np.zeros((self._axis_sizes['x'],
            self._axis_sizes['y'], self._axis_sizes['z']))

        # create material array
        self._cell_materials = [[[default_material \
                for k in range(self._axis_sizes['z'])] \
                for j in range(self._axis_sizes['y'])]  \
                for i in range(self._axis_sizes['x'])]

    def get_cell(self, position, direction=[0,0,0]):
        cell_num_vector = list()
        for num, axis in zip(xrange(3), ['x', 'y', 'z']):
            cell_num = int(round((position[num]-self._boundary_mins[axis]) \
                    / self._delta_axes[axis], 7))

            # correct error if neutron is on upper boundary of the geometry
            if cell_num == self._flux.shape[num]:
                cell_num -= 1

            # correct error if neutron is on upper boundary of cell
            if position[num] == (self._boundary_mins[axis] \
                    + cell_num*self._delta_axes[axis]) \
                    and direction[num] < 0:
                        cell_num -=1

            cell_num_vector.append(cell_num)
        return cell_num_vector

    def flux_add(self, cell, distance):
        self._flux[cell[0]][cell[1]][cell[2]] += distance
    
    def flux_clear(self):
        self._flux[:][:][:] = 0

    def display_flux(self):
        print self._flux

    def display_materials(self):
        print self._cell_materials

    def get_flux(self):
        return self._flux
    
    def get_cell_max(self, position, direction=[0,0,0]):
        cell_number = self.get_cell(position, direction)
        maxes = dict()
        for i, axis in enumerate(['x', 'y', 'z']):
            maxes.update({axis:((cell_number[i] + 1) * \
                    self._delta_axes[axis] + self._boundary_mins[axis])})
        return maxes

    def get_cell_min(self, position, direction=[0,0,0]):
        cell_number = self.get_cell(position, direction)
        mins = dict()
        for i, axis in enumerate(['x', 'y', 'z']):
            mins.update({axis:(cell_number[i] * self._delta_axes[axis] \
                    + self._boundary_mins[axis])})
        return mins

    def get_material(self, cell_number):
        return self._cell_materials[ \
                cell_number[0]][cell_number[1]][cell_number[2]]

    # fill cells with a certain material.
    # Locations should be a 3x2 array with the maxes and mins of the area to
    # be filled in each dimension
    def fill_material(self, material_type, locations):
        x_smallest_cell = self.get_cell([locations[0][0],
            self._boundary_mins['x'],self._boundary_mins['x']])
        x_largest_cell = self.get_cell([locations[0][1],
            self._boundary_mins['x'],self._boundary_mins['x']])
        y_smallest_cell = self.get_cell([self._boundary_mins['y'],
            locations[1][0],self._boundary_mins['y']])
        y_largest_cell = self.get_cell([self._boundary_mins['y'],
            locations[1][1],self._boundary_mins['y']])
        z_smallest_cell = self.get_cell([self._boundary_mins['z'],
            self._boundary_mins['z'],locations[2][0]])
        z_largest_cell = self.get_cell([self._boundary_mins['z'],
            self._boundary_mins['z'],locations[2][1]])
        for i in range(x_smallest_cell[0], x_largest_cell[0]+1):
            for j in range(y_smallest_cell[1], y_largest_cell[1]):
                for k in range(z_smallest_cell[2], z_largest_cell[2]+1):
                    self._cell_materials[i][j][k] = material_type
