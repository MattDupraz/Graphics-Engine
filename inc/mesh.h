#pragma once

#include <vector>
#include <memory>

#include "matrix.h"

struct Face {
	unsigned int v_indices[3];
};

struct Simplex {
	Vec3D pos[3];

	Vec3D normal() {
		Vec3D v1 = pos[1] - pos[0];
		Vec3D v2 = pos[2] - pos[0];
		return v1 ^ v2;
	}
};

class Mesh {
	public:
		Mesh(){}
		Mesh(std::vector<Vec3D> const& vertices, std::vector<Face> const& faces)
			: vertices(vertices), faces(faces)
		{}
		Mesh(std::vector<double> const& v,
				std::vector<unsigned int> const& indices) {
			for (unsigned int i(0); i < v.size() / 3; ++i) {
				vertices.push_back({v[3*i],  v[3*i + 1], v[3*i + 2]});
			}
			for (unsigned int i(0); i < indices.size() / 3; ++i) {
				faces.push_back({
						indices[3*i],
						indices[3*i + 1],
						indices[3*i + 2]
				});
			}
		}

		Mesh transform(Matrix4D m) {
			std::vector<Vec3D> transformed;
			for (Vec3D& v : vertices) {
				transformed.push_back(m * v);
			}
			return Mesh(transformed, faces);
		}

		Simplex get_simplex(unsigned int i) {
			Face face(faces[i]);
			return {
				vertices[face.v_indices[0]],
				vertices[face.v_indices[1]],
				vertices[face.v_indices[2]],
			};
		}

		unsigned int n_faces() {
			return faces.size();
		}

		unsigned int n_vertices() {
			return vertices.size();
		}
	private:
		std::vector<Vec3D> vertices;
		std::vector<Face> faces;
};
