#ifndef DEF_MAKE2DARRAY_FUNC
#define DEF_MAKE2DARRAY_FUNC

#include<iostream>
#include<vector>
#include"llg.hpp"
#include<fftw3.h>
#include<math.h>


template<typename T1, typename T2>
Make2DArray& fft_2d(
	const Params&	p,
	Make2DArray&	S,
	T1				axis_in,
	T2				axis_out
){
	fftw_complex *in, *out;
	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (p.Lx*p.N_steps));
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (p.Lx*p.N_steps));

	fftw_plan plan = fftw_plan_dft_2d(p.Lx, p.N_steps, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	for(int n=0; n<p.Lx; n++){
		for(int step=0; step<p.N_steps; step++){
			int idx = n*p.N_steps + step;
			in[idx][0] = axis_in(S(n, step));
			in[idx][1] = 0.0;
		}
	}

	fftw_execute(plan);

	for(int n=0; n<p.Lx; n++){
		for(int step=0; step<p.N_steps; step++){
			int idx = n*p.N_steps + step;
			double real = out[idx][0];
			double imag = out[idx][1];
			axis_out(S(n, step), sqrt(real*real + imag*imag));
		}
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

#endif