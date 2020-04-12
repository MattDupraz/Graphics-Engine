#pragma once

#include "matrix.h"
#include "vect.h"
#include "raster.h"

#include "math.h"
#include <vector>

struct attribute {
	vec3f pos;
	vec3f color;
	vec3f normal;
};

struct varying {
	vec3f pos;
	vec3f color;
	vec3f normal;
};

class Renderer {
	public:
		Renderer(Raster* raster)
			:raster(raster) {}

		void render(std::vector<vec3f> const& vertices,
				std::vector<vec3f> const& colors,
				std::vector<vec3f> const& normals);

		void render_triangle(vec3f const* vertices,
				vec3f const* colors,
				vec3f const* normals);

		void vertex_shader(attribute const& in, vec4f& gl_Position, varying& out);

		void fragment_shader(varying const& in, vec4f& gl_FragColor);
		
		void set_model_matrix(matrix4f const& m) { model = m; }
		void set_view_matrix (matrix4f const& m) { view = m; }

		void set_near_plane(float val) { near_plane = val; }
		void set_far_plane(float val) { aspect_ratio = val; }
		void set_fov(float val) { aspect_ratio = val; }
		void set_aspect_ratio(float val) { aspect_ratio = val; }

		void update_perspective() {
			perspective = matrix4f::perspective(
					near_plane, far_plane, fov, aspect_ratio);
		}


	private:
		Raster* raster;

		float near_plane = 0.1f;
		float far_plane = 1000.0f;
		float fov = M_PI/2.0f;
		float aspect_ratio = 1.0f;
		
		// Uniforms
		matrix4f model = matrix4f::identity();
		matrix4f perspective = matrix4f::identity();
		matrix4f view = matrix4f::translation(0.0, 0.0, 4.0f);

		vec3f light_pos {2.0f, 2.0, -5.0f};
		vec3f light_color {1.0f, 1.0f, 1.0f};

		float ambient_strength = 0.2;
};
