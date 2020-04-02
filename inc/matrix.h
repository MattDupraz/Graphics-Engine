#pragma once

#include "math.h"

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
		static Matrix<4, 4> make_perspective() {
			static_assert(N == M && N == 4);
		}

		// This is the "modern art" way of programming
		class LineView {
			public:
				LineView(Matrix<N, M> *m, unsigned int l):
					matrix_(m), line_(l) {}
				double operator[](unsigned int j) const {
					return matrix_->data_[line_][j];
				}
				double& operator[](unsigned int j) {
					return matrix_->data_[line_][j];
				}
			private:
				Matrix<N, M> *matrix_;
				unsigned int line_;
		};
		LineView const operator[](unsigned int i) const {
			return LineView(this, i);
		}
		LineView operator[](unsigned int i) {
			return LineView(this, i);
		}
		// I don't care if you feel violated

		template<unsigned int L>
		Matrix<N, L> operator*(Matrix<M, L> const& other) {
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

		Vec<N> operator*(Vec<M> const& vec) {
			Vec<N> result;
			for (unsigned int i(0); i < N; ++i) {
				for (unsigned int j(0); j < M; ++j) {
					result[i] += vec[j] * data_[i][j];
				}
			}
			return result;
		}

	private:
		double data_[N][M];
};

typedef Matrix<2, 2> Matrix2D;
typedef Matrix<3, 3> Matrix3D;
typedef Matrix<4, 4> Matrix4D;

