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
    h_app_norm,
    J,
):
    k_size = int(Lx)
    omega_size = int(N_steps)
    # original layout: rows = k, cols = omega
    S_omega_k = np.array(S).reshape(k_size, omega_size)
    # swap axes so rows = omega, cols = k
    S_k_omega = S_omega_k.T

    plt.figure()
    omega_max = 2 * np.pi / float(dt)
    k_max = 2 * np.pi

    data_max = S_k_omega.max()
    data_min = S_k_omega.min()

    if data_min <= 0.0:
        print("Data contains non-positive values.")
    norm = LogNorm(
        vmin=float(data_min),
        vmax=float(data_max)
        )
    img = plt.imshow(
        S_k_omega,
        cmap='viridis',
        norm=norm,
        aspect='auto',
        origin='lower',
        interpolation='nearest',
        extent=[0, k_max, 0, omega_max]
    )
    cbar = plt.colorbar(img)
    cbar.ax.tick_params(labelsize=15)
    plt.xlabel('k')
    plt.ylabel(r'$\omega$')
    #ideal plot
    k_array = np.linspace(0, k_max, Lx)
    ideal_array = h_app_norm + 2*J*( 1-np.cos(k_array) )
    plt.plot(k_array, ideal_array, linestyle='--', c='ghostwhite', lw=1, label="Ideal Line")

    plt.legend()
    plt.show()
    # quick diagnostics
    print("shape (k,omega):", S_k_omega.shape, "min:", S_k_omega.min(), "max:", S_k_omega.max())


