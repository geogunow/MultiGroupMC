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
    def __init__(self, bounds, spacing):
        '''
        self._cell_x = []
        self._cell_y = []
        self._cell_z = []
        self._cell_x_bounds = []
        self._cell_y_bounds = []
        self._cell_z_bounds = []
        '''

        self._cell = dict()
        for direction in ['x', 'y', 'z']:
            self._cell[direction] = {'number': [], 'min': [], 'max': []}
        # TODO: this will be off by one cell in instances when the spacing
        # doesn't lead to all cells being the exact same size. To fix this
        # the int() inside the xrange() needs to round up in all cases
        # when the argument isn't an integer value.
        # Also, is hardcoding the round(x, 5) the best way to avoid huge 
        # decimal errors?
        for i in xrange(int((float(bounds.get_x_max() - bounds.get_x_min()) \
                / spacing))):
            # self._cell_x.append(i)
            self._cell['x']['min'].append(round(bounds.get_x_min() + i*spacing,
                5))
            self._cell['x']['number'].append(i)
        for j in xrange(int((float(bounds.get_y_max() - bounds.get_y_min()) \
                / spacing))):
            # self._cell_y.append(j)
            self._cell['y']['min'].append(round(bounds.get_y_min() + j*spacing,
                5)) 
            self._cell['y']['number'].append(j)
        
        for k in xrange(int((float(bounds.get_z_max() - bounds.get_z_min()) \
                / spacing))):
            # self._cell_z.append(k)
            self._cell['z']['min'].append(round(bounds.get_z_min() + k*spacing,
                5))
            self._cell['z']['number'].append(k) 
        
        for direction in ['x', 'y', 'z']:
            for i in xrange(len(self._cell[direction]['min'])):
                self._cell[direction]['max'].append(round( \
                        self._cell[direction]['min'][i] + spacing, 5))

    def print_mesh(self):
        print self._cell['x']['number']
        print self._cell['y']['number']
        print self._cell['z']['number']
        print self._cell['x']['min']
        print self._cell['x']['max']
    
    def get_cell_bounds(self, direction, max_or_min, number):
        return self._cell[direction][max_or_min][int(number)]

    def get_cell(self, position):
        x = position[0]
        y = position[1]
        z = position[2]
        return 0
        raise NotImplementedError

    def get_mesh_length(self, direction):
        return len(self._cell[direction]['number'])
