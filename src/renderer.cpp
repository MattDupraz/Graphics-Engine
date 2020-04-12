#include "renderer.h"
#include "math.h"
#include <algorithm>

#include <iostream>

void Renderer::render(std::vector<vec3f> const& vertices,
		std::vector<vec3f> const& colors,
		std::vector<vec3f> const& normals) {
	for (int i(0); i < vertices.size() / 3; ++i) {
		render_triangle(vertices.data() + i * 3,
				colors.data() + i * 3, normals.data() + i * 3);
	}
}

void Renderer::render_triangle(vec3f const* vertices,
		vec3f const* colors,
		vec3f const* normals) {
	varying var[3];
	vec4f vertex_pos[3];
	
	vec2f lower_left { float(raster->get_width()),
		float(raster->get_height()) };
	vec2f upper_right {0.0f, 0.0f};	
	for (int i(0); i < 3; ++i) {
		attribute attr {
			vertices[i],
			colors[i],
			normals[i]	
		};

		vertex_shader(attr, vertex_pos[i], var[i]);
		
		vec4f& pos(vertex_pos[i]);

		// Perform perspective division
		float w = 1.0f/pos.w();
		pos *= w;
		// We store 1/w in the 4th coordinate to use it later
		pos.set_w(w);

		// Convert to window coordinates
		pos.set_x(raster->get_width() * ((pos.x() + 1.0f) / 2));
		pos.set_y(raster->get_height() * ((pos.y() + 1.0f) / 2));

		// Expand bounding box
		if (pos.x() < lower_left.x())
			lower_left.set_x(pos.x());
		if (pos.y() < lower_left.y())
			lower_left.set_y(pos.y());
		if (pos.x() > upper_right.x())
			upper_right.set_x(pos.x());
		if (pos.y() > upper_right.y())
			upper_right.set_y(pos.y());
	}

	// Triangle vertices
	vec4f const& A(vertex_pos[0]);
	vec4f const& B(vertex_pos[1]);
	vec4f const& C(vertex_pos[2]);

	// Connecting vectors
	vec4f AB = B - A;
	vec4f AC = C - A;

	if ((AC.to_3D() ^ AB.to_3D()).z() < 0.0f) return;

	// Precalculate part of the barycentric coordinates
	float d(AC.y() * AB.x() - AC.x() * AB.y());
	float beta_0((AC.x() * A.y() - AC.y() * A.x()) / d);
	float gamma_0((AB.y() * A.x() - AB.x() * A.y()) / d);

	// Triangle bounding box (clipped to viewport)
	int min_x(std::max(int(lower_left.x() + 0.5f), 0));
	int min_y(std::max(int(lower_left.y() + 0.5f), 0));
	int max_x(std::min(int(upper_right.x() + 0.5f), int(raster->get_width()) - 1));
	int max_y(std::min(int(upper_right.y() + 0.5f), int(raster->get_height()) - 1));

	for (int x(min_x); x <= max_x; ++x)
	for (int y(min_y); y <= max_y; ++y) {
		vec4f frag_coord;
		frag_coord.set_x(double(x) + 0.5);
		frag_coord.set_y(double(y) + 0.5);

		// Barycentric coordinates (P = A + beta*AB + gamma*AC)
		float beta = beta_0 + (AC.y() * frag_coord.x() - AC.x() * frag_coord.y()) / d;
		float gamma = gamma_0 + (AB.x() * frag_coord.y() - AB.y() * frag_coord.x()) / d;
		float alpha = 1 - beta - gamma;
		
		if (beta < 0.0f || gamma < 0.0f || beta + gamma > 1.0f)
			continue;

		// Linear interpolation of z and w coords
		frag_coord.set_z(A.z() + beta*AB.z() + gamma*AC.z());
		frag_coord.set_w(A.w() + beta*AB.w() + gamma*AC.w());

		// Clip depth to frustum
		if (frag_coord.z() < 0.0f || frag_coord.z() > 1.1f)
			continue;

		float a = alpha * A.w();
		float b = beta * B.w();
		float c = gamma * C.w();
		float q = 1.0f / (a + b + c);
		a *= q;
		b *= q;
		c *= q;

		varying interpolated {
			a * var[0].pos + b * var[1].pos + c * var[2].pos,
			a * var[0].color + b * var[1].color + c * var[2].color,
			a * var[0].normal + b * var[1].normal + c * var[2].normal,
		};


		vec4f frag_color;

		fragment_shader(interpolated, frag_color);

		raster->draw_pixel(x, y,
				int(frag_color[0] * 255.0f + 0.5f),
				int(frag_color[1] * 255.0f + 0.5f),
				int(frag_color[2] * 255.0f + 0.5f),
				int(frag_color[3] * 255.0f + 0.5f));
	}
	
}

void Renderer::vertex_shader(attribute const& in, vec4f& vertex_pos, varying& out) {
	vertex_pos = (perspective * view * model) * in.pos.to_4D(1.0f);	
	out.color = in.color;
	out.normal = (model * in.normal.to_4D(1.0f)).to_3D();
	out.pos = (model * in.pos.to_4D(1.0f)).to_3D();
}

void Renderer::fragment_shader(varying const& in, vec4f& frag_color) {
	vec3f ambient(ambient_strength * light_color);
	vec3f light_vec(~(light_pos - in.pos));
	float diffusion_strength(std::max(0.0f, in.normal * light_vec));
	vec3f diffusion(diffusion_strength * light_color);
	vec3f combined(ambient + diffusion);
	frag_color = {
		std::min(combined[0], 1.0f) * in.color[0],
		std::min(combined[1], 1.0f) * in.color[1],
		std::min(combined[2], 1.0f) * in.color[2],
		1.0f
	};
}

