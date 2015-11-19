'''
 @file      plotter.py
 @brief     Functions for plotting in Monte Carlo applications
 @author    Geoffrey Gunow
 @date      October 7, 2015
'''

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import matplotlib.image as mpimg

'''
 @brief     Creates a scatter plot of 3D points
 @details   Input data referring to point locations is plotted on a 3D graph.
            A new figure is created to plot the data and after plotting, the
            show() function is called, suspending execution.
 @param     xdata the x-coordinates of the data points
 @param     ydata the y-coordinates of the data points
 @param     zdata the z-coordinates of the data points
'''
def plot_3D_points(xdata, ydata, zdata):
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(xdata, ydata, zdata, s=10, c='k')
    plt.show()

'''
 @brief     Creates a heat map of the flux data
 @param     flux_data a 3d numpy array containing the flux information
 @param     index the z cell-value at which the heat map is displayed
'''
def plot_heat_map(flux_data, index):

    img = flux_data
    lum_img = img[:, :, index]
    
    
    '''    
    nx = len(lum_img)
    ny = len(lum_img[0])

    repeat = 100
    plot_array = np.zeros( (nx*repeat, ny*repeat) )
    for i in range(nx):
        for ii in range(i*repeat, (i+1)*repeat):
            for j in range(ny):
                for jj in range(j*repeat, (j+1)*repeat):
                    plot_array[ii, jj] = lum_img[i, j]
    '''
    plt.imshow(lum_img, origin='lower')
    plt.colorbar()
    plt.show()
