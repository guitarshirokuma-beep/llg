#ifndef LLG_H
#define LLG_H

#include<iostream>
#include<vector>
#include"params.h"
#include<fftw3.h>
#include<math.h>
using namespace std;

class Vec3{
	public:
		double x, y, z;
		Vec3 cross(const Vec3& other) const;
		Vec3 operator+(const Vec3& other) const;
		Vec3& operator+=(const Vec3& other);
		friend Vec3 operator*(double c, const Vec3& a);
		friend Vec3 operator/(const Vec3& a, const Vec3& b);
		void normalize();
};

class Make1DArray{
	public:
		int Lx;
		vector<Vec3> val;
		Make1DArray(int Lx_)
			: Lx(Lx_), val(Lx_) {}
		Vec3& operator()(int x);
		const Vec3& operator()(int x) const;
		Make1DArray operator+(const Make1DArray& other) const;
		Make1DArray& operator+=(const Make1DArray& other);
		friend Make1DArray operator*(double c, const Make1DArray& a);
		friend Make1DArray operator/(const Make1DArray& a, const Make1DArray& b);
		void normalize();
};

class Make2DArray{
	public:
		int Lx, N_steps;
		vector<Vec3> val;
		Make2DArray(int Lx_, int N_steps_)
			: Lx(Lx_), N_steps(N_steps_), val(Lx_ * N_steps_) {}
		Vec3& operator()(int x, int t);
		const Vec3& operator()(int x, int t) const;
		friend Make2DArray operator/(
			const Make2DArray& a,
			const Make2DArray& b
		);
};



Make1DArray extract_const_step(
	const Params&		p,
	const Make2DArray&	S_2d,
	int					step
);

Make1DArray extract_const_n(
	const Params&		p,
	const Make2DArray&	S_2d,
	int 				n
);

void initialize(Params& p, Make2DArray& S, Make2DArray& h_app);

void run_llg(
	const Params&		p,
	Make2DArray&		S,
	const Make2DArray&	h_app
);

Make1DArray calc_dSdt(
	const Params&		p,
	const Make1DArray&	S,
	const Make2DArray&	h_app,
	const Make1DArray&	h_exc,
	int					step
);

void input(
	const Params&		p,
	Make2DArray&		S,
	const Make1DArray&	S_new,
	int					step
);

void output_data(
	const Params&		p,
	const Make1DArray&	S,
	char 				axis
);

void output_data(
	const Params&		p,
	const Make2DArray&	S,
	char				axis
);

void output_params(const Params& p);

Make1DArray calc_h_exc(
	const Params&		p,
	const Make1DArray&	S_old
);

Make1DArray& fft_1d_time(
	const Params&	p,
	Make1DArray&	S,
	char			axis
);

double gaussian(
	int		n,
	double	center,
	double	sigma
);

void avoid_zero(
	const Params& p,
	Make2DArray& h_app
);

Make2DArray calc_response(
	const Params& p,
	const Make2DArray& S,
	const Make2DArray& h_app
);



#endif
