#include<iostream>
#include<fstream>
#include<vector>
#include"llg.h"
using namespace std;

int main(){
	const int Lx = 3;
	const int N_steps = 5;
	const double lam = 0.05;
	const double h_app_norm = 10.102;
	Params p(Lx, N_steps, lam, h_app_norm);
	Make2DArray S(p.Lx, p.N_steps);
	Make2DArray h_app(p.Lx, p.N_steps);
	initialize(p, S, h_app);
	run_llg(p, S, h_app);
	output_data(p, S);
	output_params(p);
	return 0;
}
