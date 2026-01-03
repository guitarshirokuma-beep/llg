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
	const double J = 10.0;
	const double sigma = 10.0;
	Params p(Lx, N_steps, lam, h_app_norm, pulse_norm, J);
	output_params(p);
	Make2DArray S(p.Lx, p.N_steps);
	Make2DArray h_app(p.Lx, p.N_steps);
	initialize(p, S, h_app);
	run_llg(p, S, h_app);
	//test
	S = fft_2d(p, S);
	output_data(p, S);
	return 0;
}