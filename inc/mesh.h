#pragma once

#include <vector>
#include <memory>

#include "matrix.h"

struct Vertex : public Vec4D {
	public:
	  	Vertex(double (&&data)[4]): Vec4D(data) {}
};


struct Face {
	std::shared_ptr<Vertex> vertices[3];

	Vec3D normal() {
		Vec3D v1 = (*vertices[1] - *vertices[0]).to_3D();
		Vec3D v2 = (*vertices[2] - *vertices[0]).to_3D();
		return v1 ^ v2;
	}

};

struct Mesh {
	std::vector<Vertex> vertices;
	std::vector<Face> faces;

	Mesh(){}
	Mesh(std::vector<double> const& v,
			std::vector<unsigned int> const& indices)
	{
		for (unsigned int i(0); i < v.size() / 3; ++i) {
			vertices.push_back(Vertex({v[3*i],  v[3*i + 1], v[3*i + 2], 1.0}));
		}
		for (unsigned int i(0); i < indices.size() / 3; ++i) {
			faces.push_back({
					std::shared_ptr<Vertex>(&vertices[indices[3*i]]),
					std::shared_ptr<Vertex>(&vertices[indices[3*i + 1]]),
					std::shared_ptr<Vertex>(&vertices[indices[3*i + 2]])
					});

		}
	}
};
