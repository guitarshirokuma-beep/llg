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
