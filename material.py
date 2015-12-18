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
    def __init__(self, sigma_t, sigma_s, nu, sigma_f, chi):
        
        # total cross-section
        self._sigma_t = sigma_t

        # scattering cross-section
        self._sigma_s = sigma_s

        # average number of neutrons emitted per fission
        self._nu = nu

        # fission cross-section
        self._sigma_f = sigma_f
        
        # chi
        self._chi = chi
        
        # absorption cross-section
        self._sigma_a = list()
        for i in range(len(sigma_t)):
            self._sigma_a.append(sigma_t[i] - sum(sigma_s[i]))

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

    @property
    def chi(self):
        return self._chi
