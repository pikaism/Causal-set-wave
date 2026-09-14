#include "causet.hpp"
#include <random>
#include <iostream>
#include <cmath>

void CausalSet::sprinkle(int N, double T, double L) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> t_dist(0.0, T);
    std::uniform_real_distribution<double> x_dist(-L, L);

    elements.clear();
    for (int i = 0; i < N; i++) {
        Element e;
        e.id = i;
        e.t = t_dist(rng);
        e.x = x_dist(rng);
        elements.push_back(e);
    }
}

bool CausalSet::precedes(const Element& i, const Element& j) const {
    double dt = j.t - i.t;
    double dx = j.x - i.x;
    double interval = dt * dt - dx * dx;
    return (interval >= 0.0) && (dt > 0.0);
}

void CausalSet::compute_causal_order() {
    int N = elements.size();
    past.assign(N, std::vector<int>());

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j) continue;
            if (precedes(elements[i], elements[j])) {
                past[j].push_back(i);
            }
        }
    }
}

int CausalSet::interval_size(int y, int x) const {
    int count = 0;
    for (int z : past[x]) {
        if (z == y) continue;
        if (precedes(elements[y], elements[z])) {
            count++;
        }
    }
    return count;
}

void CausalSet::compute_layers(int max_layer) {
    int N = elements.size();
    layers.assign(N, std::vector<std::vector<int>>(max_layer + 1));

    for (int x = 0; x < N; x++) {
        for (int y : past[x]) {
            int k = interval_size(y, x);
            if (k <= max_layer) {
                layers[x][k].push_back(y);
            }
        }
    }
}

std::vector<double> CausalSet::apply_dalembertian(const std::vector<double>& phi, double rho) const {
    int N = elements.size();
    std::vector<double> result(N, 0.0);
    double l2 = 1.0 / rho;

    for (int x = 0; x < N; x++) {
        double sum_L0 = 0.0, sum_L1 = 0.0, sum_L2 = 0.0;
        for (int y : layers[x][0]) sum_L0 += phi[y];
        for (int y : layers[x][1]) sum_L1 += phi[y];
        for (int y : layers[x][2]) sum_L2 += phi[y];

        result[x] = (1.0 / l2) * ( -2.0 * phi[x]
                                    + 4.0 * sum_L0
                                    - 8.0 * sum_L1
                                    + 4.0 * sum_L2 );
    }
    return result;
}

void CausalSet::print_summary() const {
    int N = elements.size();
    long total_relations = 0;
    for (const auto& p : past) {
        total_relations += p.size();
    }
    std::cout << "Causal set with " << N << " elements." << std::endl;
    std::cout << "Total causal relations found: " << total_relations << std::endl;
    long max_possible = (long)N * (N - 1) / 2;
    std::cout << "(out of a max possible " << max_possible << " pairs)" << std::endl;
}

std::vector<long> CausalSet::compute_interval_spectrum(int max_m) const {
    int N = elements.size();
    std::vector<long> Nm(max_m + 1, 0);

    for (int x = 0; x < N; x++) {
        for (int y : past[x]) {
            int m = interval_size(y, x);
            if (m > max_m) m = max_m;  // overflow bin
            Nm[m]++;
        }
    }
    return Nm;
}
