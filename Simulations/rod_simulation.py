'''
 @file      pebble_simulation.py
 @brief     A program to test the simulation of a fuel rod
 @author    Luke Eure
 @date      December 23, 2015
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
chi = [1.0, 0.0]
num_groups = 2

# create test materials
fuel = material.Material(sigma_t=[2.0/9.0, 5.0/6.0],
        sigma_s=[[71.0/360.0, .02], [0.0, 11.0/15.0]],
        nu=2.4, sigma_f=[1.0/480.0, 1.0/16.0], chi = [1.0, 0.0])
water = material.Material(sigma_t= [2.0/9.0, 5.0/3.0],
        sigma_s=[[71.0/360.0, .0250], [0.0, 47.0/30.0]],
        nu=2.4, sigma_f=[0.0, 0.0], chi = [0.0, 0.0])

bounds =  boundaries.Boundaries(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0)

mesh = mesh.Mesh(bounds, 4.0/9.0, 4.0/9.0, 4.0/9.0,
        num_groups = num_groups,
        default_material=None)

# fill mesh with some fuel
mesh.fill_material(fuel, [[-2.0/3.0, 2.0/3.0],[-2.0/3.0, 2.0/3.0],[-2.0, 2.0]])

# fill the rest of the mesh with water
mesh.fill_material(water, [[-2.0, -2.0/3.0],[-2.0, 2.0],[-2.0, 2.0]])
mesh.fill_material(water, [[2.0/3.0, 2.0],[-2.0, 2.0],[-2.0, 2.0]])
mesh.fill_material(water, [[-2.0, 2.0],[-2.0, -2.0/3.0],[-2.0, 2.0]])
mesh.fill_material(water, [[-2.0, 2.0],[2.0/3.0, 2.0],[-2.0, 2.0]])

# run simulation
monte_carlo.generate_neutron_histories(n_histories=10**6,
        bounds=bounds, mesh=mesh, num_batches=10)

# display plot of neutron flux
index = 1
flux_to_plot = mesh.get_flux()
for i in range(num_groups):
    plotter.plot_heat_map(flux_to_plot[i], index, repeat = 5,
            title = ('Group ' + str(i+1)))
