#include "fission.h"

Fission::Fission() {}

Fission::~Fission() {}
    
void Fission::clear() {
    _fission_locations.clear();
}

int Fission::length() {
    return _fission_locations.size();
}

void Fission::add(double *location) {
    _fission_locations.push_back(location);
}

double* Fission::location(int num) {
    return _fission_locations[num];
}

double* Fission::next() {
    item =_fission_locations[0];
    _fission_locations.erase(_fission_locations.begin());
    return item;
}
