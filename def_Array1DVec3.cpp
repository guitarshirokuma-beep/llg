#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<fftw3.h>
#include"llg.hpp"
using namespace std;

Vec3& Array1DVec3::operator()(int x){
			return val[x];
		}

const Vec3& Array1DVec3::operator()(int x) const{
	return val[x];
}

Array1DVec3 Array1DVec3::operator+(const Array1DVec3& other) const{
	Array1DVec3 result(Lx);
	for(int n=0; n<Lx; n++){
		result.val[n] = val[n] + other.val[n];
	}
	return result;
}

Array1DVec3& Array1DVec3::operator+=(const Array1DVec3& other) {
	Array1DVec3 result(Lx);
	for(int n=0; n<Lx; n++){
		val[n] += other.val[n];	
	}
	return *this;	
}

Array1DVec3 operator*(double c, const Array1DVec3& a){
	Array1DVec3 result(a.Lx);
	for(int n=0; n<a.Lx; n++){
		result.val[n] = c * a.val[n];
	}
	return result;
}

Array1DVec3 operator/(const Array1DVec3& a, const Array1DVec3& b){
	Array1DVec3 result(a.Lx);//Should be Lx N_steps?
	for(int n=0; n<a.Lx; n++){
		result.val[n] = a.val[n] / b.val[n];
	}
	return result;
}

void Array1DVec3::normalize(){
	for(int n=0; n<Lx; n++){
		val[n].normalize();
	}
}
