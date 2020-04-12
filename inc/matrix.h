#pragma once

#include "math.h"
#include <ostream>
#include <array>

#include "vect.h"

template <unsigned int N, unsigned int M>
class matrix {
	public:
		matrix():data_{{0}} {}
		matrix(float (&&data)[N][M]): matrix(data) {}
		matrix(float (&data)[N][M]) {
			for (unsigned int i(0); i < N; ++i) {
				for (unsigned int j(0); j < N; ++j) {
					data_[i][j] = data[i][j];
				}
			}
		}

		static matrix<N, N> scalar(float d) {
			static_assert(N == M);
			matrix<N, N> m;
			for (unsigned int i(0); i < N; ++i) {
				m.data_[i][i] = d;
			}
			return m;
		}
		static matrix<N, N> identity() {
			static_assert(N == M);
			return scalar(1.0);
		}
		static matrix<4, 4> perspective(
				float near, float far, float fov, float aspect) {
			static_assert(N == M && N == 4);
			float t = 1.0 / tan(fov / 2.0);
			matrix<4, 4> m;
			m[0][0] = aspect * t;
			m[1][1] = t;
			m[2][2] = far / (far - near);
			m[2][3] = -near * far / (far - near);
			m[3][2] = 1;
			m[3][3] = 0;
			return m;
		}
		static matrix<4, 4> translation(
				float x, float y, float z) {
			static_assert(N == M && N == 4);
			matrix<4, 4> m = identity();
			m[0][3] = x;
			m[1][3] = y;
			m[2][3] = z;
			return m;
		}
		static matrix<4, 4> rotation_x(float a) {
			static_assert(N == M && N == 4);
			matrix<4, 4> m = identity();
			m[1][1] = cos(a);
			m[1][2] = -sin(a);
			m[2][1] = sin(a);
			m[2][2] = cos(a);
			return m;
		}
		static matrix<4, 4> rotation_y(float a) {
			static_assert(N == M && N == 4);
			matrix<4, 4> m = identity();
			m[0][0] = cos(a);
			m[0][2] = sin(a);
			m[2][0] = -sin(a);
			m[2][2] = cos(a);
			return m;
		}
		static matrix<4, 4> rotation_z(float a) {
			static_assert(N == M && N == 4);
			matrix<4, 4> m = identity();
			m[0][0] = cos(a);
			m[0][1] = -sin(a);
			m[1][0] = sin(a);
			m[1][1] = cos(a);
			return m;
		}

		std::array<float, M> const& operator[](unsigned int i) const {
			return data_[i];
		}
		std::array<float, M>& operator[](unsigned int i) {
			return data_[i];
		}

		template<unsigned int L>
		matrix<N, L> operator*(matrix<M, L> const& other) const {
			matrix<N, L> result;
			for (unsigned int i(0); i < N; ++i) {
				for (unsigned int j(0); j < L; ++j) {
					for (unsigned int k(0); k < M; ++k) {
						result[i][j] += data_[i][k] * other.data_[k][j];
					}
				}
			}
			return result;
		}

	private:
		std::array<std::array<float, M>, N> data_;
};

typedef matrix<2, 2> matrix2f;
typedef matrix<3, 3> matrix3f;
typedef matrix<4, 4> matrix4f;

template<unsigned int N, unsigned int M>
vec<N> operator*(matrix<N, M> const& m, vec<M> const& v) {
	vec<N> result {0};
	for (unsigned int i(0); i < N; ++i) {
		for (unsigned int j(0); j < M; ++j) {
			result[i] += v[j] * m[i][j];
		}
	}
	return result;
}

template<unsigned int N, unsigned int M>
std::ostream &operator<<(std::ostream& os, matrix<N, M> const& mat) {
	for (unsigned int i(0); i < N; ++i) {
		for (unsigned int j(0); j < M; ++j) {
			os << mat[i][j];
			if (j < M - 1) {
				os << " ";
			}
		}
		os << "\n";
	}
	return os;
}
