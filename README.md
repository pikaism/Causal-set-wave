# Causal Set Wave

A C++ toolkit for simulating scalar field propagation on a discrete
causal set in 1+1D Minkowski spacetime, validated against the
continuum wave equation.

**Status:** Complete. See `report/REPORT.pdf` for the full writeup,
including methods, all results, and known limitations.

## Goal

Implement and validate the core kinematical and dynamical ingredients
of causal set quantum gravity — Poisson-style sprinkling, the causal
order, the discrete Benincasa-Dowker (BD) d'Alembertian, and a scalar
field evolution scheme — and test them against the known continuum
limit. This is a first working numerical study, not a claim of novel
physics: the value of this project is a working pipeline, honest
validation (including two real numerical issues that were found,
diagnosed, and fixed rather than hidden), and a foundation for further
numerical work in this area.

## Key findings

- The discrete Benincasa-Dowker operator's convergence to the
  continuum d'Alembertian is a statement about the *ensemble average*
  over many sprinklings, not any single realization. Confirmed via
  ensemble averaging over 30 independent sprinklings (N=500): mean =
  0.117, standard error = 1.365, consistent with the theoretical
  value of 0.
- A naive exact solve of the discrete field evolution equation was
  numerically unstable, with the field magnitude growing
  exponentially (O(1) to O(10^25)) over a handful of time bins. This
  was diagnosed as a structural absence of a CFL-like stability
  parameter, and fixed via layer-averaging plus an under-relaxation
  factor (&alpha;=0.3), at the acknowledged cost of no longer solving
  the exact discrete equation.
- Comparing discrete and continuum energy-weighted centroids at
  increasing sprinkling density (N = 500, 1000, 2000, 4000) shows the
  gap between them trending downward, a qualitative indication of
  convergence to the continuum limit.
- The computed n-interval spectrum N_m decreases smoothly and
  monotonically with m, matching the "continuumlike" signature
  described in Surya, arXiv:2510.19403 (2025), for a causal diamond
  in Minkowski spacetime.

## What this project contains

- **Sprinkling and causal order**: N points sampled uniformly in a
  bounded Minkowski region, with the causal order between every pair
  computed via the Minkowski interval condition.
- **Discrete d'Alembertian**: causal layers L0/L1/L2 classified by
  interval cardinality, and the Benincasa-Dowker operator applied and
  statistically validated.
- **Wave packet evolution**: a Gaussian wave packet initialized on an
  early time-slice and evolved forward via a retarded, stabilized
  solve of the discrete field equation.
- **Continuum reference solution**: a standard CFL-stable
  finite-difference leapfrog solver for the same wave equation, used
  for direct comparison.
- **n-interval spectrum**: an independent order-invariant diagnostic
  of continuumlike behavior, following current causal set research.

## Definitions

- **Sprinkling**: a set of points generated uniformly at random in a
  spacetime region, the standard way of building a discrete
  approximation to a continuum causal spacetime.
- **Causal order (i &#8804; j)**: i causally precedes j iff t_j &gt;
  t_i and (t_j - t_i)^2 - (x_j - x_i)^2 &#8805; 0.
- **Causal interval I(y,x)**: the set of elements z with y &#8804; z
  &#8804; x; its cardinality m places y in layer L_m(x).
- **Benincasa-Dowker (BD) d'Alembertian**: a nonlocal, order-based
  discrete operator whose ensemble-averaged action on a sprinkled
  causal set approximates the continuum wave operator.
- **Damping factor (&alpha;)**: an under-relaxation parameter applied
  to the discrete evolution scheme to keep it numerically stable,
  at the cost of no longer solving the exact BD field equation.
- **n-interval spectrum (N_m)**: the abundance of causal intervals of
  cardinality m across the whole causal set, used as an
  order-invariant "fingerprint" of a spacetime region.

## Structure

src/
causet.hpp/cpp - Element, CausalSet: sprinkling, causal order, layers, BD operator, interval spectrum
wavepacket.hpp/cpp - Gaussian wave packet initial data
wave_evolution.hpp/cpp - discrete field evolution (retarded solve, damped)
continuum.hpp/cpp - continuum finite-difference reference solver
main.cpp - driver: runs discrete + continuum evolution, writes comparison data
spectrum_main.cpp - driver: computes and writes the n-interval spectrum
figures/ - final figures (see below)
data/ - raw output data (t, x, phi; convergence; spectrum)
scripts/ - Python plotting scripts and PDF report builder
report/ - full technical report (REPORT.pdf)


## Figures

**Wave Packet: Discrete vs Continuum**

![Wave packet comparison](figures/wavepacket_comparison.png)

**Convergence With Density**

![Convergence plot](figures/convergence_plot.png)

**N-Interval Spectrum**

![Interval spectrum](figures/interval_spectrum.png)

## Reproducibility

1. Build the core simulation:

g++ -O2 src/causet.cpp src/wavepacket.cpp src/wave_evolution.cpp src/continuum.cpp src/main.cpp -o causet_test
./causet_test

2. Build the n-interval spectrum driver:

g++ -O2 src/causet.cpp src/spectrum_main.cpp -o spectrum_test
./spectrum_test

3. Install plotting dependencies and generate figures:

pip install matplotlib --break-system-packages
python3 scripts/plot_wavepacket.py
python3 scripts/plot_convergence.py
python3 scripts/plot_interval_spectrum.py

4. Rebuild the PDF report:

pip install reportlab --break-system-packages
python3 scripts/build_report.py


Developed and run on g++ (Ubuntu 15.2.0) under WSL/Ubuntu, with
Python 3 and matplotlib for plotting.

## Data

No external data is used. All causal sets are generated in-code via
sprinkling; no data files are downloaded or required.

## References

- Bombelli, Lee, Meyer, Sorkin, "Space-Time as a Causal Set," Phys.
  Rev. Lett. 59, 521 (1987)
- Benincasa, Dowker, "The scalar curvature of a causal set," Phys.
  Rev. Lett. 104, 181301 (2010)
- Surya, "A Closeness Function on Coarse Grained Lorentzian
  Geometries," arXiv:2510.19403 (2025)

---
*Shivangi Gupta*
