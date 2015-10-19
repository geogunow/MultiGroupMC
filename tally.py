'''
 @file      tally.py
 @brief     Contains the Tally class
 @author    Luke Eure
 @date      October 13, 2015
'''

import numpy as np

'''
 @class Tally tally.py "tally.py"
 @brief Tally contains tallies of information used in neutron simulation
'''

class Tally():
    def __init__(self):
        self.count = 0
    def increment(self, amt):
        self.count += amt
    def clear(self):
        self.count = 0
    @property
    def amt(self):
        return float(self.count)
