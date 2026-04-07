#ifndef PARAMS_H
#define PARAMS_H
#include <filesystem>

struct Params
{
public:
	const int Lx = 4;
	const int N_steps = 32768;
	const std::size_t Lx_sizet = Lx;
	const std::size_t N_steps_sizet = N_steps;
	const double lam = 0.05;
	const double h_app_static = 0.0;
	const double sigma_x = 0.5;
	const double sigma_step = 0.8;
	const double pulse_norm = 0.0;
	const double time_pulse_center = double(N_steps) / 2.0;
	const double local_pulse_center = double(Lx) / 2.0;
	const double sin_norm = 20.0;
	const double omega = 15.0;
	const double delta = 1e-13; // for avoid_zero()
	const double dt = 0.001;
	const double gamma = 1.0;
	const double J = 1.0;
	const double tau_ramp = 10.0;
	void save_params(const std::filesystem::path &dir) const;
};

#endif
