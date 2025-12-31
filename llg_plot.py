import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np
from matplotlib.colors import LogNorm

def output_normal_graph(
    fontsize,
    title_text,
    xlim,
    xlabel,
    x_data,
    ylim,
    ylabel,
    y_data,
    legend_text,
    figsize,
):
    fig, ax = plt.subplots(figsize=figsize)
    ax.grid()
    ax.set_title(label=title_text,fontsize=fontsize)
    ax.tick_params(labelsize=fontsize)
    ax.set_xlim(0,xlim)
    ax.set_xlabel(xlabel, fontsize=fontsize)
    ax.set_ylabel(ylabel, fontsize=fontsize)
    ax.set_ylim(-ylim, ylim)
    ax.plot(x_data, y_data, label=legend_text)
    ax.legend(fontsize=fontsize)
    plt.show()

def out_put_heatmap(
        Lx,
        N_steps,
        S,
        dt,
        minimum,
        figsize,
):
    omega_size = int(N_steps)
    k_size = int(Lx)
    omega_axis  = np.arange(omega_size)
    k_axis = np.arange(k_size)
    S_k_omega = S.reshape(k_size, omega_size)
    plt.figure(figsize=figsize)
    img = plt.imshow(S_k_omega, cmap='viridis', norm=LogNorm(vmin=minimum, vmax=S_k_omega.max()), aspect='auto', origin='lower', extent=[0, 2*np.pi, 0, (2*np.pi/dt)])
    cbar = plt.colorbar(img)
    cbar.ax.tick_params(labelsize=15)
    plt.show()
    print(Lx,N_steps)