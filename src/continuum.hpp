#ifndef CONTINUUM_HPP
#define CONTINUUM_HPP

#include "wavepacket.hpp"
#include <vector>

struct ContinuumSnapshot {
    double t;
    double centroid_x;
    double total_energy;
};

std::vector<ContinuumSnapshot> solve_continuum_wave(
    const WavePacketParams& params, double T, double L,
    int Nx, int num_output_bins);

#endif
