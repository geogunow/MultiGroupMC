'''
 @file      assignment1.py
 @brief     Script for monte carlo assignment 1
 @details   This file imports monte carlo functions from monte_carlo.py
            and uses them to complete monte carlo assignment 1.
 @author    Geoffrey Gunow
 @date      September 30, 2015
'''



from monte_carlo import *

'''
 @brief     Function that plots directions on a unit sphere given polar and
            azimuthal angle samplers.
 @details   Points are sampled on a unit sphere by sampling azimuthal and polar
            angles. Sampled points are stored in python lists. 
            The plot_3D_points function is called to plot the points.
 @param     azimuthal_sampler a function to randomly sample an azimuthal angle
 @param     polar_sampler a function to randomly smaple a polar anlge
 @param     num_points the number of points to generate and plot
'''
def plot_unit_sphere(azimuthal_sampler, polar_sampler, num_points):
    
    xdata = list()
    ydata = list()
    zdata = list()
    for i in range(num_points):
        phi = azimuthal_sampler()
        theta = polar_sampler()
        x = sin(theta) * cos(phi)
        y = sin(theta) * sin(phi)
        z = cos(theta)
        xdata.append(x)
        ydata.append(y)
        zdata.append(z)
    
    plot_3D_points(xdata, ydata, zdata)

# Script starts here
#plot_unit_sphere(sample_azimuthal_angle, sample_polar_angle, 2000)
#plot_unit_sphere(sample_azimuthal_angle, sample_incorrect_polar_angle, 2000)

# TODO: uncomment to run monte carlo code:
generate_neutron_histories(10000, 2, 1, -1, 1, -1, 1, -1, 1)
