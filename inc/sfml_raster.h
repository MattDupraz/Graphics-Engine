#pragma once
#include <SFML/Graphics.hpp>
#include "raster.h"

class SFMLRaster : public Raster {
	public:
		SFMLRaster(unsigned int w, unsigned int h);
		~SFMLRaster() override;

		void draw_pixel(unsigned int x, unsigned int y,
				uint8_t r, uint8_t g,
				uint8_t b, uint8_t a = 255) override;

		void update_texture();
		const sf::Sprite& get_sprite() const;

		void resize(unsigned int w, unsigned int h) override;

		unsigned int get_width() const override { return width; }
		unsigned int get_height() const override { return height; }

		void clear() override;

	private:
		unsigned int width;
		unsigned int height;

		sf::Uint8* pixels;
		sf::Texture texture;
		sf::Sprite sprite;
};
