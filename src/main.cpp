#include "causet.hpp"
#include "wavepacket.hpp"
#include "wave_evolution.hpp"
#include "continuum.hpp"
#include <iostream>
#include <cmath>

double run_discrete_trial(int N, double T, double L, const WavePacketParams& params,
                           double t0, double dt_slice, double alpha, int target_bin, int num_bins) {
    CausalSet cs;
    cs.sprinkle(N, T, L);
    cs.compute_causal_order();
    cs.compute_layers(2);

    std::vector<double> phi = assign_initial_phi(cs, params, t0, dt_slice, 0);
    std::vector<bool> is_fixed(N, false);
    for (int i = 0; i < N; i++) {
        if (cs.elements[i].t >= t0 && cs.elements[i].t <= t0 + dt_slice) is_fixed[i] = true;
    }
    double rho = N / (T * 2.0 * L);
    evolve_field(cs, phi, is_fixed, rho, 0.0, alpha);

    double bin_width = T / num_bins;
    double t_lo = target_bin * bin_width, t_hi = (target_bin + 1) * bin_width;
    double weighted_sum = 0.0, weight_total = 0.0;
    for (int i = 0; i < N; i++) {
        if (cs.elements[i].t >= t_lo && cs.elements[i].t < t_hi) {
            double e = phi[i] * phi[i];
            weighted_sum += cs.elements[i].x * e;
            weight_total += e;
        }
    }
    return (weight_total > 0) ? (weighted_sum / weight_total) : 0.0;
}

int main() {
    double T = 3.0, L = 3.0;
    WavePacketParams params;
    params.A = 1.0; params.x0 = -1.5; params.sigma = 0.4; params.k0 = 3.0; params.v = 1.0;

    int num_bins = 6;
    int last_bin = num_bins - 1;  // compare at the final time bin

    std::vector<ContinuumSnapshot> cont = solve_continuum_wave(params, T, L, 300, num_bins);
    double cont_centroid = cont[last_bin].centroid_x;

    std::cout << "Continuum centroid at final bin: " << cont_centroid << std::endl;
    std::cout << "---" << std::endl;

    for (int N : {1000, 2000, 4000}) {
        double disc_centroid = run_discrete_trial(N, T, L, params, 0.0, 0.2, 0.3, last_bin, num_bins);
        double gap = std::abs(disc_centroid - cont_centroid);
        std::cout << "N=" << N << ": discrete centroid=" << disc_centroid
                  << ", |gap to continuum|=" << gap << std::endl;
    }

    return 0;
}
