#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<fftw3.h>
#include"llg.hpp"
using namespace std;

Vec3& Make2DArray::operator()(int x, int t){
			return val[x * N_steps + t];
		}

const Vec3& Make2DArray::operator()(int x, int t) const{
	return val[x * N_steps + t];
}

Make2DArray operator/(
	const Make2DArray& a,
	const Make2DArray& b
){
	Make2DArray result(a.Lx, a.N_steps);
	for(int n=0; n<a.Lx; n++){
		for(int step=0; step<a.N_steps; step++){
			result(n, step) = a(n, step) / b(n, step);
		}
	}
	return result;
}