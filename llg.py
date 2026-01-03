import matplotlib
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np
import llg_plot
import importlib
importlib.reload(llg_plot)

params_data = np.loadtxt("llg_params.dat")

Lx = int(params_data[0])
N_steps = params_data[1]
lam = params_data[2]
h_app_norm = params_data[3]
dt = params_data[4]
gamma = params_data[5]
J = params_data[6]

title_text = rf'$Lx = {Lx}, hz = {h_app_norm}, dt = {dt}, J = , \lambda = {lam}, k = , \omega = $'

llg_data = np.loadtxt("llg.dat")
h_k_omega = llg_data[:, 2]

figsize=(8,6)
fontsize=15
title_text = rf'$Lx = {Lx}, hz = {h_app_norm}, dt = {dt}, J = , \lambda = {lam}, k = , \omega = $'
xlabel = r'Step'
ylabel = r'$m_y$'
legend_text = r'$m_x( n = 0, t )$'
minimum = 1

llg_plot.output_heatmap(
    Lx,
    N_steps,
    h_k_omega,
    dt,
    minimum,
)
