'''
 @file      tally.py
 @brief     Contains the Tally class
 @author    Luke Eure
 @date      October 13, 2015
'''

import numpy as np

'''
 @class Tally tally.py "tally.py"
 @brief Tally contains tallys of information used in neutron simulation
'''

class Tally():
    def __init__(self):
        self.count = 0
    def increment(self):
        self.count += 1
    def display(self):
        print self.count
