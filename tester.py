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

# create test materials
fuel_material = material.Material(sigma_t=2.0, sigma_s=1.0,
        nu=2.4, sigma_f=.5)
water = material.Material(sigma_t=2.0, sigma_s=1.0,
        nu=2.4, sigma_f=0.0)

test_bounds =  boundaries.Boundaries(-10.0, 10.0, -10.0, 10.0, -10.00, 10.0)

test_mesh = mesh.Mesh(test_bounds, 1.0, 1.0, 1.0, default_material=None)

# fill mesh with some fuel
test_mesh.fill_material(fuel_material, [[-6.0, -5.0],[-6.0,-5.0],[-10.0,10.0]])
test_mesh.fill_material(fuel_material, [[-6.0, -5.0],[-3.0,-2.0],[-10.0,10.0]])
test_mesh.fill_material(fuel_material, [[-6.0, -5.0],[0.0,1.0],[-10.0,10.0]])
test_mesh.fill_material(fuel_material, [[5.0, 6.0],[-6.0,-5.0],[-10.0,10.0]])
test_mesh.fill_material(fuel_material, [[5.0, 6.0],[-3.0,-2.0],[-10.0,10.0]])
test_mesh.fill_material(fuel_material, [[5.0, 6.0],[0.0,1.0],[-10.0,10.0]])

# fill the rest of the mesh with water
test_mesh.fill_material(water, [[-10.0, 10.0],[-10.0, -6.0],[-10.0,10.0]])
test_mesh.fill_material(water, [[-10.0, 10.0],[-5.0, -3.0],[-10.0,10.0]])
test_mesh.fill_material(water, [[-10.0, 10.0],[-2.0, 0.0],[-10.0,10.0]])
test_mesh.fill_material(water, [[-10.0, 10.0],[1.0, 10.0],[-10.0,10.0]])

test_mesh.fill_material(water, [[-10.0, -6.0],[-10.0, 10.0],[-10.0,10.0]])
test_mesh.fill_material(water, [[-5.0, 5.0],[-10.0, 10.0],[-10.0,10.0]])
test_mesh.fill_material(water, [[6.0, 10.0],[-10.0, 10.0],[-10.0,10.0]])

# run simulation
monte_carlo.generate_neutron_histories(n_histories=10000, 
        bounds=test_bounds, mesh=test_mesh,
        num_batches=3)

# display plot of neutron flux
index = 2
plotter.plot_heat_map(test_mesh.get_flux(), index, repeat = 5)
