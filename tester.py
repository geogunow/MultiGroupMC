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


# define chi
test_chi = [.6, .3, .1]

# create cross sections
test_num_groups = 3
test_sigma_s = [[.8,1.0,1.2],[.7,.9,1.4],[.9,1.0,1.1]]
test_sigma_t = [4.0, 4.0, 4.0]
test_sigma_f = [.4,.3,.2]

# create test materials
fuel_material = material.Material(sigma_t= test_sigma_t, sigma_s=test_sigma_s,
        nu=2.4, sigma_f= test_sigma_f, chi = test_chi)
water = material.Material(sigma_t= test_sigma_t, sigma_s=test_sigma_s,
        nu=2.4, sigma_f=[0.0, 0.0, 0.0], chi = test_chi)

test_bounds =  boundaries.Boundaries(-10.0, 10.0, -10.0, 10.0, -10.00, 10.0)

test_mesh = mesh.Mesh(test_bounds, 1.0, 1.0, 1.0, num_groups = test_num_groups,
        default_material=None)

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
monte_carlo.generate_neutron_histories(n_histories=10**6,
        bounds=test_bounds, mesh=test_mesh, num_batches=2)

# display plot of neutron flux
index = 2
flux_to_plot = test_mesh.get_flux()
for i in range(test_num_groups):
    plotter.plot_heat_map(flux_to_plot[i], index, repeat = 5,
            title = ('Group ' + str(i+1)))
