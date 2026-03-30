#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<fftw3.h>
#include"llg.hpp"
#include<Eigen/Dense>
using namespace std;

Array1DVec3 extract_const_step(
	const Params&		p,
	const Array2DVec3&	S_2d,
	int					step
){
	Array1DVec3 S_1d(p.Lx);
	for(std::size_t n=0; n<p.Lx; ++n){
		S_1d(n) = S_2d(n, step);
	}
	return S_1d;
}

Array1DVec3 extract_const_n(
	const Params&		p,
	const Array2DVec3&	S_2d,
	int					n
){
	Array1DVec3 S_1d(p.N_steps);
	for(int step=0; step<p.N_steps; step++){
		S_1d(step) = S_2d(n, step);
	}
	return S_1d;
}

Array1DVec3 calc_dSdt(
	const Params&		p,
	const Array1DVec3&	S_step,
	const Array2DVec3&	h_app,
	const Array1DVec3&	h_exc,
	int					step
){
	Array1DVec3 dS_over_dt(p.Lx);
	for(int n=0; n<p.Lx; n++){
        Eigen::Vector3d h = h_app(n, step) + h_exc(n);
        Eigen::Vector3d Sxh = S_step(n).cross(h);
        Eigen::Vector3d SxSxh = S_step(n).cross(Sxh);

		double c = (-p.gamma)/(1.0 + p.lam*p.lam);

		dS_over_dt(n) = c*(Sxh + p.lam*SxSxh);
	}

	return dS_over_dt;
}

Array1DVec3 calc_h_exc(
	const Params&		p,
	const Array1DVec3&	S_old
){
	Array1DVec3 h_exc(p.Lx);
	for(int n=0; n<p.Lx; n++){
		h_exc(n) = S_old((n-1+p.Lx) % p.Lx);
		h_exc(n) += S_old((n+1) % p.Lx);
		h_exc(n) = p.J * h_exc(n);
	}
	return h_exc;
}

Array1DVec3& fft_1d_time(
	const Params&	p,
	Array1DVec3&	S,
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
			in[step][0] = S(step).x();
			in[step][1] = 0.0;
		}
		break;
	case 'y':
		for(int step=0; step<p.N_steps; step++){
			in[step][0] = S(step).y();
			in[step][1] = 0.0;
		}
		break;
	case 'z':
		for(int step=0; step<p.N_steps; step++){
			in[step][0] = S(step).z();
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
			S(step).x() = amp;
		}
		break;
	case 'y':
		for(int step=0; step<p.N_steps; step++){
			double amp = sqrt(out[step][0]*out[step][0] + out[step][1]*out[step][1]);
			S(step).y() = amp;
		}
		break;
	case 'z':
		for(int step=0; step<p.N_steps; step++){
			double amp = sqrt(out[step][0]*out[step][0] + out[step][1]*out[step][1]);
			S(step).z() = amp;
		}
		break;
	default:
		break;
	}

	fftw_free(in);
	fftw_free(out);
	return S;
}

Array1DVec3 calc_response(
	const Params& p,
	const Array1DVec3& S,
	const Array1DVec3& h_app
){
	Array1DVec3 response(p.N_steps);
	for(int step=0; step<p.N_steps; step++){
		response(step) = S(step).array() / h_app(step).array();
	}
	return response;
}

