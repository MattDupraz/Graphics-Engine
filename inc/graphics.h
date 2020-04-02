#pragma once

#include<vector>

#include "vec3d.h"

struct triangle
{
	vec3d p[3];
};

struct mesh
{
	std::vector<triangle> triangles;
};

class GraphicsEngine {
	public:
		GraphicsEngine()
		{}

	private:
}
