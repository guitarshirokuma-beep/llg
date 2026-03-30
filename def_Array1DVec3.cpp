#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<fftw3.h>
#include<cassert>
#include"llg.hpp"
#include<Eigen/Dense>
using namespace std;

Eigen::Vector3d& Array1DVec3::operator()(std::size_t x){
			return data_[x];
		}

const Eigen::Vector3d& Array1DVec3::operator()(size_t x) const{
	return data_[x];
}

Array1DVec3 Array1DVec3::operator+(const Array1DVec3& other) const{
    assert(Lx_ == other.Lx_);

	Array1DVec3 result(Lx_);
	for(std::size_t i=0; i<result.data_.size(); ++i)
    {
        result.data_[i] = data_[i].array() + other.data_[i].array();
	}
	return result;
}

Array1DVec3& Array1DVec3::operator+=(const Array1DVec3& other)
{
    assert(Lx_ == other.Lx_);
    
	for(std::size_t i=0; i<Lx_; ++i)
    {
		data_[i].array() += other.data_[i].array();	
	}
    
	return *this;	
}

Array1DVec3 operator*(const double c, const Array1DVec3& a){
	Array1DVec3 result(a.Lx_);
	for(std::size_t i=0; i<a.Lx_; ++i)
    {
		result.data_[i].array() = c * a.data_[i].array();
	}
	return result;
}

Array1DVec3 operator/(const Array1DVec3& a, const Array1DVec3& b)
{
    assert(a.Lx_ == b.Lx_);
    
	Array1DVec3 result(a.Lx_);	
    for(std::size_t i=0; i<a.Lx_; ++i)
    {
        assert(b.data_[i].norm() != 0.0);
		result.data_[i].array() = a.data_[i].array() / b.data_[i].array();
	}
	return result;
}

void Array1DVec3::normalize(){
	for(std::size_t i=0; i<Lx_; ++i){
		data_[i].normalize();
	}
}
