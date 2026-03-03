#include <iostream>
#include <fstream>
#include <vector>
#include "llg.hpp"
#include "def_Make2DArray_func.hpp"
#include <filesystem>
using namespace std;

int main()
{
	Params p;

	// mkdir for log
	auto run_dir = make_run_dir();
	save_params(p, run_dir);

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

	Make1DArray S_n_0 = extract_const_n(p, S, 0);
	output_data(p, S_n_0, 'y', run_dir);

	run_python(run_dir);

	return 0;
}