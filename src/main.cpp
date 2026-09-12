#include "causet.hpp"
#include "wavepacket.hpp"
#include <iostream>
#include <algorithm>

int main() {
    CausalSet cs;
    int N = 1000;
    double T = 10.0, L = 5.0;

    cs.sprinkle(N, T, L);
    cs.compute_causal_order();

    WavePacketParams params;
    params.A = 1.0;
    params.x0 = -2.0;   // start the packet on the left side, moving right
    params.sigma = 0.5;
    params.k0 = 3.0;
    params.v = 1.0;

    double t0 = 0.0;
    double dt_slice = 0.3;  // thin strip near t=0

    std::vector<double> phi_level0 = assign_initial_phi(cs, params, t0, dt_slice, 0);

    // Collect (x, phi) pairs for elements actually in the slice, sorted by x,
    // so we can see the packet shape by eye.
    std::vector<std::pair<double,double>> slice_values;
    for (int i = 0; i < N; i++) {
        if (cs.elements[i].t >= t0 && cs.elements[i].t <= t0 + dt_slice) {
            slice_values.push_back({cs.elements[i].x, phi_level0[i]});
        }
    }
    std::sort(slice_values.begin(), slice_values.end());

    std::cout << "Elements in initial slice: " << slice_values.size() << std::endl;
    std::cout << "x, phi(x) -- should peak near x = " << params.x0 << std::endl;
    for (auto& pr : slice_values) {
        std::cout << pr.first << ", " << pr.second << std::endl;
    }

    return 0;
}
