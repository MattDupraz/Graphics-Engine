#pragma once

#include <ostream>
#include "math.h"

template <unsigned int N>
struct vec {
	float coords[N];

	float norm2() const {
		float sum;
		for (float d: coords) {
			sum += pow(d, 2);
		}
		return sum;
	}

	float norm() const {
		return sqrt(norm2());
	}

	float operator[](unsigned int i) const { return coords[i]; }
	float& operator[](unsigned int i) { return coords[i]; }

	vec<N> operator+=(vec<N> const& other) {
		for (unsigned int i(0); i < N; ++i) {
			coords[i] += other.coords[i];
		}
		return *this;
	}

	vec<N> operator-=(vec<N> const& other) {
		for (unsigned int i(0); i < N; ++i) {
			coords[i] -= other.coords[i];
		}
		return *this;
	}

	vec<N> operator*=(float val) {
		for (unsigned int i(0); i < N; ++i) {
			coords[i] *= val;
		}
		return *this;
	}

	float x() const { 
		static_assert(N >= 1);
		return coords[0];
	}
	float y() const {
		static_assert(N >= 2);
		return coords[1];
	}
	float z() const {
		static_assert(N >= 3);
		return coords[2];
	}
	float w() const {
		static_assert(N >= 4);
		return coords[3];
	}

	void set_x(float val) {
		static_assert(N >= 1);
		coords[0] = val;
	}
	void set_y(float val) {
		static_assert(N >= 2);
		coords[1] = val;
	}
	void set_z(float val) {
		static_assert(N >= 3);
		coords[2] = val;
	}
	void set_w(float val) {
		static_assert(N >= 4);
		coords[3] = val;
	}

	vec<3> operator^(vec<3> const& other) {
		static_assert(N == 3);
		return vec<3>({
				y()*other.z() - z()*other.y(),
				z()*other.x() - x()*other.z(),
				x()*other.y() - y()*other.x()});
	}

	vec<4> to_4D(float val = 1.0) const {
		static_assert(N == 3);
		return vec<4> {
			x(), y(), z(), val
		};
	}

	vec<3> to_3D() const {
		static_assert(N == 4);
		return vec<3> {
			x(), y(), z()
		};
	}

};

typedef vec<2> vec2f;
typedef vec<3> vec3f;
typedef vec<4> vec4f;

template<unsigned int N>
vec<N> operator+(vec<N> v1, vec<N> const& v2) {
	return (v1 += v2);
}

template<unsigned int N>
vec<N> operator-(vec<N> v1, vec<N> const& v2) {
	return (v1 -= v2);
}

template<unsigned int N>
vec<N> operator*(float d, vec<N> v) {
	return (v *= d);
}

template<unsigned int N>
vec<N> operator*(vec<N> v, float d) {
	return (v *= d);
}

template<unsigned int N>
vec<N> operator~(vec<N> v) {
	return (v *= (1.0 / v.norm()));
}

template<unsigned int N>
float operator*(vec<N> v1, vec<N> v2) {
	float result(0);
	for (unsigned int i(0); i < N; ++i) {
		result += v1[i]  * v2[i];
	}
	return result;
}

template<unsigned int N>
std::ostream &operator<<(std::ostream& os, vec<N> const& vec) {
	for (unsigned int i(0); i < N; ++i) {
		os << vec[i];
		if (i < N - 1) {
			os << " ";
		}
	}
	return os;
}
