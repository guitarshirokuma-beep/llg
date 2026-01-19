#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<fftw3.h>
#include"llg.hpp"
using namespace std;



Make2DArray calc_response(
	const Params& p,
	const Make2DArray& S,
	const Make2DArray& h_app
){
	Make2DArray response(p.Lx, p.N_steps);
	for(int n=0; n<p.Lx; n++){
		for(int step=0; step<p.N_steps; step++){
			response(n, step).x = S(n, step).y / h_app(n, step).x;
		}
	}
	return response;
}