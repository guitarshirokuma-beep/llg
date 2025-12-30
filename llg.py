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
step = llg_data[:, 0]
m_n0_step_x = llg_data[:, 1]

figsize=(8,6)
fontsize=15
title_text = rf'$Lx = {Lx}, hz = {h_app_norm}, dt = {dt}, J = , \lambda = {lam}, k = , \omega = $'
xlim = Lx
ylim = 1.1
xlabel = r'Step'
ylabel = r'$m_y$'
x_data = step
y_data = m_n0_step_x
legend_text = r'$m_x( n = 0, t )$'

llg_plot.output_graph(
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
)





