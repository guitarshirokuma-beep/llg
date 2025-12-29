#ifndef LLG_H
#define LLG_H

#include<iostream>
#include<vector>
#include"params.h"
using namespace std;

class Data{
	public:
		double x, y, z;
		Data cross(const Data& other) const;
		Data operator+(const Data& other) const;
		Data& operator+=(const Data& other);
		friend Data operator*(double c, const Data& a);
		void normalize();
};

class Make2DArray{
	public:
		int Lx, N_steps;
		vector<Data> val;
		Make2DArray(int Lx_, int N_steps_)
			: Lx(Lx_), N_steps(N_steps_), val(Lx_ * N_steps_) {}
		Data& operator()(int x, int t);
		const Data& operator()(int x, int t) const;
};

class Make1DArray{
	public:
		int Lx;
		vector<Data> val;
		Make1DArray(int Lx_)
			: Lx(Lx_), val(Lx_) {}
		Data& operator()(int x);
		const Data& operator()(int x) const;
		Make1DArray operator+(const Make1DArray& other) const;
		Make1DArray& operator+=(const Make1DArray& other);
		friend Make1DArray operator*(double c, const Make1DArray& a);
		static Make1DArray extract(const Params& p
								,const Make2DArray& S, int step);
		void normalize();
};

void initialize(Params& p, Make2DArray& S, Make2DArray& h_app);

void run_llg(Params& p, Make2DArray& S, Make2DArray& h_app);

Make1DArray calc_dSdt(Params& p, Make1DArray& S
						, Make2DArray& h_app, Make1DArray& h_exc,int step);

void input(const Params& p,Make2DArray& S, const Make1DArray& S_new, int step);

void output_data(const Params& p, const Make2DArray& S);

void output_data(const Params& p, const Make1DArray& S);

void output_params(const Params& p);

Make1DArray calc_h_exc(const Params& p, const Make1DArray& S_old);

Make1DArray& fft_1d(const Params& p, Make1DArray& S);

#endif
