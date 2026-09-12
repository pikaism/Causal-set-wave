#include "causet.hpp"
#include <iostream>

int main() {
    CausalSet cs;

    int N = 200;      // number of sprinkled points
    double T = 10.0;  // time extent of the box: t in [0, T]
    double L = 5.0;   // spatial extent: x in [-L, L]

    cs.sprinkle(N, T, L);
    std::cout << "Sprinkled " << cs.elements.size() << " points." << std::endl;

    cs.compute_causal_order();
    cs.print_summary();

    return 0;
}
