#include "sfml_raster.h"

SFMLRaster::SFMLRaster(unsigned int w, unsigned int h)
		: width(w), height(h),
		pixels(new sf::Uint8[w*h*4])	{
	texture.create(w, h);
	sprite.setTexture(texture, true);
}

SFMLRaster::~SFMLRaster() {
	delete pixels;
}

void SFMLRaster::draw_pixel(unsigned int x, unsigned int y,
		uint8_t r, uint8_t g,
		uint8_t b, uint8_t a) {
	unsigned int i = x * 4  + (height - y - 1) * width * 4;
	pixels[i] = r;
	pixels[i + 1] = g;
	pixels[i + 2] = b;
	pixels[i + 3] = a;
}

void SFMLRaster::update_texture() {
	texture.update(pixels);
}

const sf::Sprite& SFMLRaster::get_sprite() const {
	return sprite;
}

void SFMLRaster::resize(unsigned int w, unsigned h) {
	delete pixels;
	pixels = new sf::Uint8[w*h*4];
	texture.create(w, h);
	sprite.setTexture(texture, true);
	width = w;
	height = h;
}

void SFMLRaster::clear() {
	for (unsigned int x(0); x < width; ++x)
	for (unsigned int y(0); y < height; ++y){
		draw_pixel(x, y, 0, 0, 0, 0);
	}
}
