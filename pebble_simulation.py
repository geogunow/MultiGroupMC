'''
 @file      pebble_simulation.py
 @brief     A program to test the simulation of a fuel pebble
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
fuel = material.Material(sigma_t=[.22222222,.833333333],
        sigma_s=[[0.152222222, .02],[0.0,0.7333333333]],
        nu=2.4, sigma_f=[.222222222,0.8333333333], chi = [1.0, 0.0])
water = material.Material(sigma_t= [.22222222,.833333333],
        sigma_s=[[0.19722222222 , 0.025], [0.0, 1.65666666666]],
        nu=2.4, sigma_f=[0.0, 0.0], chi = [0.0, 0.0])

bounds =  boundaries.Boundaries(-7.5, 7.5, -7.5, 7.5, -7.5, 7.5)

mesh = mesh.Mesh(bounds, 5.0/3.0, 5.0/3.0, 5.0/3.0,
        num_groups = num_groups,
        default_material=None)

# fill mesh with some fuel
mesh.fill_material(fuel, [[-2.5, 2.5],[-2.5, 2.5],[-2.5,2.5]])

# fill the rest of the mesh with water
mesh.fill_material(water, [[-7.5, -2.5],[-7.5, 7.5],[-7.5, 7.5]])
mesh.fill_material(water, [[2.5, 7.5],[-7.5, 7.5],[-7.5, 7.5]])
mesh.fill_material(water, [[-7.5, 7.5],[-7.5, -2.5],[-7.5, 7.5]])
mesh.fill_material(water, [[-7.5, 7.5],[2.5, 7.5],[-7.5, 7.5]])
mesh.fill_material(water, [[-7.5, 7.5],[-7.5, 7.5],[-7.5, -2.5]])
mesh.fill_material(water, [[-7.5, 7.5],[-7.5, 7.5],[2.5, 7.5]])

# run simulation
monte_carlo.generate_neutron_histories(n_histories=100000, 
        bounds=bounds, mesh=mesh,
        num_batches=100)

# display plot of neutron flux
index = 5
flux_to_plot = mesh.get_flux()
for i in range(num_groups):
    plotter.plot_heat_map(flux_to_plot[i], index, repeat = 5,
            title = ('Group ' + str(i+1)))
