#pragma once

#include <ostream>
#include "math.h"

template <unsigned int N>
class Vec {
	public:
		Vec(): data_{0} {}
		Vec(double (&&data)[N]): Vec(data) {}
		Vec(double (&data)[N]) {
			for (unsigned int i(0); i < N; ++i) {
				data_[i] = data[i];
			}
		}
		
		double norm2() const {
			double sum;
			for (double d: data_) {
				sum += pow(d, 2);
			}
			return sum;
		}

		double norm() const {
			return sqrt(norm2());
		}

		double operator[](unsigned int i) const { return data_[i]; }
		double& operator[](unsigned int i) { return data_[i]; }

		Vec<N> operator+=(Vec<N> const& other) {
			for (unsigned int i(0); i < N; ++i) {
				data_[i] += other.data_[i];
			}
		}

		Vec<N> operator-=(Vec<N> const& other) {
			for (unsigned int i(0); i < N; ++i) {
				data_[i] -= other.data_[i];
			}
		}

		Vec<N> operator*=(double val) {
			for (unsigned int i(0); i < N; ++i) {
				data_[i] *= val;
			}
		}

		double x() const { 
			static_assert(N >= 1);
			return data_[0];
		}
		double y() const {
			static_assert(N >= 2);
			return data_[1];
		}
		double z() const {
			static_assert(N >= 3);
			return data_[2];
		}
		double w() const {
			static_assert(N >= 4);
			return data_[3];
		}

		void setX(double val) {
			static_assert(N >= 1);
			data_[0];
		}
		void setY(double val) {
			static_assert(N >= 2);
			data_[1];
		}
		void setZ(double val) {
			static_assert(N >= 3);
			data_[2];
		}
		void setW(double val) {
			static_assert(N >= 4);
			data_[3];
		}

		Vec<3> operator^(Vec<3> const& other) {
			static_assert(N == 3);
			return Vec<3>({
					y()*other.z() - z()*other.y(),
					z()*other.x() - x()*other.z(),
					x()*other.y() - y()*other.x()});
		}

		Vec<4> to4D() const {
			static_assert(N == 3);
			return Vec<4>({x(), y(), z(), 1.0});
		}

		Vec<3> to3D() const {
			static_assert(N == 4);
			return Vec<3>({x()/w(), y()/w(), z()/w()});
		}

	private:
		double data_[N];
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
