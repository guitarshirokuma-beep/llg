import matplotlib
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np
import llg_plot
import importlib
importlib.reload(llg_plot)

params_data = np.loadtxt("llg_params.dat")

Lx = int(params_data[0])
N_steps = int(params_data[1])
lam = params_data[2]
h_app_norm = params_data[3]
pulse_norm = params_data[4]
dt = params_data[5]
gamma = params_data[6]
J = params_data[7]
sigma_x = params_data[8]
delta = params_data[9]
sigma_step = params_data[10]

title_text = rf'$Lx = {Lx}, N_{{steps}} = {N_steps}, h_z = {h_app_norm}, h_x\;pulse = {pulse_norm},$'+"\n"+rf'$\sigma_x = {sigma_x}, \sigma_{{step}} = {sigma_step}, dt = {dt}, J = {J}, \lambda = {lam}$'

llg_data = np.loadtxt("llg.dat")
S_k_omega = llg_data[:, 2]

fontsize=12
xlabel = r'$k/\pi$'
ylabel = r'$\omega$'
clabel = r'$|\chi| = |m_y\;/\;h_{app\;x}|$'
legend_text = r'$m_x( n = 0, t )$'
# Set the minimum value for the color scale
vmin_self = 0.09

llg_plot.output_heatmap(
    title_text,
    Lx,
    N_steps,
    S_k_omega,
    dt,
    h_app_norm,
    J,
    vmin_self,
    xlabel,
    ylabel,
    fontsize,
    clabel,
)