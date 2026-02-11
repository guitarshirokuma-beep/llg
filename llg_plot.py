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
    run_dir,
):
    fig, ax = plt.subplots()
    ax.grid()
    ax.set_title(label=title_text,fontsize=fontsize)
    ax.tick_params(labelsize=fontsize)
    ax.set_xlabel(xlabel, fontsize=fontsize)
    ax.set_xlim(0, 50)
    ax.set_ylabel(ylabel, fontsize=fontsize)
    ax.plot(x_data, y_data, label=legend_text)
    ax.legend(fontsize=fontsize)
    plt.savefig(f"{run_dir}/normal_graph.png", dpi=300)
    plt.savefig(f"{run_dir}/normal_graph.pdf")
    plt.show()

def output_heatmap(
    title_text,
    Lx,
    N_steps,
    S,
    dt,
    h_app_static,
    J,
    vmin_self,
    xlabel,
    ylabel,
    fontsize,
    clabel,
    run_dir,
):
    k_size = int(Lx)
    omega_size = int(N_steps)
    # original layout: rows = k, cols = omega
    S_omega_k = np.array(S).reshape(k_size, omega_size)
    # swap axes so rows = omega, cols = k
    S_k_omega = S_omega_k.T

    plt.figure()
    plt.grid(color='ghostwhite', linewidth=0.5)
    omega_max = 2 * np.pi / float(dt)
    k_max = 2

    data_max = S_k_omega.max()
    data_min = S_k_omega.min()

    if data_min <= 0.0:
        print("Data contains non-positive values.")
    norm = LogNorm(
        vmin=vmin_self,
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
    cbar.ax.tick_params(labelsize=fontsize)
    cbar.set_label(clabel, fontsize=fontsize)
    plt.xlabel(xlabel, fontsize=fontsize)
    plt.ylabel(ylabel, fontsize=fontsize)
    ymax_self = omega_max/10
    plt.ylim(h_app_static-10, h_app_static+60)
    #ideal plot
    k_array = np.linspace(0, k_max, Lx)
    ideal_array = h_app_static + 2*J*( 1-np.cos(k_array*np.pi) )
    plt.plot(k_array, ideal_array, linestyle='--', c='ghostwhite', lw=1, label="hz + 2J(1-cos(k))")

    plt.title(title_text, fontsize=fontsize)
    plt.legend()
    plt.savefig(f"{run_dir}/dipression.pdf")
    plt.savefig(f"{run_dir}/dipression.png", dpi=300)
    plt.show()
    # quick diagnostics
    print("shape (k,omega):", S_k_omega.shape, "min:", S_k_omega.min(), "max:", S_k_omega.max())


