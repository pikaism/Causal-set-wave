import matplotlib.pyplot as plt
import numpy as np

disc = np.loadtxt("data/discrete_output.txt")
disc_t, disc_x, disc_phi = disc[:,0], disc[:,1], disc[:,2]

cont = np.loadtxt("data/continuum_output.txt")
cont_bin, cont_t, cont_x, cont_phi = cont[:,0], cont[:,1], cont[:,2], cont[:,3]

T = disc_t.max()
num_bins = int(cont_bin.max()) + 1
bin_width = T / num_bins

fig, axes = plt.subplots(2, 3, figsize=(15, 8), sharey=True)
axes = axes.flatten()

for b in range(num_bins):
    ax = axes[b]
    mask_c = cont_bin == b
    xs = cont_x[mask_c]; ps = cont_phi[mask_c]
    order = np.argsort(xs)
    ax.plot(xs[order], ps[order], '-', color='tab:blue', label='Continuum', linewidth=1.5)

    t_lo, t_hi = b*bin_width, (b+1)*bin_width
    mask_d = (disc_t >= t_lo) & (disc_t < t_hi)
    ax.scatter(disc_x[mask_d], disc_phi[mask_d], color='tab:red', s=10, label='Discrete', alpha=0.7)

    ax.set_title(f"t in [{t_lo:.2f}, {t_hi:.2f})")
    ax.set_xlabel("x")
    if b % 3 == 0: ax.set_ylabel("phi")
    ax.legend(fontsize=8)
    ax.axhline(0, color='gray', linewidth=0.5)

plt.suptitle("Wave packet: discrete (causal set) vs continuum, at successive times")
plt.tight_layout()
plt.savefig("data/wavepacket_comparison.png", dpi=150)
print("Saved data/wavepacket_comparison.png")
