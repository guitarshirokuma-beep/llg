#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <fftw3.h>
#include "llg.hpp"
using namespace std;

double gaussian(int n, double center, double sigma)
{
	return exp(-0.5 * pow((double(n) - center) / sigma, 2));
}

void initialize(Params &p, Array2DVec3 &S, Array2DVec3 &h_app)
{

	for (int n = 0; n < p.Lx; n++)
	{
		for (int step = 0; step < p.N_steps; step++)
		{
			double t = step * p.dt;
			double f = 1.0 - exp(-t / p.tau_ramp);
			double local_pulse = gaussian(n, p.local_pulse_center, p.sigma_x);
			double time_pulse = gaussian(step, p.time_pulse_center, p.sigma_step);
			h_app(n, step).x() = p.pulse_norm * time_pulse * local_pulse + p.sin_norm * f * sin(p.omega * step * p.dt);
			h_app(n, step).y() = 0.0;
			h_app(n, step).z() = p.h_app_static;
		}
	}

	int step = 0;
	for (int n = 0; n < p.Lx; n++)
	{
		S(n, step).x() = 0.0;
		S(n, step).y() = 0.0;
		S(n, step).z() = 1.0;
	}
}

void input(
	const Params &p,
	Array2DVec3 &S,
	const Array1DVec3 &S_new,
	int step)
{
	for (int n = 0; n < p.Lx; n++)
	{
		S(n, step) = S_new(n);
	}
}

void run_llg(
	const Params &p,
	Array2DVec3 &S,
	const Array2DVec3 &h_app)
{
	for (int step = 0; step < p.N_steps - 1; step++)
	{
		Array1DVec3 S_old = extract_const_step(p, S, step);
		Array1DVec3 h_exc = calc_h_exc(p, S_old);
		Array1DVec3 dS_over_dt = calc_dSdt(p, S_old, h_app, h_exc, step);
		Array1DVec3 S_pred = S_old + p.dt * dS_over_dt;
		S_pred.normalize();
		Array1DVec3 h_exc_2 = calc_h_exc(p, S_pred);
		Array1DVec3 dS_over_dt_2 = calc_dSdt(p, S_pred, h_app, h_exc_2, step);
		Array1DVec3 S_new = S_old + 0.5 * p.dt * (dS_over_dt + dS_over_dt_2);
		S_new.normalize();
		input(p, S, S_new, step + 1);
	}
}

void output_data(
	const Params &p,
	const Array1DVec3 &S,
	char axis,
	const std::string &dir)
{
	std::ofstream f(dir + "/llg.dat");
	switch (axis)
	{
	case 'x':
		f << "# step S(step).x\n";
		for (int step = 0; step < p.N_steps; step++)
		{
			f << step << " " << S(step).x() << "\n";
		}
		break;
	case 'y':
		f << "# step S(step).y\n";
		for (int step = 0; step < p.N_steps; step++)
		{
			f << step << " " << S(step).y() << "\n";
		}
		break;
	case 'z':
		f << "# step S(step).z\n";
		for (int step = 0; step < p.N_steps; step++)
		{
			f << step << " " << S(step).z() << "\n";
		}
		break;
	default:
		throw invalid_argument("axis must be x, y, or z");
	}
}

void output_data(
	const Params &p,
	const Array2DVec3 &S,
	char axis,
	const std::string &dir)
{
	std::ofstream f(dir + "/llg.dat");
	switch (axis)
	{
	case 'x':
		f << "# n step S(n, step).x\n";
		for (int n = 0; n < p.Lx; n++)
		{
			for (int step = 0; step < p.N_steps; step++)
			{
				f << n << " " << step << " " << S(n, step).x() << "\n";
			}
		}
		break;
	case 'y':
		f << "# n step S(n, step).y\n";
		for (int n = 0; n < p.Lx; n++)
		{
			for (int step = 0; step < p.N_steps; step++)
			{
				f << n << " " << step << " " << S(n, step).y() << "\n";
			}
		}
		break;
	case 'z':
		f << "# n step S(n, step).z\n";
		for (int n = 0; n < p.Lx; n++)
		{
			for (int step = 0; step < p.N_steps; step++)
			{
				f << n << " " << step << " " << S(n, step).z() << "\n";
			}
		}
		break;
	default:
		throw invalid_argument("axis must be x, y, or z");
	}
}

void avoid_zero(
	const Params &p,
	Array2DVec3 &h_app)
{
	for (int n = 0; n < h_app.size_x(); ++n)
	{
		for (int step = 0; step < h_app.size_x(); step++)
		{
			if (h_app(n, step).x() < p.delta)
			{
				h_app(n, step).x() = p.delta;
			}
		}
	}
}
