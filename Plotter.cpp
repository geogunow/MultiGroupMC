/*
 @file      Plotter.cpp
 @brief     plotting function
 @author    Luke Eure
 @date      January 28 2016
*/

#include "Plotter.h"

void printFluxToFile(std::vector <std::vector <std::vector <std::vector <double> > > >
       &_flux) {
    
    std::ofstream out("flux_plot.txt");
    
    for (int i=0; i<_flux.size(); ++i) {
        out << "\n";
        for (int j=0; j<_flux[0].size(); ++j) {
            out << "\n";
            for (int k=0; k<_flux[0][0].size(); ++k) {
                out << "\n";
                for (int l=0; l<_flux[0][0][0].size(); ++l) {
                    out << _flux[i][j][k][l] << " ";
                }
            }
        }
    }
    out.close();
}
