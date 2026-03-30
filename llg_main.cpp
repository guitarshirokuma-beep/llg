#include <iostream>
#include <fstream>
#include <vector>
#include "llg.hpp"
#include "def_Array2DVec3_func.hpp"
#include <filesystem>
#include <Eigen/Dense>
using namespace std;

int main()
{
	Params p;

	// mkdir for log
	auto run_dir = make_run_dir();
	p.save_params(run_dir);

	Array2DVec3 S(p.Lx_sizet, p.N_steps_sizet);
	Array2DVec3 h_app(p.Lx_sizet, p.N_steps_sizet);
	initialize(p, S, h_app);
	run_llg(p, S, h_app);

	S = fft_2d(
		p,
		S,
		[](const Eigen::Vector3d& v)
		{ return v.y(); },
		[](Eigen::Vector3d &v, double val)
		{ v.y() = val; });

	Array2DVec3 response(p.Lx, p.N_steps);

	h_app = fft_2d(
		p,
		h_app,
		[](const Eigen::Vector3d &v)
		{ return v.x(); },
		[](Eigen::Vector3d &v, double val)
		{ v.x() = val; });

	avoid_zero(p, h_app);

	response = calc_response(
		p,
		S,
		[](const Eigen::Vector3d &v)
		{ return v.y(); },
		h_app,
		[](const Eigen::Vector3d &v)
		{ return v.x(); });

	Array1DVec3 S_n_0 = extract_const_n(p, S, 0);
	output_data(p, S_n_0, 'y', run_dir);

	run_python(run_dir);
    
	return 0;
}
