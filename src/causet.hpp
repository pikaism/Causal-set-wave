#ifndef CAUSET_HPP
#define CAUSET_HPP

#include <vector>

struct Element { int id; double t; double x; };

class CausalSet {
public:
    std::vector<Element> elements;
    std::vector<std::vector<int>> past;
    std::vector<std::vector<std::vector<int>>> layers;

    void sprinkle(int N, double T, double L);
    void compute_causal_order();
    bool precedes(const Element& i, const Element& j) const;
    int interval_size(int y, int x) const;
    void compute_layers(int max_layer = 2);
    std::vector<double> apply_dalembertian(const std::vector<double>& phi, double rho) const;
    void print_summary() const;

    // Computes the n-interval spectrum N_m for m = 0..max_m: the number of
    // causally related pairs (y,x) whose causal interval |I(y,x)| equals m.
    // This is the order-invariant "fingerprint" described in Surya,
    // "A Closeness Function on Coarse Grained Lorentzian Geometries"
    // (arXiv:2510.19403, Oct 2025) -- a direct generalization of the
    // layer-counting already used for the BD operator (layers = spectrum
    // capped at m=0,1,2), now computed as a full histogram over all pairs.
    // Any interval with m > max_m is counted in the final bin (index max_m).
    std::vector<long> compute_interval_spectrum(int max_m) const;
};

#endif
