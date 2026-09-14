#include "causet.hpp"
#include "wavepacket.hpp"
#include "wave_evolution.hpp"
#include "continuum.hpp"
#include <iostream>
#include <fstream>
#include <cmath>

double run_discrete_and_dump(int N, double T, double L, const WavePacketParams& params,
                              double t0, double dt_slice, double alpha, bool write_full) {
    CausalSet cs;
    cs.sprinkle(N, T, L);
    cs.compute_causal_order();
    cs.compute_layers(2);

    std::vector<double> phi = assign_initial_phi(cs, params, t0, dt_slice, 0);
    std::vector<bool> is_fixed(N, false);
    for (int i = 0; i < N; i++)
        if (cs.elements[i].t >= t0 && cs.elements[i].t <= t0 + dt_slice) is_fixed[i] = true;

    double rho = N / (T * 2.0 * L);
    evolve_field(cs, phi, is_fixed, rho, 0.0, alpha);

    if (write_full) {
        std::ofstream out("data/discrete_output.txt");
        for (int i = 0; i < N; i++)
            out << cs.elements[i].t << " " << cs.elements[i].x << " " << phi[i] << "\n";
        out.close();
    }

    int num_bins = 6;
    double bin_width = T / num_bins;
    double t_lo = (num_bins-1) * bin_width, t_hi = T;
    double wsum=0, wtot=0;
    for (int i = 0; i < N; i++) {
        if (cs.elements[i].t >= t_lo && cs.elements[i].t < t_hi) {
            double e = phi[i]*phi[i];
            wsum += cs.elements[i].x * e;
            wtot += e;
        }
    }
    return (wtot>0) ? (wsum/wtot) : 0.0;
}

int main() {
    double T = 3.0, L = 3.0;
    WavePacketParams params;
    params.A = 1.0; params.x0 = -1.5; params.sigma = 0.4; params.k0 = 3.0; params.v = 1.0;
    double t0 = 0.0, dt_slice = 0.2, alpha = 0.3;
    int num_bins = 6;

    run_discrete_and_dump(3000, T, L, params, t0, dt_slice, alpha, true);
    std::vector<ContinuumSnapshot> cont = solve_continuum_wave(params, T, L, 300, num_bins);
    double cont_centroid_final = cont[num_bins-1].centroid_x;

    std::ofstream conv("data/convergence.txt");
    int trials_per_N = 5;
    for (int N : {500, 1000, 2000, 4000}) {
        double gap_sum = 0.0;
        for (int trial = 0; trial < trials_per_N; trial++) {
            double disc_centroid = run_discrete_and_dump(N, T, L, params, t0, dt_slice, alpha, false);
            gap_sum += std::abs(disc_centroid - cont_centroid_final);
        }
        double avg_gap = gap_sum / trials_per_N;
        conv << N << " " << avg_gap << "\n";
        std::cout << "N=" << N << " avg gap over " << trials_per_N << " trials = " << avg_gap << std::endl;
    }
    conv.close();

    std::cout << "Wrote data/discrete_output.txt, data/continuum_output.txt, data/convergence.txt" << std::endl;
    return 0;
}
