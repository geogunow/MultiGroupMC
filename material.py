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
    def __init__(self, sigma_t, sigma_s, nu, sigma_f):
        
        # total cross-section
        self._sigma_t = sigma_t

        # scattering cross-section
        self._sigma_s = sigma_s

        # average number of neutrons emitted per fission
        self._nu = nu

        # fission cross-section
        self._sigma_f = sigma_f

        # absorption cross-section
        self._sigma_a = sigma_t - sigma_s


    # functions to allow access to the values stored in Material
    @property
    def __sigma_t__(self):
        return self._sigma_t

    @property
    def __sigma_s__(self):
        return self._sigma_s
   
    @property
    def __nu__(self):
        return self._nu

    @property
    def __sigma_f__(self):
        return self._sigma_f


