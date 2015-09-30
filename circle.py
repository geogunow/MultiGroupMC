'''
 @file      circle.py
 @brief     Calculates pi from a simple monte carlo calculation
 @details   Pi is computed by calculating the area of the unit circle. For the
            first assignment a simple square bounding box is used. A point
            coordinate generator is provided. Implement a script that uses
            this coordinate generator to compute the area of the unit circle.
 @author    Geoffrey Gunow
 @date      September 30, 2015
'''

# Import libraries and set random number seed to 0
import random
random.seed(0)



'''
 @brief     Function that samples a random location within a 2D bounding box.
 @details   A point is randomly and uniformally sampled in the bounding box 
            provided in the input.
 @param     x_min the minimum x-coordinate in the bounding box
 @param     x_max the maximum x-coordinate in the bounding box
 @param     y_min the minimum y-coordinate in the bounding box
 @param     y_max the maximum y-coordinate in the bounding box
 @return    A tuple containing the x and y coordinates of the sampled 
            location
'''
def generate_2D_coordinate(x_min, x_max, y_min, y_max):
    
    x = x_min + random.random() * (x_max - x_min)
    y = y_min + random.random() * (y_max - y_min)
    return (x, y)


# Below is an example of how to generate a random 2D coordinate
x, y = generate_2D_coordinate(-1, 1, -1, 1)

