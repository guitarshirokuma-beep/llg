import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np
from matplotlib.colors import LogNorm

def output_normal_graph(
    fontsize,
    title_text,
    xlabel,
    x_data,
    ylabel,
    y_data,
    legend_text,
):
    fig, ax = plt.subplots()
    ax.grid()
    ax.set_title(label=title_text,fontsize=fontsize)
    ax.tick_params(labelsize=fontsize)
    ax.set_xlabel(xlabel, fontsize=fontsize)
    ax.set_ylabel(ylabel, fontsize=fontsize)
    ax.plot(x_data, y_data, label=legend_text)
    ax.legend(fontsize=fontsize)
    plt.show()

def output_heatmap(
        Lx,
        N_steps,
        S,
        dt,
        minimum,
):
    k_size = int(Lx)
    omega_size = int(N_steps)
    S_k_omega = np.array(S).reshape(k_size, omega_size)

    plt.figure()
    # extent = [xmin, xmax, ymin, ymax]
    # here x -> omega (frequency), y -> k
    omega_max = 2 * np.pi / float(dt)
    k_max = 2 * np.pi
    img = plt.imshow(
        S_k_omega,
        cmap='viridis',
        vmin=minimum,
        vmax=S_k_omega.max(),
        aspect='auto',
        origin='lower',
        interpolation='nearest',
        extent=[0, omega_max, 0, k_max]
    )
    cbar = plt.colorbar(img)
    cbar.ax.tick_params(labelsize=15)
    plt.xlabel('omega')
    plt.ylabel('k')
    plt.show()
    # quick diagnostics
    print("shape:", S_k_omega.shape, "min:", S_k_omega.min(), "max:", S_k_omega.max())