#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<fftw3.h>
#include"llg.hpp"
using namespace std;

Vec3 Vec3::cross(const Vec3& other) const{
	return {
		y*other.z - z*other.y,
		z*other.x - x*other.z,
		x*other.y - y*other.x
	};
}

Vec3 Vec3::operator+(const Vec3& other) const{
	return {
		x + other.x,
		y + other.y,
		z + other.z
	};
}

Vec3 operator*(double c, const Vec3& a){
	return {c * a.x, c * a.y, c* a.z};
}

Vec3 operator/(const Vec3& a, const Vec3& b){
	return { a.x / b.x, a.y / b.y, a.z / b.z };
}

Vec3& Vec3::operator+=(const Vec3& other){
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

void Vec3::normalize(){
	double n = sqrt(x*x + y*y + z*z);
	if(n > 0.0){
		x /= n;
		y /= n;
		z /= n;
	}
	else{
		cout << "size <= 0\n";
	}
}

double Vec3::norm() const 
{
    return sqrt(x*x + y*y + z*z);
}
