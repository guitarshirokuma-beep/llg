#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<fftw3.h>
#include"llg.hpp"
using namespace std;

Make1DArray extract_const_step(
	const Params&		p,
	const Make2DArray&	S_2d,
	int					step
){
	Make1DArray S_1d(p.Lx);
	for(int n=0; n<p.Lx; n++){
		S_1d(n) = S_2d(n, step);
	}
	return S_1d;
}

Make1DArray extract_const_n(
	const Params&		p,
	const Make2DArray&	S_2d,
	int					n
){
	Make1DArray S_1d(p.N_steps);
	for(int step=0; step<p.N_steps; step++){
		S_1d(step) = S_2d(n, step);
	}
	return S_1d;
}

Make1DArray calc_dSdt(
	const Params&		p,
	const Make1DArray&	S_step,
	const Make2DArray&	h_app,
	const Make1DArray&	h_exc,
	int					step
){
	Make1DArray dS_over_dt(p.Lx);
	for(int n=0; n<p.Lx; n++){
		Vec3 h = h_app(n, step) + h_exc(n);
		Vec3 Sxh = S_step(n).cross(h);
		Vec3 SxSxh = S_step(n).cross(Sxh);

		double c = (-p.gamma)/(1.0 + p.lam*p.lam);

		dS_over_dt(n) = c*(Sxh + p.lam*SxSxh);
	}

	return dS_over_dt;
}

Make1DArray calc_h_exc(
	const Params&		p,
	const Make1DArray&	S_old
){
	Make1DArray h_exc(p.Lx);
	for(int n=0; n<p.Lx; n++){
		h_exc(n) = S_old((n-1+p.Lx) % p.Lx);
		h_exc(n) += S_old((n+1) % p.Lx);
		h_exc(n) = p.J * h_exc(n);
	}
	return h_exc;
}

Make1DArray& fft_1d_time(
	const Params&	p,
	Make1DArray&	S,
	char			axis
){
	fftw_complex *in, *out;
	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * p.N_steps);
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * p.N_steps);

	fftw_plan plan = fftw_plan_dft_1d(p.N_steps, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

	switch (axis)
	{
	case 'x':
		for(int step=0; step<p.N_steps; step++){
			in[step][0] = S(step).x;
			in[step][1] = 0.0;
		}
		break;
	case 'y':
		for(int step=0; step<p.N_steps; step++){
			in[step][0] = S(step).y;
			in[step][1] = 0.0;
		}
		break;
	case 'z':
		for(int step=0; step<p.N_steps; step++){
			in[step][0] = S(step).z;
			in[step][1] = 0.0;
		}
		break;
	default:
		throw invalid_argument("axis must be x, y, or z");
	}

	fftw_execute(plan);

	switch (axis)
	{
	case 'x':
		for(int step=0; step<p.N_steps; step++){
			double amp = sqrt(out[step][0]*out[step][0] + out[step][1]*out[step][1]);
			S(step).x = amp;
		}
		break;
	case 'y':
		for(int step=0; step<p.N_steps; step++){
			double amp = sqrt(out[step][0]*out[step][0] + out[step][1]*out[step][1]);
			S(step).y = amp;
		}
		break;
	case 'z':
		for(int step=0; step<p.N_steps; step++){
			double amp = sqrt(out[step][0]*out[step][0] + out[step][1]*out[step][1]);
			S(step).z = amp;
		}
		break;
	default:
		break;
	}

	fftw_free(in);
	fftw_free(out);
	return S;
}

Make1DArray calc_response(
	const Params& p,
	const Make1DArray& S,
	const Make1DArray& h_app
){
	Make1DArray response(p.N_steps);
	for(int step=0; step<p.N_steps; step++){
		response(step) = S(step) / h_app(step);
	}
	return response;
}

