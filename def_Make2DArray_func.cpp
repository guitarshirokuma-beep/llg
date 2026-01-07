#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<fftw3.h>
#include"llg.hpp"
using namespace std;

Make2DArray& fft_2d(
	const Params&	p,
	Make2DArray&	S,
	char			axis
){
	fftw_complex *in, *out;
	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (p.Lx*p.N_steps));
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (p.Lx*p.N_steps));

	fftw_plan plan = fftw_plan_dft_2d(p.Lx, p.N_steps, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	switch (axis){
	case 'x':
		for(int n=0; n<p.Lx; n++){
			for(int step=0; step<p.N_steps; step++){
				int idx = n*p.N_steps + step;
				in[idx][0] = S(n, step).x;
				in[idx][1] = 0.0;
			}
		}
		break;
	case 'y':
		for(int n=0; n<p.Lx; n++){
			for(int step=0; step<p.N_steps; step++){
				int idx = n*p.N_steps + step;
				in[idx][0] = S(n, step).y;
				in[idx][1] = 0.0;
			}
		}
		break;
	case 'z':
		for(int n=0; n<p.Lx; n++){
			for(int step=0; step<p.N_steps; step++){
				int idx = n*p.N_steps + step;
				in[idx][0] = S(n, step).z;
				in[idx][1] = 0.0;
			}
		}
		break;
	default:
		throw invalid_argument("axis must be x, y, or z");
	}

	fftw_execute(plan);
	switch (axis){
	case 'x':
		for(int n=0; n<p.Lx; n++){
			for(int step=0; step<p.N_steps; step++){
				int idx = n*p.N_steps + step;
				double real = out[idx][0];
				double imag = out[idx][1];
				S(n, step).x = sqrt(real*real + imag*imag);
			}
		}
		break;
	case 'y':
		for(int n=0; n<p.Lx; n++){
			for(int step=0; step<p.N_steps; step++){
				int idx = n*p.N_steps + step;
				double real = out[idx][0];
				double imag = out[idx][1];
				S(n, step).y = sqrt(real*real + imag*imag);
			}
		}
		break;
	case 'z':
		for(int n=0; n<p.Lx; n++){
			for(int step=0; step<p.N_steps; step++){
				int idx = n*p.N_steps + step;
				double real = out[idx][0];
				double imag = out[idx][1];
				S(n, step).z = sqrt(real*real + imag*imag);
			}
		}
		break;
	default:
		break;
	}
	fftw_free(in);
	fftw_free(out);
	return S;
}

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