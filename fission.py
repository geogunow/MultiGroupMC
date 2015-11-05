'''
 @file      fission.py
 @brief     contains the Fission class
 @author    Luke Eure
 @date      October 18, 2015
'''

import numpy as np

'''
 @class Fission fission.py "fission.py"
 @brief Contains a list of locations where fission events occured
'''
class Fission():
    
    def __init__(self):
        self._fission_locations = list()
    
    def clear(self):
        self._fission_locations = list()

    @property
    def length(self):
        return len(self._fission_locations)

    def next(self):
        item = self._fission_locations[0]
        self._fission_locations.pop(0)
        return item

    # location should be a numpy array
    def add(self, location):
        self._fission_locations.append(location)

    def location(self, num):
        return self._fission_locations[num]
