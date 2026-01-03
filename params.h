#ifndef PARAMS_H
#define PARAMX_H

class Params{
	public:
		int Lx;
		int N_steps;
		double lam;
		double h_app_norm;
		double pulse_norm;
		const double dt = 0.01;
		const double gamma = 1.0;
		const double sigma = 1.0;
		double J;
		Params(int Lx_
				, int N_steps_
				, double lam_
				, double h_app_norm_
				, double pulse_norm_
				, double J_)
			: Lx(Lx_)
			, N_steps(N_steps_)
			, lam(lam_)
			, h_app_norm(h_app_norm_)
			, pulse_norm(pulse_norm_)
			, J(J_) {};
};

#endif