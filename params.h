#ifndef PARAMS_H
#define PARAMX_H

class Params{
	public:
		int Lx;
		int N_steps;
		double lam;
		double h_app_norm;
		double sigma;
		double sin_norm;
		double time_pulse_center;
		double local_pulse_center;
		double delta;
		const double dt = 0.001;
		const double gamma = 1.0;
		double J;
		Params(int Lx_
				, int N_steps_
				, double lam_
				, double h_app_norm_
				, double sigma_
				, double sin_norm_
				, double time_pulse_center_
				, double local_pulse_center_
				, double J_
				, double delta_)
			: Lx(Lx_)
			, N_steps(N_steps_)
			, lam(lam_)
			, h_app_norm(h_app_norm_)
			, sigma(sigma_)
			, sin_norm(sin_norm_)
			, time_pulse_center(time_pulse_center_)
			, local_pulse_center(local_pulse_center_)
			, J(J_)
			, delta(delta_) {};
};

#endif