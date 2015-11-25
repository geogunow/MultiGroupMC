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

test_bounds =  boundaries.Boundaries(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0)

test_mesh = mesh.Mesh(test_bounds, .1, .1, .1, default_material='water')

# create test materials
fuel_material = material.Material(sigma_t=2.0, sigma_s=1.0,
        nu=2.4, sigma_f=.5)
water_material = material.Material(sigma_t=2.0, sigma_s=1.0,
        nu=2.4, sigma_f=0.0)
test_materials = {'fuel': fuel_material, 'water': water_material}

# fill mesh with some fuel
test_mesh.fill_material('fuel', [[-.5,1.0],[-1.0,0.0],[-1.0,1.0]])

# run simulation
monte_carlo.generate_neutron_histories(n_histories=10000, 
        materials = test_materials, bounds=test_bounds, mesh=test_mesh,
        num_batches=5)

# display plot of neutron flux
index = 2
plotter.plot_heat_map(test_mesh.get_flux(), index, repeat = 50)
