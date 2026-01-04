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
	const double pulse_norm = 1.0;
	const double time_pulse_center = N_steps / 2.0;
	const double local_pulse_center = Lx / 2.0;
	const double J = 10.0;
	const double sigma = 0.5;

	Params p(Lx, N_steps, lam, h_app_norm, pulse_norm, time_pulse_center, local_pulse_center, J);

	output_params(p);
	Make2DArray S(p.Lx, p.N_steps);
	Make2DArray h_app(p.Lx, p.N_steps);
	initialize(p, S, h_app);
	run_llg(p, S, h_app);
	S = fft_2d(p, S, 'y');
	output_data(p, S);
	return 0;
}