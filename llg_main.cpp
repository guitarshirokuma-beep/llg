#include<iostream>
#include<fstream>
#include<vector>
#include"llg.hpp"
using namespace std;

int main(){
	const int Lx = 2048;
	const int N_steps = 8192;
	const double lam = 0.03;
	const double h_app_norm = 10.0;
	const double sigma = 0.5;
	const double sin_norm = 10.0;
	const double time_pulse_center = N_steps / 2.0;
	const double local_pulse_center = Lx / 2.0;
	const double J = 1.0;
	const double delta = 1e-13;//for avoid_zero()
	const double omega = 15.0;

	Params p(Lx, N_steps, lam, h_app_norm, sigma, sin_norm, time_pulse_center, local_pulse_center, J, delta, omega);

	output_params(p);
	Make2DArray S(p.Lx, p.N_steps);
	Make2DArray h_app(p.Lx, p.N_steps);
	initialize(p, S, h_app);
	run_llg(p, S, h_app);
	fft_2d(p, S, 'y');
	fft_2d(p, h_app, 'x');
	avoid_zero(p, h_app);
	Make2DArray response = calc_response(p, S, h_app);
	avoid_zero(p, response);
	Make1DArray response_n0 = extract_const_n(p, response, 0);
	output_data(p, response, 'x');
	return 0;
}