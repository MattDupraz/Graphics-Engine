#pragma once

class Raster {
	public:
		virtual ~Raster() {}

		virtual void draw_pixel(unsigned int x,
				unsigned int y,
				uint8_t r,
				uint8_t g,
				uint8_t b,
				uint8_t a = 255) = 0;
	
		virtual unsigned int get_width() const = 0;
		virtual unsigned int get_height() const = 0;
		
		virtual void resize(unsigned int w, unsigned int h) = 0;

		virtual void clear() = 0;
};
