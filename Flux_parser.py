'''
 @file      flux_parser.py
 @brief     Parses flux data from a text file
            and plots it as a heat map
 @author    Luke Eure
 @date      February 3, 2016
'''
import numpy as np
import matplotlib.pyplot as plt

num_groups = 2;
x_cells = 9;
y_cells = 9;
z_cells = 9;


'''
 @brief     Creates a heat map of the flux data
 @param     flux_data a 3d numpy array containing the flux information
 @param     index the z cell-value at which the heat map is displayed
'''
def plot_heat_map(flux_data, index, repeat = 100, title=''):

    img = flux_data
    lum_img = img[:, :, index]

    nx = len(lum_img)
    ny = len(lum_img[0])

    plot_array = np.zeros( (nx*repeat, ny*repeat) )
    for i in range(nx):
        for ii in range(i*repeat, (i+1)*repeat):
            for j in range(ny):
                for jj in range(j*repeat, (j+1)*repeat):
                    plot_array[ii, jj] = lum_img[j, i]
    plt.imshow(plot_array, origin='lower')
    plt.colorbar()
    plt.title(title)
    plt.show(title)

with open('flux_plot.txt', 'r') as fh:
    flux_file = fh.read().replace('\n', '')
    flux_list = flux_file.split()

    flux_to_plot = np.zeros((num_groups, x_cells,
        y_cells, z_cells))
    for i in range(num_groups):
        for j in range(x_cells):
            for k in range(y_cells):
                for l in range(z_cells):
                    flux_to_plot[i][j][k][l] = \
                            flux_list[i*x_cells*y_cells*z_cells \
                            + j*y_cells*z_cells + k*z_cells + l]

    index = 1
    for i in range(num_groups):
        plot_heat_map(flux_to_plot[i], index, repeat = 5,
                title = ('Group ' + str(i+1)))

