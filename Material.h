/* 
 @file      Material.h
 @brief     contains the material class
 @author    Luke Eure
 @date      January 6 2016
*/

#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <vector>
#include <time.h>
#include <math.h>
#include <stdlib.h>

class Material {
private:

    /** total cross sections */
    std::vector <double> _sigma_t;

    /** fission cross sections */
    std::vector <double> _sigma_f;

    /** initial energy distribution */
    std::vector <double> _chi;

    /** absorption cross sections */
    std::vector <double> _sigma_a;

    /** scattering cross sections */
    std::vector <std::vector <double> > _sigma_s;

    /** average number of neutrons released per fission event */
    double _nu;

    /** summation of sigma_s */
    double _s_sum;

    /** number of energy groups */
    int _num_groups;


    Material(std::vector <double> &sigma_t, 
            std::vector <std::vector <double> > &sigma_s, double nu, 
            std::vector <double> &sigma_f, std::vector <double> &chi);
    virtual ~Material();
    
    double arand();
    double getSigmaT(int group);
    double getSigmaF(int group);
    double getChi(int group);
    double getSigmaA(int group);
    std::vector <double> getSigmaS(int group);
    double getNu();
    int sampleInteraction(int group);
};

#endif
