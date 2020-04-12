#pragma once

#include <SFML/Graphics.hpp>
#include "sfml_raster.h"
#include "renderer.h"

class SFMLApp {	
	public:
		SFMLApp(unsigned int width, unsigned int height,
				sf::String const& title);
		int start();
		void draw();
	private:
		sf::RenderWindow window;
		sf::Sprite sprite;
		SFMLRaster raster;
		Renderer renderer;
};
