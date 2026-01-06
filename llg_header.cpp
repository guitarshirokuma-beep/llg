#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<fftw3.h>
#include"llg.h"
using namespace std;

Data Data::cross(const Data& other) const{
	return {
		y*other.z - z*other.y,
		z*other.x - x*other.z,
		x*other.y - y*other.x
	};
}

Data Data::operator+(const Data& other) const{
	return {
		x + other.x,
		y + other.y,
		z + other.z
	};
}

Data operator*(double c, const Data& a){
	return {c * a.x, c * a.y, c* a.z};
}

Data operator/(const Data& a, const Data& b){
	return { a.x / b.x, a.y / b.y, a.z / b.z };
}

Data& Data::operator+=(const Data& other){
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

void Data::normalize(){
	double n = sqrt(x*x + y*y + z*z);
	if(n > 0.0){
		x /= n;
		y /= n;
		z /= n;
	}
	else{
		cout << "size <= 0\n";
	}
}

Data& Make2DArray::operator()(int x, int t){
			return val[x * N_steps + t];
		}

const Data& Make2DArray::operator()(int x, int t) const{
	return val[x * N_steps + t];
}

Make2DArray operator/(
	const Make2DArray& a,
	const Make2DArray& b
){
	Make2DArray result(a.Lx, a.N_steps);
	for(int n=0; n<a.Lx; n++){
		for(int step=0; step<a.N_steps; step++){
			result(n, step) = a(n, step) / b(n, step);
		}
	}
	return result;
}

Data& Make1DArray::operator()(int x){
			return val[x];
		}

const Data& Make1DArray::operator()(int x) const{
	return val[x];
}

Make1DArray Make1DArray::operator+(const Make1DArray& other) const{
	Make1DArray result(Lx);
	for(int n=0; n<Lx; n++){
		result.val[n] = val[n] + other.val[n];
	}
	return result;
}

Make1DArray& Make1DArray::operator+=(const Make1DArray& other) {
	Make1DArray result(Lx);
	for(int n=0; n<Lx; n++){
		val[n] += other.val[n];	
	}
	return *this;	
}

Make1DArray operator*(double c, const Make1DArray& a){
	Make1DArray result(a.Lx);
	for(int n=0; n<a.Lx; n++){
		result.val[n] = c * a.val[n];
	}
	return result;
}

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

void Make1DArray::normalize(){
	for(int n=0; n<Lx; n++){
		val[n].normalize();
	}
}

double gaussian(int n, double center, double sigma){
	return exp(-0.5 * pow((double(n) - center) / sigma, 2));
}

void initialize(Params& p, Make2DArray& S, Make2DArray& h_app){
    for(int n=0; n<p.Lx; n++){
        for(int step=0; step<p.N_steps; step++){
			double local_pulse = gaussian(n, p.local_pulse_center, p.sigma);
			double time_pulse = gaussian(step, p.time_pulse_center, p.sigma);
			 h_app(n, step).x = p.pulse_norm * time_pulse * local_pulse;
             h_app(n, step).y = 0.0;
             h_app(n, step).z = p.h_app_norm;
         }
     }
 
     int step = 0;
     for(int n=0; n<p.Lx; n++){
         S(n, step).x = 0.0;
         S(n, step).y = 0.0;
         S(n, step).z = 1.0;
     }
 }

void input(
	const Params&		p,
	Make2DArray&		S,
	const Make1DArray&	S_new,
	int					step
){
	for(int n=0; n<p.Lx; n++){
		S(n, step) = S_new(n);
	}
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
		Data h = h_app(n, step) + h_exc(n);
		Data Sxh = S_step(n).cross(h);
		Data SxSxh = S_step(n).cross(Sxh);

		double c = (-p.gamma)/(1.0 + p.lam*p.lam);

		dS_over_dt(n) = c*(Sxh + p.lam*SxSxh);
	}

	return dS_over_dt;
}

void run_llg(
	const Params&		p,
	Make2DArray&		S,
	const Make2DArray&	h_app
){
	for(int step=0; step<p.N_steps-1; step++){
		Make1DArray S_old = extract_const_step(p, S, step);
		Make1DArray h_exc = calc_h_exc(p, S_old);
		Make1DArray dS_over_dt = calc_dSdt(p, S_old, h_app, h_exc, step);
		Make1DArray S_pred = S_old + p.dt * dS_over_dt;
		S_pred.normalize();
		Make1DArray h_exc_2 = calc_h_exc(p, S_pred);
		Make1DArray dS_over_dt_2 = calc_dSdt(p, S_pred, h_app, h_exc_2, step);
		Make1DArray S_new = S_old + 0.5*p.dt*( dS_over_dt + dS_over_dt_2 );
		S_new.normalize();
		input(p, S, S_new, step+1);
	}
}

void output_data(
	const Params&		p,
	const Make1DArray&	S,
	char				axis
){
	ofstream ofs("llg.dat");
	switch (axis){
	case 'x':
		ofs << "# step S(step).x\n";
		for(int step=0; step<p.N_steps; step++){
			ofs << step << " " << S(step).x << "\n";
		}
		break;
	case 'y':
		ofs << "# step S(step).y\n";
		for(int step=0; step<p.N_steps; step++){
			ofs << step << " " << S(step).y << "\n";
		}
		break;
	case 'z':
		ofs << "# step S(step).z\n";
		for(int step=0; step<p.N_steps; step++){
			ofs << step << " " << S(step).z << "\n";
		}
		break;
	default:
		throw invalid_argument("axis must be x, y, or z");
	}
	ofs.close();
}

void output_data(
	const Params&		p,
	const Make2DArray&	S,
	char				axis
){
	ofstream ofs("llg.dat");
	switch (axis){
	case 'x':
		ofs << "# n step S(n, step).x\n";
		for(int n=0; n<p.Lx; n++){
			for(int step=0; step<p.N_steps; step++){
				ofs << n << " " << step << " " << S(n, step).x << "\n";
			}
		}
		break;
	case 'y':
		ofs << "# n step S(n, step).y\n";
		for(int n=0; n<p.Lx; n++){
			for(int step=0; step<p.N_steps; step++){
				ofs << n << " " << step << " " << S(n, step).y << "\n";
			}
		}
		break;
	case 'z':
		ofs << "# n step S(n, step).z\n";
		for(int n=0; n<p.Lx; n++){
			for(int step=0; step<p.N_steps; step++){
				ofs << n << " " << step << " " << S(n, step).z << "\n";
			}
		}
		break;
	default:
		throw invalid_argument("axis must be x, y, or z");
	}
	ofs.close();
}

//Don't change order
void output_params(const Params& p){
	cout << "Lx = " << p.Lx << "\n";
	cout << "N_steps = " << p.N_steps << "\n";
	cout << "lam = " << p.lam << "\n";
	cout << "h_app_norm = " << p.h_app_norm << "\n";
	cout << "pulse_norm = " << p.pulse_norm << "\n";
	cout << "dt = " << p.dt << "\n";
	cout << "gamma = " << p.gamma << "\n";
	cout << "J = " << p.J << "\n";
	cout << "sigma = " << p.sigma << "\n";
	cout << "delta = " << p.delta << "\n";

	ofstream ofs("llg_params.dat");
	ofs <<  p.Lx << "\n";
	ofs << p.N_steps << "\n";
	ofs << p.lam << "\n";
	ofs << p.h_app_norm << "\n";
	ofs << p.pulse_norm << "\n";
	ofs << p.dt << "\n";
	ofs << p.gamma << "\n";
	ofs << p.J << "\n";
	ofs << p.sigma << "\n";
	ofs << p.delta << "\n";
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

void avoid_zero(
	const Params& p,
	Make2DArray& h_app
){
	for(int n=0; n<h_app.Lx; n++){
		for(int step=0; step<h_app.N_steps; step++){
			if( h_app(n, step).x < p.delta ){
					h_app(n, step).x = p.delta;
				}
			}
		}
}