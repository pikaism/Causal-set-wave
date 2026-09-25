# Causal Set Wave

**A C++ toolkit for simulating scalar field propagation on a discrete causal set — and testing how well it approximates the continuum wave equation.**

![Wave packet: discrete vs continuum](figures/wavepacket_comparison.png)

Built as an independent numerical physics project exploring causal set quantum gravity: sprinkling, causal order, the discrete Benincasa-Dowker d'Alembertian, wave-packet evolution, and direct comparison against a continuum finite-difference solution.

Two real numerical issues were found and fixed along the way (not hidden): the BD operator's large single-realization variance, and a genuine instability in the naive field evolution scheme. Both are documented and validated in the [full report](report/REPORT.pdf).

## What it does

1. **Sprinkles** N points uniformly at random (Poisson process) into a bounded region of 1+1D Minkowski spacetime.
2. **Computes the causal order** between every pair of points via the Minkowski interval condition.
3. **Classifies causal layers** (L0, L1, L2) and applies the discrete **Benincasa-Dowker d'Alembertian**.
4. **Initializes a Gaussian wave packet** on an early time-slice.
5. **Evolves the field** forward via a retarded solve over the causal order — stabilized with layer-averaging and damping after a naive exact solve was found to be numerically unstable.
6. **Compares against a continuum reference solution** (a standard CFL-stable leapfrog finite-difference scheme).
7. **Computes the n-interval spectrum**, following [Surya (2025)](https://arxiv.org/abs/2510.19403), as an independent check that the sprinkling is genuinely continuumlike.

## Key results

| | |
|---|---|
| **BD operator validation** | Ensemble-averaged over 30 sprinklings: mean = 0.117, std. error = 1.365 — consistent with the theoretical value of 0 |
| **Instability found & fixed** | Naive evolution diverged to O(10^25); stabilized via layer-averaging + damping (&alpha;=0.3) |
| **Convergence with density** | Discrete-continuum gap shrinks as N increases (500 &rarr; 4000) |
| **n-interval spectrum** | Matches the monotonically decreasing "continuumlike" signature from Surya (2025) |

![Convergence with density](figures/convergence_plot.png)
![n-interval spectrum](figures/interval_spectrum.png)

## Build and run

```bash
g++ -O2 src/causet.cpp src/wavepacket.cpp src/wave_evolution.cpp src/continuum.cpp src/main.cpp -o causet_test
./causet_test
```

For the n-interval spectrum specifically:
```bash
g++ -O2 src/causet.cpp src/spectrum_main.cpp -o spectrum_test
./spectrum_test
```

Plots are generated from the output data via the scripts in `scripts/` (requires `matplotlib`):
```bash
pip install matplotlib --break-system-packages
python3 scripts/plot_wavepacket.py
python3 scripts/plot_convergence.py
python3 scripts/plot_interval_spectrum.py
```

## Repository structure
src/
causet.hpp/cpp - Element, CausalSet: sprinkling, causal order, layers, BD operator, interval spectrum
wavepacket.hpp/cpp - Gaussian wave packet initial data
wave_evolution.hpp/cpp - discrete field evolution (retarded solve, damped)
continuum.hpp/cpp - continuum finite-difference reference solver
main.cpp - driver: runs discrete + continuum evolution, writes comparison data
spectrum_main.cpp - driver: computes and writes the n-interval spectrum
figures/ - generated plots
data/ - raw output data (t, x, phi; convergence; spectrum)
scripts/ - Python plotting scripts and PDF report builder
report/REPORT.pdf - full technical report: methods, results, limitations, references


## Full report

See [`report/REPORT.pdf`](report/REPORT.pdf) for the complete writeup, including methods, all results, known limitations, and references.

## References

- Bombelli, Lee, Meyer, Sorkin, "Space-Time as a Causal Set," Phys. Rev. Lett. 59, 521 (1987)
- Benincasa, Dowker, "The scalar curvature of a causal set," Phys. Rev. Lett. 104, 181301 (2010)
- Surya, "A Closeness Function on Coarse Grained Lorentzian Geometries," [arXiv:2510.19403](https://arxiv.org/abs/2510.19403) (2025)

---
*Shivangi Gupta*
