#ifndef CAUSET_HPP
#define CAUSET_HPP

#include <vector>

// A single spacetime point in the causal set.
// id is just its index/label; t and x are its Minkowski coordinates.
struct Element {
    int id;
    double t;
    double x;
};

class CausalSet {
public:
    // All the sprinkled elements, in no particular order.
    std::vector<Element> elements;

    // past[i] = list of indices j such that element j causally precedes element i
    // (i.e. j is in the causal past of i). Filled in by compute_causal_order().
    std::vector<std::vector<int>> past;

    // layers[x] = {L0(x), L1(x), L2(x)}: for element x, the indices of its
    // layer-0, layer-1, and layer-2 predecessors. Filled in by compute_layers().
    // layers[x][k] is the list of indices in layer k of element x.
    std::vector<std::vector<std::vector<int>>> layers;

    // Scatter N points uniformly at random into the box [0, T] x [-L, L].
    void sprinkle(int N, double T, double L);

    // For every pair of elements, decide if one causally precedes the other,
    // and fill in the 'past' vector accordingly.
    void compute_causal_order();

    // Small helper: returns true if element i causally precedes element j.
    bool precedes(const Element& i, const Element& j) const;

    // Counts |I(y,x)|: how many elements lie causally between y and x
    // (assumes y precedes x already).
    int interval_size(int y, int x) const;

    // For every element x, classify each past element y into layer 0, 1, or 2
    // based on interval_size(y, x). max_layer caps how many layers we bother computing.
    void compute_layers(int max_layer = 2);
    // Computes the full n-interval spectrum N_m for m=0..max_m: the number of
    // causally-related pairs (y,x) whose causal interval |I(y,x)| equals m.
    // This generalizes compute_layers (which only tracked m=0,1,2 per-element)
    // into the global histogram described in Surya, "A Closeness Function on
    // Coarse Grained Lorentzian Geometries" (arXiv:2510.19403, Oct 2025).
    std::vector<long> compute_interval_spectrum(int max_m) const;

    // Applies the discrete Benincasa-Dowker d'Alembertian to a field phi
    // (one value per element, same ordering as 'elements'), given density rho.
    // Returns (B*phi)(x) for every element x.
    std::vector<double> apply_dalembertian(const std::vector<double>& phi, double rho) const;

    // Prints basic stats (number of elements, number of causal relations found)
    // so we can sanity-check the result.
    void print_summary() const;
};

#endif
