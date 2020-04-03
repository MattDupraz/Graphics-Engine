#pragma once

#include "math.h"
#include <ostream>
#include <array>

#include "vect.h"

template <unsigned int N, unsigned int M>
class Matrix {
	public:
		Matrix():data_{{0}} {}
		Matrix(double (&&data)[N][M]): Matrix(data) {}
		Matrix(double (&data)[N][M]) {
			for (unsigned int i(0); i < N; ++i) {
				for (unsigned int j(0); j < N; ++j) {
					data_[i][j] = data[i][j];
				}
			}
		}
		static Matrix<N, N> make_scalar(double d) {
			static_assert(N == M);
			Matrix<N, N> m;
			for (unsigned int i(0); i < N; ++i) {
				m.data_[i][i] = d;
			}
			return m;
		}
		static Matrix<N, N> make_identity() {
			static_assert(N == M);
			return make_scalar(1.0);
		}
		static Matrix<4, 4> make_perspective(
				double near, double far, double fov, double aspect) {
			static_assert(N == M && N == 4);
			double t = 1.0 / tan(fov / 2.0);
			Matrix<4, 4> m;
			m[0][0] = aspect * t;
			m[1][1] = t;
			m[2][2] = far / (far - near);
			m[2][3] = -near * far / (far - near);
			m[3][2] = 1;
			m[3][3] = 0;
			return m;
		}
		static Matrix<4, 4> make_translation(
				double x, double y, double z) {
			static_assert(N == M && N == 4);
			Matrix<4, 4> m = make_identity();
			m[0][3] = x;
			m[1][3] = y;
			m[2][3] = z;
			return m;
		}
		static Matrix<4, 4> make_rotation_x(double a) {
			static_assert(N == M && N == 4);
			Matrix<4, 4> m = make_identity();
			m[1][1] = cos(a);
			m[1][2] = -sin(a);
			m[2][1] = sin(a);
			m[2][2] = cos(a);
			return m;
		}
		static Matrix<4, 4> make_rotation_y(double a) {
			static_assert(N == M && N == 4);
			Matrix<4, 4> m = make_identity();
			m[0][0] = cos(a);
			m[0][2] = sin(a);
			m[2][0] = -sin(a);
			m[2][2] = cos(a);
			return m;
		}
		static Matrix<4, 4> make_rotation_z(double a) {
			static_assert(N == M && N == 4);
			Matrix<4, 4> m = make_identity();
			m[0][0] = cos(a);
			m[0][1] = -sin(a);
			m[1][0] = sin(a);
			m[1][1] = cos(a);
			return m;
		}

		std::array<double, M> const& operator[](unsigned int i) const {
			return data_[i];
		}
		std::array<double, M>& operator[](unsigned int i) {
			return data_[i];
		}

		template<unsigned int L>
		Matrix<N, L> operator*(Matrix<M, L> const& other) const {
			Matrix<N, L> result;
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
		std::array<std::array<double, M>, N> data_;
};

typedef Matrix<2, 2> Matrix2D;
typedef Matrix<3, 3> Matrix3D;
typedef Matrix<4, 4> Matrix4D;

template<unsigned int N, unsigned int M>
Vec<N> operator*(Matrix<N, M> const& m, Vec<M> const& vec) {
	Vec<N> result;
	for (unsigned int i(0); i < N; ++i) {
		for (unsigned int j(0); j < M; ++j) {
			result[i] += vec[j] * m[i][j];
		}
	}
	return result;
}

inline Vec3D operator*(Matrix4D const& m, Vec3D const& v) {
	return (m * v.to_4D()).to_3D();
}

template<unsigned int N, unsigned int M>
std::ostream &operator<<(std::ostream& os, Matrix<N, M> const& mat) {
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
