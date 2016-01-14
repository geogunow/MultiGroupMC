#include "distributions.h"

/**
  @brief    Returns a random number between 0 and a give valule

*/
double random(double num) {
    return (rand() % (int)(num * 100000000) / 100000000.0);
}

/**
  @brief    Function that randomly samples an azimuthal angle                   
  @details  An azimuthal angle is uniformally sampled in [0, 2 pi]              
  @return    A randomly sampled angle in [0, 2pi] 
*/
double sampleAzimuthalAngle() {

    return 2 * M_PI*random(1.0);
}

/**

*/
double samplePolarAngle() {
    double cos_theta = 2*random(1.0) - 1.0;
    return acos(cos_theta);

}

/**

*/
double sampleDistance(Material mat, int group) {
    return -log(random(1.0)) / mat.getSigmaT()[group];
}

/**

*/
int sampleInteraction(Material mat, int group) {
    
    double sum_sigma_s = mat.getSigmaT()[group]-mat.getSigmaA()[group];
    return (int)(random(sum_sigma_s) / mat.getSigmaT()[group]);
}

/**

*/
std::vector <double> sampleLocation(Boundaries bounds) {
    
    //std::vector <double> _dist_location = new std::vector <double>;
    std::vector <double> _dist_location;
    for (int i=0; i<3; ++i) {
       
        /** is this the correct way to sample a rondom location? */
        _dist_location.push_back(bounds.getSurfaceCoord(i, 0) +
            bounds.getSurfaceCoord(i, 1) - bounds.getSurfaceCoord(i, 0)
            * random(1.0));
    }
    return _dist_location;
    //
}

/**

*/
int sampleFission(Material mat, int group) {
    return (int)(random(mat.getSigmaF()[group] / mat.getSigmaA()[group]));
}

int sampleNumFission(Material mat) {
    int lower = (int)(mat.getNu());
    int add = (int)(random(mat.getNu()-lower));
    return lower + add;
}

/**

*/
std::vector <double> sampleFissionSite(Fission fission_bank) {
    int index = rand() % fission_bank.length();
    return fission_bank.location(index);
}

/**

*/
int sampleNeutronEnergyGroup(std::vector <double> chi) {
    double r = random(1.0);
    double chi_sum = 0.0;
    for (int g=0; g<NUM_GROUPS; ++g) {
        chi_sum += chi[g];
        if (r<chi_sum) {
            return g;
        }
    }
    return NUM_GROUPS - 1;
}

/**

*/
int sampleScatteredGroup(double **scattering_matrix, int group) {
    double r = random(1.0) * NUM_GROUPS;
    double scatter_sum = 0.0;
    for (int g=0; g<NUM_GROUPS; ++g) {
        scatter_sum += scattering_matrix[group][g];
        if (r<scatter_sum) {
            return g;
        }
    }
    return NUM_GROUPS - 1;
}
