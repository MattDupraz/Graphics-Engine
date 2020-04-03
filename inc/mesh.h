#pragma once

#include <vector>

#include "vect.h"
#include "matrix.h"

struct Face {
	unsigned int v_indices[3];
};

struct Simplex {
	Vec3D pos[3];
	short c = 0;

	Vec3D normal() const;
	bool contains(double x, double y) const;
	Vec3D midpoint() const;
};

class Mesh {
	public:
		Mesh(){}
		Mesh(std::vector<Vec3D> const& vertices, std::vector<Face> const& faces)
			: vertices(vertices), faces(faces)
		{}
		Mesh(std::vector<double> const& v,
				std::vector<unsigned int> const& indices);

		Mesh transform(Matrix4D m);

		Simplex get_simplex(unsigned int i);

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
