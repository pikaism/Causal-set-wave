import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt("data/convergence.txt")
N, gap = data[:,0], data[:,1]

plt.figure(figsize=(7,5))
plt.plot(N, gap, 'o-', color='tab:purple')
plt.xlabel("Number of sprinkled points (N)")
plt.ylabel("|discrete centroid - continuum centroid|")
plt.title("Discrete-continuum gap vs sprinkling density")
plt.xscale('log')
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("data/convergence_plot.png", dpi=150)
print("Saved data/convergence_plot.png")
