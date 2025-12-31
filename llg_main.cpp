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
	output_params(p);
	Make2DArray S(p.Lx, p.N_steps);
	Make2DArray h_app(p.Lx, p.N_steps);
	initialize(p, S, h_app);
	run_llg(p, S, h_app);
	Make1DArray S_n0_step = extract_const_n(p, S, 0);	
	S_n0_step = fft_1d_time(p, S_n0_step);
	output_data(p, S_n0_step);
	//test
	S = fft_2d(p, S);
	return 0;
}