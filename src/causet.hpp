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

    // Scatter N points uniformly at random into the box [0, T] x [-L, L].
    void sprinkle(int N, double T, double L);

    // For every pair of elements, decide if one causally precedes the other,
    // and fill in the 'past' vector accordingly.
    void compute_causal_order();

    // Small helper: returns true if element i causally precedes element j.
    bool precedes(const Element& i, const Element& j) const;

    // Prints basic stats (number of elements, number of causal relations found)
    // so we can sanity-check the result.
    void print_summary() const;
};

#endif