#ifndef WAVEPACKET_HPP
#define WAVEPACKET_HPP

#include "causet.hpp"
#include <vector>

struct WavePacketParams {
    double A;
    double x0;
    double sigma;
    double k0;
    double v;
};

double gaussian_wavepacket(double x, const WavePacketParams& p);
double gaussian_wavepacket_dx(double x, const WavePacketParams& p);

std::vector<double> assign_initial_phi(const CausalSet& cs, const WavePacketParams& p,
                                        double t0, double dt_slice, int which_level);

#endif
