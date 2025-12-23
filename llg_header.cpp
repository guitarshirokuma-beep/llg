#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include"llg.h"

Data Data::cross(const Data& other) const{
	return {
		y*other.z - z*other.y,
		z*other.x - x*other.z,
		x*other.y - y*other.x
	};
}

Data Data::operator+(const Data& other) const{
	return {
		x + other.x,
		y + other.y,
		z + other.z
	};
}

Data operator*(double c, const Data& a){
	return {c * a.x, c * a.y, c* a.z};
}

void Data::normalize(){
	double n = sqrt(x*x + y*y + z*z);
	if(n > 0.0){
		x /= n;
		y /= n;
		z /= n;
	}
	else{
		cout << "サイズがゼロ以下です\n";
	}
}

Data& Make2DArray::operator()(int x, int t){
			return val[x * N_steps + t];
		}

const Data& Make2DArray::operator()(int x, int t) const{
	return val[x * N_steps + t];
}

Data& Make1DArray::operator()(int x){
			return val[x];
		}

const Data& Make1DArray::operator()(int x) const{
	return val[x];
}

Make1DArray Make1DArray::operator+(const Make1DArray& other) const{
	Make1DArray result(Lx);
	for(int n=0; n<Lx; n++){
		result.val[n] = val[n] + other.val[n];
	}
	return result;
}

Make1DArray operator*(double c, const Make1DArray& a){
	Make1DArray result(a.Lx);
	for(int n=0; n<a.Lx; n++){
		result.val[n] = c * a.val[n];
	}
	return result;
}

Make1DArray Make1DArray::extract(const Params& p 
									,const Make2DArray& S, int step){
	Make1DArray S_old(p.Lx);
	for(int n=0; n<p.Lx; n++){
		S_old(n) = S(n, step); 
	}
	return S_old;
}

void Make1DArray::normalize(){
	for(int n=0; n<Lx; n++){
		val[n].normalize();
	}
}

void initialize(Params& p, Make2DArray& S, Make2DArray& h_app){
	for(int n=0; n<p.Lx; n++){
		for(int step=0; step<p.N_steps; step++){
			h_app(n, step).x = 0.0;
			h_app(n, step).y = 0.0;
			h_app(n, step).z = p.h_app_norm;
		}
	}

	int step = 0;
	for(int n=0; n<p.Lx; n++){
		S(n, step).x = 1.0;
		S(n, step).y = 0.0;
		S(n, step).z = 0.0;
	}
}

void input(const Params& p 
			,Make2DArray& S,const Make1DArray& S_new, int step){
	for(int n=0; n<p.Lx; n++){
		S(n, step) = S_new(n);
	}
}

Make1DArray calc_dSdt(Params& p, Make1DArray& S_step
						, Make2DArray& h_app, int step){
	Make1DArray dS_over_dt(p.Lx);
	for(int n=0; n<p.Lx; n++){
		const Data& S_ = S_step(n);
		const Data& h_ = h_app(n, step);

		Data Sxh = S_step(n).cross(h_app(n, step));
		Data SxSxh = S_step(n).cross(Sxh);

		double c = (-p.gamma)/(1.0 + p.lam*p.lam);

		dS_over_dt(n) = c*(Sxh + p.lam*SxSxh);
	}

	return dS_over_dt;
}

void run_llg(Params& p, Make2DArray& S, Make2DArray& h_app){
	for(int step=0; step<p.N_steps-1; step++){
		Make1DArray S_old = Make1DArray::extract(p, S, step);
		Make1DArray dS_over_dt = calc_dSdt(p, S_old, h_app, step);
		Make1DArray S_pred = S_old + p.dt * dS_over_dt;
		S_pred.normalize();
		Make1DArray dS_over_dt_2 = calc_dSdt(p, S_pred, h_app, step);
		Make1DArray S_new = S_old + 0.5*p.dt*( dS_over_dt + dS_over_dt_2 );
		S_new.normalize();
		input(p, S, S_new, step+1);
	}
	//ofstream ofs("test.dat");
	int step = 0;
	for(int step=0; step<p.N_steps; step++){
		for(int n=0; n<p.Lx; n++){
			cout << "S(" << n << "," << step << ").x = " << S(n, step).x << "\n";
			cout << "S(" << n << "," << step << ").y = " << S(n, step).y << "\n";
			cout << "S(" << n << "," << step << ").z = " << S(n, step).z << "\n";
			cout << "\n";
		}
	}

}

void output_data(const Params& p, const Make2DArray& S){
    ofstream ofs("llg.dat");
	int n = 0;
	ofs << "# step S(n=0, step).x\n";
	for(int step=0; step<p.N_steps; step++){
		ofs << step << " " << S(n, step).x << "\n";
	}
}