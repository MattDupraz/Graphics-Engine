#include "sfml_app.h"
#include "math.h"

#include <iostream>

SFMLApp::SFMLApp(unsigned int width, unsigned int height,
		sf::String const& title)
	: window(sf::VideoMode(width, height), title),
	raster(width, height),
	renderer(&raster)
{}

int SFMLApp::start() {	

	renderer.set_aspect_ratio(float(window.getSize().y)/window.getSize().x);
	renderer.update_perspective();

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			} if (event.type == sf::Event::Resized) {
				unsigned int width = event.size.width;
				unsigned int height = event.size.height;
				sf::FloatRect visibleArea(0, 0, width, height);
				window.setView(sf::View(visibleArea));

				raster.resize(width, height);

				renderer.set_aspect_ratio(float(height)/width);
				renderer.update_perspective();
			}
		}

		window.clear();
		raster.clear();

		draw();
		
		raster.update_texture();
		window.draw(raster.get_sprite());
		
		window.display();
	}

	return 0;
}

void SFMLApp::draw() {

	static std::vector<vec3f> vertices {
		// Face -X
		{-1.0f, -1.0f, -1.0f},
		{-1.0f, -1.0f,  1.0f},
		{-1.0f,  1.0f,  1.0f},

		{-1.0f, -1.0f, -1.0f},
		{-1.0f,  1.0f,  1.0f},
		{-1.0f,  1.0f, -1.0f},

		// Face +X
		{ 1.0f, -1.0f, -1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{ 1.0f, -1.0f,  1.0f},

		{ 1.0f, -1.0f, -1.0f},
		{ 1.0f,  1.0f, -1.0f},
		{ 1.0f,  1.0f,  1.0f},

		// Face -Y
		{-1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f,  1.0f},

		{-1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f,  1.0f},
		{-1.0f, -1.0f,  1.0f},

		// Face +Y
		{-1.0f,  1.0f, -1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{ 1.0f,  1.0f, -1.0f},

		{-1.0f,  1.0f, -1.0f},
		{-1.0f,  1.0f,  1.0f},
		{ 1.0f,  1.0f,  1.0f},

		// Face -Z
		{-1.0f, -1.0f, -1.0f},
		{-1.0f,  1.0f, -1.0f},
		{ 1.0f,  1.0f, -1.0f},

		{-1.0f, -1.0f, -1.0f},
		{ 1.0f,  1.0f, -1.0f},
		{ 1.0f, -1.0f, -1.0f},

		// Face +Z
		{-1.0f, -1.0f,  1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{-1.0f,  1.0f,  1.0f},

		{-1.0f, -1.0f,  1.0f},
		{ 1.0f, -1.0f,  1.0f},
		{ 1.0f,  1.0f,  1.0f}
	};
	static std::vector<vec3f> colors {
		// Face -X
		{ 1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},

		{ 1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},

		// Face +X
		{ 1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},

		{ 1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},

		// Face -Y
		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},

		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},

		// Face +Y
		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},

		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},

		// Face -Z
		{ 0.0f,  0.0f,  1.0f},
		{ 0.0f,  0.0f,  1.0f},
		{ 0.0f,  0.0f,  1.0f},

		{ 0.0f,  0.0f,  1.0f},
		{ 0.0f,  0.0f,  1.0f},
		{ 0.0f,  0.0f,  1.0f},

		// Face +Z
		{ 0.0f,  0.0f,  1.0f},
		{ 0.0f,  0.0f,  1.0f},
		{ 0.0f,  0.0f,  1.0f},

		{ 0.0f,  0.0f,  1.0f},
		{ 0.0f,  0.0f,  1.0f},
		{ 0.0f,  0.0f,  1.0f},
	};
	static std::vector<vec3f> normals {
		// Face -X
		{-1.0f,  0.0f,  0.0f},
		{-1.0f,  0.0f,  0.0f},
		{-1.0f,  0.0f,  0.0f},

		{-1.0f,  0.0f,  0.0f},
		{-1.0f,  0.0f,  0.0f},
		{-1.0f,  0.0f,  0.0f},

		// Face +X
		{ 1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},

		{ 1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},
		{ 1.0f,  0.0f,  0.0f},

		// Face -Y
		{ 0.0f, -1.0f,  0.0f},
		{ 0.0f, -1.0f,  0.0f},
		{ 0.0f, -1.0f,  0.0f},

		{ 0.0f, -1.0f,  0.0f},
		{ 0.0f, -1.0f,  0.0f},
		{ 0.0f, -1.0f,  0.0f},

		// Face +Y
		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},

		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},
		{ 0.0f,  1.0f,  0.0f},

		// Face -Z
		{ 0.0f,  0.0f, -1.0f},
		{ 0.0f,  0.0f, -1.0f},
		{ 0.0f,  0.0f, -1.0f},

		{ 0.0f,  0.0f, -1.0f},
		{ 0.0f,  0.0f, -1.0f},
		{ 0.0f,  0.0f, -1.0f},

		// Face +Z
		{ 0.0f,  0.0f,  1.0f},
		{ 0.0f,  0.0f,  1.0f},
		{ 0.0f,  0.0f,  1.0f},

		{ 0.0f,  0.0f,  1.0f},
		{ 0.0f,  0.0f,  1.0f},
		{ 0.0f,  0.0f,  1.0f},
	};

	static float phi(0.0);
	static float theta(0.0);
	phi += 0.05;
	theta += 0.068;

	matrix4f transform(matrix4f::identity());
	transform = transform * matrix4f::rotation_x(phi);
	transform = transform * matrix4f::rotation_y(theta);

	//std::cout << transform << std::endl;

	renderer.set_model_matrix(transform);

	renderer.render(vertices, colors, normals);
}
