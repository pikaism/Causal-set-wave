#include "continuum.hpp"
#include <cmath>
#include <iostream>

std::vector<ContinuumSnapshot> solve_continuum_wave(
    const WavePacketParams& params, double T, double L,
    int Nx, int num_output_bins) {

    double dx = (2.0 * L) / (Nx - 1);
    double dt = 0.5 * dx;
    int Nt = (int)(T / dt);

    std::vector<double> x_grid(Nx);
    for (int i = 0; i < Nx; i++) {
        x_grid[i] = -L + i * dx;
    }

    std::vector<double> phi_old(Nx), phi_now(Nx), phi_new(Nx);

    for (int i = 0; i < Nx; i++) {
        phi_old[i] = gaussian_wavepacket(x_grid[i], params);
    }
    phi_old[0] = 0.0; phi_old[Nx - 1] = 0.0;

    for (int i = 0; i < Nx; i++) {
        double phi_dot0 = -params.v * gaussian_wavepacket_dx(x_grid[i], params);
        phi_now[i] = phi_old[i] + dt * phi_dot0;
    }
    phi_now[0] = 0.0; phi_now[Nx - 1] = 0.0;

    double r2 = (dt / dx) * (dt / dx);

    std::vector<ContinuumSnapshot> snapshots;
    double bin_width_steps = (double)Nt / num_output_bins;
    int next_snapshot_bin = 0;

    for (int step = 0; step <= Nt; step++) {
        double current_t = step * dt;

        if (step >= next_snapshot_bin * bin_width_steps && next_snapshot_bin < num_output_bins) {
            double weighted_sum = 0.0, weight_total = 0.0, total_energy = 0.0;
            for (int i = 0; i < Nx; i++) {
                double energy = phi_now[i] * phi_now[i];
                weighted_sum += x_grid[i] * energy;
                weight_total += energy;
                total_energy += energy;
            }
            double centroid = (weight_total > 0) ? (weighted_sum / weight_total) : 0.0;
            snapshots.push_back({current_t, centroid, total_energy});
            next_snapshot_bin++;
        }

        if (step == Nt) break;

        for (int i = 1; i < Nx - 1; i++) {
            phi_new[i] = 2.0 * phi_now[i] - phi_old[i]
                       + r2 * (phi_now[i + 1] - 2.0 * phi_now[i] + phi_now[i - 1]);
        }
        phi_new[0] = 0.0;
        phi_new[Nx - 1] = 0.0;

        phi_old = phi_now;
        phi_now = phi_new;
    }

    return snapshots;
}
