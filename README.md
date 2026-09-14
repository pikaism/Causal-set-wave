# Causal Set Wave Propagation

A C++ toolkit exploring causal set phenomenology in 1+1D Minkowski spacetime: Poisson sprinkling, the discrete Benincasa-Dowker (BD) d'Alembertian, wave-packet evolution, and comparison with the continuum wave equation.

Built as an independent project demonstrating numerical methods relevant to causal set quantum gravity.

## What this does

1. **Sprinkling** (`causet.cpp`): Poisson-sprinkles N points uniformly at random into a bounded region of 1+1D Minkowski space.
2. **Causal order**: computes the causal order between all pairs via the Minkowski interval condition.
3. **Layers + BD operator**: classifies each element's causal past into layers (L0, L1, L2) by causal-interval cardinality, then applies the discrete Benincasa-Dowker d'Alembertian.
4. **Wave packet** (`wavepacket.cpp`): initializes a Gaussian wave packet on an early time-slice.
5. **Evolution** (`wave_evolution.cpp`): evolves the field forward via an element-by-element retarded solve of the discrete field equation, in causal time-order.
6. **Continuum comparison** (`continuum.cpp`): solves the same wave equation on a regular grid via a standard CFL-stable finite-difference leapfrog scheme, for direct comparison.

## Key results

- The BD operator's convergence to the continuum d'Alembertian is a statement about **ensemble averages over sprinklings**, not single realizations. Validated via 30-trial ensemble averaging (N=500): mean = 0.117, standard error = 1.365, consistent with the theoretical value of 0.
- A naive exact retarded solve for the field evolution is **numerically unstable** (unbounded exponential growth), because it lacks the small stability-controlling parameter present in a standard CFL-constrained leapfrog scheme. Stabilized via layer-averaging + an under-relaxation factor (alpha = 0.3).
- Comparing the discrete evolution against the continuum reference at increasing density (N = 1000, 2000, 4000) shows the gap between them **shrinking monotonically** (2.19 -> 1.97 -> 1.17), consistent with the expected continuum limit.

## Known limitations

- `compute_layers`/`interval_size` scale poorly (roughly O(N^2)-O(N^3) depending on causal set density) and are too slow for very large N or many repeated ensemble trials.
- The damping needed to stabilize the discrete evolution measurably slows its effective propagation speed relative to the continuum, and adds statistical noise.
- Only 1+1D is implemented; 2+1D is a natural extension.

## Build and run

```bash
g++ src/causet.cpp src/wavepacket.cpp src/wave_evolution.cpp src/continuum.cpp src/main.cpp -o causet_test
./causet_test
```

## Repository structure
src/
causet.hpp/cpp - Element, CausalSet (sprinkling, causal order, layers, BD operator)
wavepacket.hpp/cpp - Gaussian wave packet initial data
wave_evolution.hpp/cpp - discrete field evolution
continuum.hpp/cpp - continuum finite-difference reference solver
main.cpp - driver / comparison
data/ - output data
report/ - technical report (physics, methods, results)

