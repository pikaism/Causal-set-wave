#include "wave_evolution.hpp"
#include <numeric>
#include <algorithm>

void evolve_field(const CausalSet& cs, std::vector<double>& phi,
                   const std::vector<bool>& is_fixed, double rho, double mass,
                   double alpha) {
    int N = cs.elements.size();

    std::vector<int> order(N);
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [&](int a, int b) {
        return cs.elements[a].t < cs.elements[b].t;
    });

    for (int x : order) {
        if (is_fixed[x]) continue;

        auto layer_mean = [&](int k) -> double {
            const auto& layer = cs.layers[x][k];
            if (layer.empty()) return 0.0;
            double sum = 0.0;
            for (int y : layer) sum += phi[y];
            return sum / layer.size();
        };

        double mean_L0 = layer_mean(0);
        double mean_L1 = layer_mean(1);
        double mean_L2 = layer_mean(2);

        double solved_value = 2.0 * mean_L0 - 4.0 * mean_L1 + 2.0 * mean_L2;
        phi[x] = alpha * solved_value;
    }
}
