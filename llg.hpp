#ifndef LLG_H
#define LLG_H

#include <iostream>
#include <vector>
#include "params.hpp"
#include <fftw3.h>
#include <math.h>
#include <filesystem>
using namespace std;

class Vec3
{
public:
	double x, y, z;
	Vec3() : x(0.0), y(0.0), z(0.0) {}
	Vec3(double x_, double y_, double z_)
		: x(x_), y(y_), z(z_) {}
	Vec3 cross(const Vec3 &other) const;
	Vec3 operator+(const Vec3 &other) const;
	Vec3 &operator+=(const Vec3 &other);
	friend Vec3 operator*(double c, const Vec3 &a);
	friend Vec3 operator/(const Vec3 &a, const Vec3 &b);
	void normalize();
};

class Array1DVec3
{
public:
	int Lx;
	vector<Vec3> val;
	Array1DVec3(int Lx_)
		: Lx(Lx_), val(Lx_) {}
	Vec3 &operator()(int x);
	const Vec3 &operator()(int x) const;
	Array1DVec3 operator+(const Array1DVec3 &other) const;
	Array1DVec3 &operator+=(const Array1DVec3 &other);
	friend Array1DVec3 operator*(double c, const Array1DVec3 &a);
	friend Array1DVec3 operator/(const Array1DVec3 &a, const Array1DVec3 &b);
	void normalize();
};

class Array2DVec3
{
private:
    std::size_t Lx_;
    std::size_t N_steps_;
    std::vector<Vec3> data_;
    
    std::size_t index(std::size_t x, std::size_t t) const {
        return x * N_steps_ + t;
    }
    
public:
    Array2DVec3(std::size_t Lx, std::size_t N_steps)
        : Lx_(Lx), N_steps_(N_steps), data_(Lx * N_steps) {}

    // --- access ---
	Vec3& operator()(std::size_t x, std::size_t t);
	const Vec3 &operator()(std::size_t x, std::size_t t) const;
	friend Array2DVec3 operator/(
		const Array2DVec3 &a,
		const Array2DVec3 &b);
    
    // --- get size ---
    std::size_t size_x() const { return Lx_; }
    std::size_t size_t() const { return N_steps_; }
};

Array1DVec3 extract_const_step(
	const Params &p,
	const Array2DVec3 &S_2d,
	int step);

Array1DVec3 extract_const_n(
	const Params &p,
	const Array2DVec3 &S_2d,
	int n);

void initialize(Params &p, Array2DVec3 &S, Array2DVec3 &h_app);

void run_llg(
	const Params &p,
	Array2DVec3 &S,
	const Array2DVec3 &h_app);

Array1DVec3 calc_dSdt(
	const Params &p,
	const Array1DVec3 &S,
	const Array2DVec3 &h_app,
	const Array1DVec3 &h_exc,
	int step);

void input(
	const Params &p,
	Array2DVec3 &S,
	const Array1DVec3 &S_new,
	int step);

void output_data(
	const Params &p,
	const Array1DVec3 &S,
	char axis,
	const std::string &dir);

void output_data(
	const Params &p,
	const Array2DVec3 &S,
	char axis,
	const std::string &dir);

void output_params(const Params &p);

Array1DVec3 calc_h_exc(
	const Params &p,
	const Array1DVec3 &S_old);

Array1DVec3 &fft_1d_time(
	const Params &p,
	Array1DVec3 &S,
	char axis);

double gaussian(
	int n,
	double center,
	double sigma);

void avoid_zero(
	const Params &p,
	Array2DVec3 &h_app);

Array2DVec3 calc_response(
	const Params &p,
	const Array2DVec3 &S,
	const Array2DVec3 &h_app);

std::filesystem::path make_run_dir();

std::string get_git_hash();

void run_python(const std::filesystem::path &dir);

void write_params_file(const Params &p, const std::filesystem::path &dir);

void print_params(const Params &p);

void write_python_params(const Params& p);
#endif
