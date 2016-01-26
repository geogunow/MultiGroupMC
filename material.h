/** 
 @file      material.h
 @brief     contains the material class
 @author    Luke Eure
 @date      January 6 2016
*/

#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <vector>

class Material {
private:
    std::vector <double> _sigma_t;
    std::vector <double> _sigma_f;
    std::vector <double> _chi;
    std::vector <double> _sigma_a;
    std::vector <std::vector <double> > _sigma_s;
    double _nu;
    double _s_sum;
    int _num_groups;

public:
   
    Material();
    Material(std::vector <double> &sigma_t, 
            std::vector <std::vector <double> > &sigma_s, double nu, 
            std::vector <double> &sigma_f, std::vector <double> &chi);
    virtual ~Material();
    
    std::vector <double> getSigmaT();
    std::vector <double> getSigmaF();
    std::vector <double> getChi();
    std::vector <double> getSigmaA();
    std::vector <std::vector <double> > getSigmaS();
    double getNu();

};

#endif
