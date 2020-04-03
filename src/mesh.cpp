#include "mesh.h"

Vec3D Simplex::normal() const {
	Vec3D v1 = pos[1] - pos[0];
	Vec3D v2 = pos[2] - pos[0];
	return v1 ^ v2;
}

bool Simplex::contains(double x, double y) const {
	Vec3D v{x, y, 0};
	Vec3D p1{pos[0].x(), pos[0].y(), 0};
	Vec3D p2{pos[1].x(), pos[1].y(), 0};
	Vec3D p3{pos[2].x(), pos[2].y(), 0};
	Vec3D cp1 = (p1 - v) ^ (p2 - v);
	Vec3D cp2 = (p2 - v) ^ (p3 - v);
	Vec3D cp3 = (p3 - v) ^ (p1 - v);
	bool a = cp1 * cp2 >= 0;
	bool b = cp2 * cp3 >= 0;
	bool c = cp3 * cp1 >= 0;
	return a && b && c;
}

Vec3D Simplex::midpoint() const {
	return (pos[1] + pos[2] + pos[3]) * (1.0/3.0);
}

Mesh::Mesh(std::vector<double> const& v,
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

Mesh Mesh::transform(Matrix4D m) {
	std::vector<Vec3D> transformed;
	for (Vec3D& v : vertices) {
		transformed.push_back(m * v);
	}
	return Mesh(transformed, faces);
}

Simplex Mesh::get_simplex(unsigned int i) {
	Face face(faces[i]);
	return {
		vertices[face.v_indices[0]],
		vertices[face.v_indices[1]],
		vertices[face.v_indices[2]],
	};
}

