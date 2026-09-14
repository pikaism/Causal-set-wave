import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt("data/interval_spectrum.txt")
m, Nm = data[:,0], data[:,1]

# Drop the last point (the overflow bin) so it doesn't distort the shape --
# it lumps together all m > max_m and isn't part of the real decay curve.
m_plot, Nm_plot = m[:-1], Nm[:-1]

plt.figure(figsize=(7,5))
plt.plot(m_plot, Nm_plot, 'o-', color='tab:green', markersize=4)
plt.xlabel("m (interval size)")
plt.ylabel("N_m (abundance)")
plt.title("n-interval spectrum of the sprinkled causal set\n(cf. Surya, arXiv:2510.19403, Fig. 3)")
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("figures/interval_spectrum.png", dpi=150)
print("Saved figures/interval_spectrum.png")
