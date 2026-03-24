#include <filesystem>
#include <chrono>
#include <format>
#include <fstream>
#include <array>
#include <cstdio>
#include <unistd.h>
#include <sys/wait.h>

#include "params.hpp"
#include "llg.hpp"

void Params::save_params(const std::filesystem::path &dir) const
{
    write_params_file(*this, dir);
    
    print_params(*this);

    write_python_params(*this);
}

void write_params_file(const Params &p, const std::filesystem::path &dir){
    std::ofstream f(dir / "params.txt");

    f << "Lx " << p.Lx << "\n";
    f << "N_steps " << p.N_steps << "\n";
    f << "lam " << p.lam << "\n";
    f << "h_app_static " << p.h_app_static << "\n";
    f << "sigma_x " << p.sigma_x << "\n";
    f << "sigma_step " << p.sigma_step << "\n";
    f << "pulse_norm " << p.pulse_norm << "\n";
    f << "time_pulse_center " << p.time_pulse_center << "\n";
    f << "local_pulse_center " << p.local_pulse_center << "\n";
    f << "sin_norm " << p.sin_norm << "\n";
    f << "omega " << p.omega << "\n";
    f << "J " << p.J << "\n";
    f << "delta " << p.delta << "\n";
    f << "dt " << p.dt << "\n";
    f << "gamma " << p.gamma << "\n";
    f << "git_hash " << get_git_hash() << "\n";
}

void print_params(const Params &p){
    cout << "Lx = " << p.Lx << "\n";
    cout << "N_steps = " << p.N_steps << "\n";
    cout << "lam = " << p.lam << "\n";
    cout << "h_app_static = " << p.h_app_static << "\n";
    cout << "pulse_norm = " << p.pulse_norm << "\n";
    cout << "sin_norm = " << p.sin_norm << "\n";
    cout << "omega = " << p.omega << "\n";
    cout << "dt = " << p.dt << "\n";
    cout << "gamma = " << p.gamma << "\n";
    cout << "J = " << p.J << "\n";
    cout << "sigma_x = " << p.sigma_x << "\n";
    cout << "sigma_step = " << p.sigma_step << "\n";
    cout << "delta = " << p.delta << "\n";
}

void write_python_params(const Params &p){
    std::ofstream ofs("llg_params.dat");
    ofs << p.Lx << "\n";
    ofs << p.N_steps << "\n";
    ofs << p.lam << "\n";
    ofs << p.h_app_static << "\n";
    ofs << p.pulse_norm << "\n";
    ofs << p.dt << "\n";
    ofs << p.gamma << "\n";
    ofs << p.J << "\n";
    ofs << p.sigma_x << "\n";
    ofs << p.delta << "\n";
    ofs << p.sigma_step << "\n";
    ofs << p.sin_norm << "\n";
    ofs << p.omega << "\n";
}
