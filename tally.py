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
        self._count = 0
    def add(self, amt):
        self._count += amt
    def clear(self):
        self._count = 0
    
    @property
    def count(self):
        return float(self._count)
