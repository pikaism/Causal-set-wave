#include "causet.hpp"
#include <iostream>
#include <cmath>

double single_trial_constant(int N, double T, double L) {
    CausalSet cs;
    cs.sprinkle(N, T, L);
    cs.compute_causal_order();
    cs.compute_layers(2);

    std::vector<double> phi(N, 1.0);
    double rho = N / (T * 2.0 * L);
    std::vector<double> Bphi = cs.apply_dalembertian(phi, rho);

    double margin_t = 2.0, margin_x = 1.5;
    double sum_interior = 0.0;
    int count_interior = 0;
    for (int i = 0; i < N; i++) {
        double t = cs.elements[i].t;
        double x = cs.elements[i].x;
        if (t > margin_t && t < (T - margin_t) && x > (-L + margin_x) && x < (L - margin_x)) {
            sum_interior += Bphi[i];
            count_interior++;
        }
    }
    return sum_interior / count_interior;
}

int main() {
    int N = 500;         // scaled down from 2000
    double T = 10.0, L = 5.0;
    int num_trials = 30;  // scaled down from 100

    double grand_sum = 0.0;
    double grand_sum_sq = 0.0;
    for (int trial = 0; trial < num_trials; trial++) {
        double val = single_trial_constant(N, T, L);
        grand_sum += val;
        grand_sum_sq += val * val;
        std::cout << "Trial " << trial << " done, val=" << val << std::endl;
    }

    double mean = grand_sum / num_trials;
    double variance = (grand_sum_sq / num_trials) - (mean * mean);
    double stderr_of_mean = std::sqrt(variance / num_trials);

    std::cout << "----" << std::endl;
    std::cout << "N=" << N << ", trials=" << num_trials << std::endl;
    std::cout << "Ensemble average B(phi=1) = " << mean << std::endl;
    std::cout << "Standard error of the mean = " << stderr_of_mean << std::endl;

    return 0;
}
