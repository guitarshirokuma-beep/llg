#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<fftw3.h>
#include"llg.hpp"
using namespace std;

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

