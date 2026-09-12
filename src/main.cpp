#include "causet.hpp"
#include "wavepacket.hpp"
#include "wave_evolution.hpp"
#include <iostream>
#include <fstream>
#include <cmath>

int main() {
    CausalSet cs;
    int N = 2000;
    double T = 3.0;
    double L = 3.0;

    cs.sprinkle(N, T, L);
    cs.compute_causal_order();
    cs.compute_layers(2);

    WavePacketParams params;
    params.A = 1.0;
    params.x0 = -1.5;
    params.sigma = 0.4;
    params.k0 = 3.0;
    params.v = 1.0;

    double t0 = 0.0;
    double dt_slice = 0.2;

    std::vector<double> phi = assign_initial_phi(cs, params, t0, dt_slice, 0);

    std::vector<bool> is_fixed(N, false);
    for (int i = 0; i < N; i++) {
        if (cs.elements[i].t >= t0 && cs.elements[i].t <= t0 + dt_slice) {
            is_fixed[i] = true;
        }
    }

    double rho = N / (T * 2.0 * L);
    double alpha = 0.3;

    evolve_field(cs, phi, is_fixed, rho, 0.0, alpha);

    std::ofstream out("data/evolution_output.txt");
    for (int i = 0; i < N; i++) {
        out << cs.elements[i].t << " " << cs.elements[i].x << " " << phi[i] << "\n";
    }
    out.close();

    int num_bins = 6;
    double bin_width = T / num_bins;
    for (int b = 0; b < num_bins; b++) {
        double t_lo = b * bin_width;
        double t_hi = (b + 1) * bin_width;

        double weighted_sum = 0.0;
        double weight_total = 0.0;
        double total_energy = 0.0;

        for (int i = 0; i < N; i++) {
            if (cs.elements[i].t >= t_lo && cs.elements[i].t < t_hi) {
                double energy = phi[i] * phi[i];
                weighted_sum += cs.elements[i].x * energy;
                weight_total += energy;
                total_energy += energy;
            }
        }

        double centroid = (weight_total > 0) ? (weighted_sum / weight_total) : 0.0;
        std::cout << "t in [" << t_lo << ", " << t_hi << "): "
                  << "energy-weighted centroid x = " << centroid
                  << " | total energy in bin = " << total_energy << std::endl;
    }

    return 0;
}
