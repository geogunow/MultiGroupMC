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
cell_width = 15.0
fuel_width = 5.0
num_cells = 9

# create test materials
fuel = material.Material(sigma_t=[.22222222,.833333333],
        sigma_s=[[0.152222222, .02],[0.0,0.7333333333]],
        nu=2.4, sigma_f=[.222222222,0.8333333333], chi = [1.0, 0.0])
water = material.Material(sigma_t= [.22222222,.833333333],
        sigma_s=[[0.19722222222 , 0.025], [0.0, 1.65666666666]],
        nu=2.4, sigma_f=[0.0, 0.0], chi = [0.0, 0.0])

bounds = boundaries.Boundaries(-cell_width/2, cell_width/2, -cell_width/2, 
        cell_width/2, -cell_width/2, cell_width/2)

mesh = mesh.Mesh(bounds, cell_width/num_cells, cell_width/num_cells,
        cell_width/num_cells, num_groups = num_groups, default_material=None)

# fill mesh with some fuel
fuel_range = [-fuel_width/2, fuel_width/2]
mesh.fill_material(fuel, [fuel_range]*3)

# fill the rest of the mesh with water
cell_range = [-cell_width/2, cell_width/2]
bound_limits = [[-cell_width/2, cell_width/2]]*3
for axis in range(3):
    for side in range(2):
        water_bounds = np.array(bound_limits)
        water_bounds[axis][side] = fuel_range[1-side]
        print "Water", water_bounds
        mesh.fill_material(water, water_bounds)

# run simulation
monte_carlo.generate_neutron_histories(n_histories=10**6, 
        bounds=bounds, mesh=mesh, num_batches=10)

# display plot of neutron flux
index = 5
flux_to_plot = mesh.get_flux()
for i in range(num_groups):
    plotter.plot_heat_map(flux_to_plot[i], index, repeat = 5,
            title = ('Group ' + str(i+1)))
