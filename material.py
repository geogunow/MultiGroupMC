'''
 @file      material.py
 @brief     Contains the Material class
 @author    Geoffrey Gunow
 @date      October 7, 2015
'''

'''
 @class     Material material.py "material.py"
 @brief     The Material class represents a unique material and its relevant
            nulcear data (i.e. cross-sections) for neutron transport.
'''
class Material:
    def __init__(self, sigma_t=0, sigma_s=0, nu=0, sigma_f=0):
        
        # total cross-section
        self._sigma_t = sigma_t

        # scattering cross-section
        self._sigma_s = sigma_s

        # average number of neutrons emitted per fission
        self._nu = nu

        # fission cross-section
        self._sigma_f = sigma_f

    # functions to allow access to the values stored in Material
    @property
    def sigma_t(self):
        return self._sigma_t

    @property
    def sigma_s(self):
        return self._sigma_s
   
    @property
    def nu(self):
        return self._nu

    @property
    def sigma_f(self):
        return self._sigma_f

    @property
    def sigma_a(self):
        return self._sigma_a
