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

test_mesh = mesh.Mesh(bounds, .1, .1, .1)

test_material = material.Material(sigma_t=2.0, sigma_s=1.0, nu=2.4, sigma_f=.5)

monte_carlo.generate_neutron_histories(n_histories=100000, mat=test_material, 
        bounds=bounds, mesh=test_mesh, num_batches=1)

index = 9
plotter.plot_heat_map(test_mesh.get_flux(), index, repeat = 50)
