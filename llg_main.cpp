#include <iostream>
#include <fstream>
#include <vector>
#include "llg.hpp"
#include "def_Make2DArray_func.hpp"
using namespace std;

int main()
{
	const int Lx = 2048;
	const int N_steps = 2048;
	const double lam = 0.03;
	const double h_app_norm = 10.0;
	const double sigma_x = 0.5;
	const double sigma_step = 0.8;
	const double pulse_norm = 0.01;
	const double time_pulse_center = N_steps / 2.0;
	const double local_pulse_center = Lx / 2.0;
	const double J = 10.0;
	const double delta = 1e-13; // for avoid_zero()

	Params p(Lx, N_steps, lam, h_app_norm, sigma_x, sigma_step, pulse_norm, time_pulse_center, local_pulse_center, J, delta);

	output_params(p);
	Make2DArray S(p.Lx, p.N_steps);
	Make2DArray h_app(p.Lx, p.N_steps);
	initialize(p, S, h_app);
	run_llg(p, S, h_app);

	S = fft_2d(
		p,
		S,
		[](const Vec3 &v)
		{ return v.y; },
		[](Vec3 &v, double val)
		{ v.y = val; });

	Make2DArray response(p.Lx, p.N_steps);

	h_app = fft_2d(
		p,
		h_app,
		[](const Vec3 &v)
		{ return v.x; },
		[](Vec3 &v, double val)
		{ v.x = val; });

	avoid_zero(p, h_app);

	response = calc_response(
		p,
		S,
		[](const Vec3 &v)
		{ return v.y; },
		h_app,
		[](const Vec3 &v)
		{ return v.x; });

	output_data(p, response, 'x');

	return 0;
}