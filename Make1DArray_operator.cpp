#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<fftw3.h>
#include"llg.hpp"
using namespace std;

Vec3& Make1DArray::operator()(int x){
			return val[x];
		}

const Vec3& Make1DArray::operator()(int x) const{
	return val[x];
}

Make1DArray Make1DArray::operator+(const Make1DArray& other) const{
	Make1DArray result(Lx);
	for(int n=0; n<Lx; n++){
		result.val[n] = val[n] + other.val[n];
	}
	return result;
}

Make1DArray& Make1DArray::operator+=(const Make1DArray& other) {
	Make1DArray result(Lx);
	for(int n=0; n<Lx; n++){
		val[n] += other.val[n];	
	}
	return *this;	
}

Make1DArray operator*(double c, const Make1DArray& a){
	Make1DArray result(a.Lx);
	for(int n=0; n<a.Lx; n++){
		result.val[n] = c * a.val[n];
	}
	return result;
}

Make1DArray operator/(const Make1DArray& a, const Make1DArray& b){
	Make1DArray result(a.Lx);//Should be Lx N_steps?
	for(int n=0; n<a.Lx; n++){
		result.val[n] = a.val[n] / b.val[n];
	}
	return result;
}

void Make1DArray::normalize(){
	for(int n=0; n<Lx; n++){
		val[n].normalize();
	}
}