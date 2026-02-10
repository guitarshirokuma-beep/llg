#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <fftw3.h>
#include "llg.hpp"
using namespace std;

// Don't change order
void output_params(const Params &p)
{
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

	ofstream ofs("llg_params.dat");
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
