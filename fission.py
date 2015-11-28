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
    
    '''
     @brief clears the fission sites
    '''
    def clear(self):
        self._fission_locations = list()

    @property
    def length(self):
        return len(self._fission_locations)

    '''
     @brief returns the first item in the fission_locations list then deletes
            that item
    '''
    def next(self):
        item = self._fission_locations[0]
        self._fission_locations.pop(0)
        return item
 
    '''
     @brief adds a location to the fission_locations list
     @param location a numpy array of the location to be added
    '''
    def add(self, location):
        self._fission_locations.append(location)


    '''
     @brief returns the location in fission_locations with index num
    '''
    def location(self, num):
        return self._fission_locations[num]
