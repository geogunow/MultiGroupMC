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
import math
random.seed(0)



'''
 @brief     Function that samples a random location within a 2D bounding box.
 @details   A point is randomly and uniformally sampled in the bounding box             provided in the input.
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


'''
 @brief	    Function that calcultes pi
 @details   The function randomly places points in a 1x1 box using generate_2D_coordinates. It uses the ratio of points that land in a circle with radius one vs the number of points thrown to calculate pi.
 @param     num_darts the number of points to be tested
 @return    The calculated value for pi	


'''

def calculate_pi(num_darts):
	hits = 0
	misses = 0
	for i in xrange(int(num_darts)):	
		x, y = generate_2D_coordinate(-1, 1, -1, 1)
		if math.sqrt(x**2 + y**2) <= 1:
			hits += 1
		else: 
			misses += 1
	my_pi = 4*float(hits)/(float(hits+misses))
	return my_pi


#ask the user how many points should be tested
print "Pi calculator"
print "Pi will be calculated by placing random test points on a 1x1 square with a circle inscribed within it"
print "Around 500000 points are recommended for accuracy to two decimal places"
your_pi =  calculate_pi(raw_input("How many points would you like to test?"))
print "Pi = " + str(your_pi)
