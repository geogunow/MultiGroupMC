'''
 @file      plotter.py
 @brief     Functions for plotting in Monte Carlo applications
 @author    Geoffrey Gunow
 @date      October 7, 2015
'''

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
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
def plot_neutron_flux(3D_matrix_of_flux):


plt.imshow(
x = np.linspace(-1, 1, 100)
y = np.cos(x)

plt.plot(x, y, 'k.-')
plt.xlabel('x')
plt.ylabel('y')
plt.title('Simple Matplotlib Example')
plt.show()
'''

