#ifndef WAVE_EVOLUTION_HPP
#define WAVE_EVOLUTION_HPP

#include "causet.hpp"
#include <vector>

// alpha: damping/under-relaxation factor (0 < alpha <= 1). Smaller = more stable,
// but further from the exact discrete field equation. We tune this empirically.
void evolve_field(const CausalSet& cs, std::vector<double>& phi,
                   const std::vector<bool>& is_fixed, double rho, double mass = 0.0,
                   double alpha = 1.0);

#endif
