#include "causet.hpp"
#include <iostream>
#include <fstream>

int main() {
    CausalSet cs;
    int N = 2000;
    double T = 10.0, L = 5.0;
    int max_m = 300;

    cs.sprinkle(N, T, L);
    cs.compute_causal_order();

    std::vector<long> Nm = cs.compute_interval_spectrum(max_m);

    std::ofstream out("data/interval_spectrum.txt");
    for (int m = 0; m <= max_m; m++) {
        out << m << " " << Nm[m] << "\n";
    }
    out.close();

    std::cout << "N=" << N << ", wrote data/interval_spectrum.txt (m=0.." << max_m << ")" << std::endl;
    return 0;
}
