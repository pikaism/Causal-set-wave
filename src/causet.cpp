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
