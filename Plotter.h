/*
 @file      Plotter.h
 @brief     header file for plotter.cpp
 @author    Luke Eure
 @date      January 28 2016
*/

#ifndef PLOTTER_H
#define PLOTTER_H

#include <fstream>
#include <vector>

// function declarations
void plotFlux(std::vector <std::vector <std::vector <std::vector <double> > > >
        &_flux);
void plotFluxb(std::vector <std::vector <std::vector <std::vector <double> > > >
        &_flux);

#endif
