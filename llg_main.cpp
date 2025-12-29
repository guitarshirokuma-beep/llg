#include<iostream>
#include<fstream>
#include<vector>
#include"llg.h"
using namespace std;

int main(){
	const int Lx = 2048;
	const int N_steps = 2048;
	const double lam = 0.05;
	const double h_app_norm = 10;
	const double J = 10.0;
	const double sigma = 10;
	Params p(Lx, N_steps, lam, h_app_norm, J);
	Make2DArray S(p.Lx, p.N_steps);
	Make2DArray h_app(p.Lx, p.N_steps);
	initialize(p, S, h_app);
	run_llg(p, S, h_app);
	output_params(p);
	Make1DArray S_n0_step(p.N_steps);
	for(int step=0; step<p.N_steps; step++){
		S_n0_step(step) = S(0, step);
	}
	S_n0_step = fft_1d(p, S_n0_step);
	output_data(p, S_n0_step);
	double peak = p.h_app_norm * p.gamma;
	double pi = 3.1415;
	double k_peak = peak * (double)p.Lx * p.dt / (2.0 * pi);
	cout << k_peak << "\n"; 
	return 0;
}
