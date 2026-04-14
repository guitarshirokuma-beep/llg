import matplotlib
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np
import llg_plot
import importlib
importlib.reload(llg_plot)
import sys

run_dir = sys.argv[1]

params_data = np.loadtxt("llg_params.dat")

Lx = int(params_data[0])
N_steps = int(params_data[1])
lam = params_data[2]
h_app_static = params_data[3]
pulse_norm = params_data[4]
dt = params_data[5]
gamma = params_data[6]
J = params_data[7]
sigma_x = params_data[8]
delta = params_data[9]
sigma_step = params_data[10]
sin_norm = params_data[11]
omega = params_data[12]
tau_ramp = params_data[13]

title_text = rf'$Lx = {Lx}, N_{{steps}} = {N_steps}, h_z = {h_app_static}, h_x\;pulse = {pulse_norm}, \sigma_x = {sigma_x}, $'+"\n"+rf'$\sigma_{{step}} = {sigma_step}, dt = {dt}, J = {J}, \lambda = {lam}, h_{{sin}} = {sin_norm}, \omega = {omega}, \tau = {tau_ramp}$'

llg_data = np.loadtxt(f"{run_dir}/llg.dat")
x_data = llg_data[:, 0]*(2.0*np.pi) / (dt*N_steps)
y_data = llg_data[:, 1]
# S_k_omega = llg_data[:, 2]

fontsize=12
xlabel = r'$\omega$'
ylabel = r'$|m_y(k=0, \omega)|$'
clabel = r'$|m_y(k, \omega)|$'
legend_text = r'$m_y( n = 0, t )$'
# Set the minimum value for the color scale
vmin_self = 0.09

llg_plot.output_normal_graph(
    fontsize,
    title_text,
    xlabel,
    x_data,
    ylabel,
    y_data,
    legend_text,
    run_dir,
)

# llg_plot.output_heatmap(
#     title_text,
#     Lx,
#     N_steps,
#     S_k_omega,
#     dt,
#     h_app_static,
#     J,
#     vmin_self,
#     xlabel,
#     ylabel,
#     fontsize,
#     clabel,
#     run_dir,
# )
