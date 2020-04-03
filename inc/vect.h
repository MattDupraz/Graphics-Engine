#pragma once

#include <ostream>
#include "math.h"

template <unsigned int N>
struct Vec {
	double coords[N];
		
	double norm2() const {
		double sum;
		for (double d: coords) {
			sum += pow(d, 2);
		}
		return sum;
	}

	double norm() const {
		return sqrt(norm2());
	}

	double operator[](unsigned int i) const { return coords[i]; }
	double& operator[](unsigned int i) { return coords[i]; }

	Vec<N> operator+=(Vec<N> const& other) {
		for (unsigned int i(0); i < N; ++i) {
			coords[i] += other.coords[i];
		}
		return *this;
	}

	Vec<N> operator-=(Vec<N> const& other) {
		for (unsigned int i(0); i < N; ++i) {
			coords[i] -= other.coords[i];
		}
		return *this;
	}

	Vec<N> operator*=(double val) {
		for (unsigned int i(0); i < N; ++i) {
			coords[i] *= val;
		}
		return *this;
	}

	double x() const { 
		static_assert(N >= 1);
		return coords[0];
	}
	double y() const {
		static_assert(N >= 2);
		return coords[1];
	}
	double z() const {
		static_assert(N >= 3);
		return coords[2];
	}
	double w() const {
		static_assert(N >= 4);
		return coords[3];
	}

	void set_x(double val) {
		static_assert(N >= 1);
		coords[0] = val;
	}
	void set_y(double val) {
		static_assert(N >= 2);
		coords[1] = val;
	}
	void set_z(double val) {
		static_assert(N >= 3);
		coords[2] = val;
	}
	void set_w(double val) {
		static_assert(N >= 4);
		coords[3] = val;
	}

	Vec<3> operator^(Vec<3> const& other) {
		static_assert(N == 3);
		return Vec<3>({
				y()*other.z() - z()*other.y(),
				z()*other.x() - x()*other.z(),
				x()*other.y() - y()*other.x()});
	}

	Vec<4> to_4D() const {
		static_assert(N == 3);
		return Vec<4>({x(), y(), z(), 1.0});
	}

	Vec<3> to_3D() const {
		static_assert(N == 4);
		if (w() != 0)
			return Vec<3>({x()/w(), y()/w(), z()/w()});
		else
			return Vec<3>({x(), y(), z()});
	}
};

typedef Vec<2> Vec2D;
typedef Vec<3> Vec3D;
typedef Vec<4> Vec4D;

template<unsigned int N>
Vec<N> operator+(Vec<N> v1, Vec<N> const& v2) {
	return (v1 += v2);
}

template<unsigned int N>
Vec<N> operator-(Vec<N> v1, Vec<N> const& v2) {
	return (v1 -= v2);
}

template<unsigned int N>
Vec<N> operator*(double d, Vec<N> v) {
	return (v *= d);
}

template<unsigned int N>
Vec<N> operator*(Vec<N> v, double d) {
	return (v *= d);
}

template<unsigned int N>
Vec<N> operator~(Vec<N> v) {
	return (v *= (1.0 / v.norm()));
}

template<unsigned int N>
double operator*(Vec<N> v1, Vec<N> v2) {
	double result(0);
	for (unsigned int i(0); i < N; ++i) {
		result += v1[i]  * v2[i];
	}
	return result;
}

template<unsigned int N>
std::ostream &operator<<(std::ostream& os, Vec<N> const& vec) {
	for (unsigned int i(0); i < N; ++i) {
		os << vec[i];
		if (i < N - 1) {
			os << " ";
		}
	}
	return os;
}
