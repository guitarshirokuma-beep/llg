#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<fftw3.h>
#include"llg.hpp"
using namespace std;

//Don't change order
void output_params(const Params& p){
	cout << "Lx = " << p.Lx << "\n";
	cout << "N_steps = " << p.N_steps << "\n";
	cout << "lam = " << p.lam << "\n";
	cout << "h_app_norm = " << p.h_app_norm << "\n";
	cout << "pulse_norm = " << p.pulse_norm << "\n";
	cout << "dt = " << p.dt << "\n";
	cout << "gamma = " << p.gamma << "\n";
	cout << "J = " << p.J << "\n";
	cout << "sigma = " << p.sigma << "\n";
	cout << "delta = " << p.delta << "\n";

	ofstream ofs("llg_params.dat");
	ofs <<  p.Lx << "\n";
	ofs << p.N_steps << "\n";
	ofs << p.lam << "\n";
	ofs << p.h_app_norm << "\n";
	ofs << p.pulse_norm << "\n";
	ofs << p.dt << "\n";
	ofs << p.gamma << "\n";
	ofs << p.J << "\n";
	ofs << p.sigma << "\n";
	ofs << p.delta << "\n";
}

	