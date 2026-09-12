#include "wavepacket.hpp"
#include <cmath>

double gaussian_wavepacket(double x, const WavePacketParams& p) {
    double envelope = std::exp( -std::pow(x - p.x0, 2) / (2.0 * p.sigma * p.sigma) );
    double oscillation = std::cos( p.k0 * (x - p.x0) );
    return p.A * envelope * oscillation;
}

double gaussian_wavepacket_dx(double x, const WavePacketParams& p) {
    double dx = x - p.x0;
    double envelope = std::exp( -(dx * dx) / (2.0 * p.sigma * p.sigma) );
    double oscillation = std::cos(p.k0 * dx);
    double d_envelope = envelope * ( -dx / (p.sigma * p.sigma) );
    double d_oscillation = -p.k0 * std::sin(p.k0 * dx);
    return p.A * ( d_envelope * oscillation + envelope * d_oscillation );
}

std::vector<double> assign_initial_phi(const CausalSet& cs, const WavePacketParams& p,
                                        double t0, double dt_slice, int which_level) {
    int N = cs.elements.size();
    std::vector<double> phi(N, 0.0);

    for (int i = 0; i < N; i++) {
        double t = cs.elements[i].t;
        double x = cs.elements[i].x;

        if (t >= t0 && t <= (t0 + dt_slice)) {
            double phi0 = gaussian_wavepacket(x, p);

            if (which_level == 0) {
                phi[i] = phi0;
            } else {
                double phi_dot0 = -p.v * gaussian_wavepacket_dx(x, p);
                phi[i] = phi0 + dt_slice * phi_dot0;
            }
        }
    }
    return phi;
}
