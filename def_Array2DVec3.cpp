#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<fftw3.h>
#include<cassert>
#include"llg.hpp"
#include<Eigen/Dense>
using namespace std;

Eigen::Vector3d& Array2DVec3::operator()(std::size_t x, std::size_t t){ 
    assert(x < Lx_ && t < N_steps_);
    return data_[index(x, t)];
}

const Eigen::Vector3d& Array2DVec3::operator()(std::size_t x, std::size_t t) const{
    assert(x < Lx_ && t < N_steps_);
    return data_[index(x, t)];
}

Array2DVec3 operator/(
	const Array2DVec3& a,
	const Array2DVec3& b
){
    assert( a.Lx_ == b.Lx_ && a.N_steps_ == b.N_steps_ );

	Array2DVec3 result(a.Lx_, a.N_steps_);
    for(std::size_t i = 0; i < a.data_.size(); i++)
    {
        result.data_[i] = a.data_[i].array() / b.data_[i].array();
    }
       
	return result;
}
