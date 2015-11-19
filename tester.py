'''
 @file      tester.py
 @brief     A teting program for debugging
 @author    Luke Eure
 @date      November 1, 2015
'''

#import libraries
import boundaries
import distributions
import material
import monte_carlo
import plotter
import tally
import fission
import mesh
import plotter
import numpy as np

bounds =  boundaries.Boundaries(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0)

test_mesh = mesh.Mesh(bounds, .05, .05, .05)

test_material = material.Material(2.0, 1.0, 2.4, .5)

monte_carlo.generate_neutron_histories(1000, test_material, bounds, test_mesh, 1)

index = 9
plotter.plot_heat_map(test_mesh.get_flux(), index)
'''
test_array = np.zeros((3,3,3))
print test_array
test_array[1][1][1] = 3
test_array[2][1][2] = 2
print test_array
test_array[:][:][:] = 0
print test_array
'''
