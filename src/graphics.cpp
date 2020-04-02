#include "graphics.h"
#include <vector>
#include <utility>
#include <iostream>

GraphicsEngine::~GraphicsEngine() {
	endwin();
}

void GraphicsEngine::init()
{	
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	curs_set(0);

	getmaxyx(stdscr, height, width);
	drawWidth = 2 * width;
	drawHeight = 2 * height;
	updatePerspective();

	view = Matrix4D::make_translation(0, 0, 3);

	mesh = Mesh({
				0.0, 0.0, 0.0,
				0.0, 1.0, 0.0,
				1.0, 1.0, 0.0,
				1.0, 0.0, 0.0,
				0.0, 0.0, 1.0,
				0.0, 1.0, 1.0,
				1.0, 1.0, 1.0,
				1.0, 0.0, 1.0
			}, {
				0, 1, 2,
				0, 2, 3,

				1, 5, 6,
				1, 6, 2,

				0, 7, 4,
				0, 3, 7,

				3, 2, 6,
				3, 6, 7,

				4, 5, 1,
				4, 1, 0,

				4, 7, 6,
				4, 6, 5
			});
}

void GraphicsEngine::start() {
	for (Face& face: mesh.faces) {
		Matrix4D transform = perspective * view;
		std::cout << perspective * view * (*face.vertices[0]) << std::endl;
		Vec3D pos[3] = {
			(transform * *face.vertices[0]).to_3D(),
			(transform * *face.vertices[1]).to_3D(),
			(transform * *face.vertices[2]).to_3D(),
		};
		std::cout << pos[0] << " ||| " << pos[1] << " ||| " << pos[2] << std::endl;
	}
	//while (true) {}
	
	while(true) {
		erase();
		clearBuffer();
		draw();
		renderBuffer();
		refresh();
	}
}

void GraphicsEngine::clearBuffer() {
	buffer = std::vector<std::vector<short>>(
			height * 2, std::vector<short>(width * 2, 0));
}

void GraphicsEngine::renderBuffer() {
	static wchar_t chars[16] = { L'\u2800', L'\u2598', L'\u259D',
  	L'\u2580', L'\u2596', L'\u258C', L'\u259E', L'\u259B',
	L'\u2597', L'\u259A', L'\u2590', L'\u259C', L'\u2584',
	L'\u2599', L'\u259F', L'\u2588'};

	for (std::size_t y(0); y < drawHeight; y += 2) {
		for (std::size_t x(0); x < drawWidth; x += 2) {
			short type = 0;
			type |= buffer[y][x];
			type |= buffer[y][x + 1] << 1;
			type |= buffer[y + 1][x] << 2;
			type |= buffer[y + 1][x + 1] << 3;
			if (type != 0) {
				move(y/2, x/2);
				cchar_t c{A_NORMAL, chars[type]};
				add_wch(&c);
			}	
		}
	}
}

void GraphicsEngine::draw() {
	static double theta = 0;
	theta += 0.001;
	static double phi = 0;
	phi += 0.00037;
	Matrix4D transform = perspective * view;
	transform = transform * Matrix4D::make_rotation_y(theta);
	transform = transform * Matrix4D::make_rotation_x(phi);
	for (Face& face: mesh.faces) {

		Vec3D pos[3] = {
			(transform * *face.vertices[0]).to_3D(),
			(transform * *face.vertices[1]).to_3D(),
			(transform * *face.vertices[2]).to_3D(),
		};

		for (int i(0); i < 3; ++i) {
			pos[i] += Vec3D({1, 1, 0});
			pos[i].set_x(pos[i].x() * double(drawWidth)/2.0);
			pos[i].set_y(pos[i].y() * double(drawHeight)/2.0);
		}
		// 2597 - 259F
		//cchar_t c { A_NORMAL, L'\u2588'};
		short c(1);
		if (((pos[1] - pos[0]) ^ (pos[2] - pos[0])).z() < 0.0) {
			drawLine(int(pos[0].x()), int(pos[0].y()),
					int(pos[1].x()), int(pos[1].y()), c);
			drawLine(int(pos[1].x()), int(pos[1].y()),
					int(pos[2].x()), int(pos[2].y()), c);
			drawLine(int(pos[2].x()), int(pos[2].y()),
					int(pos[0].x()), int(pos[0].y()), c);
		}
	}
}

void GraphicsEngine::drawLine(int x1, int y1, int x2, int y2, short c) {
	int dx = x2 - x1;
	int dy = y2 - y1;

	if (dx != 0 && abs(dx) >= abs(dy)) {
		if (x1 > x2) {
			std::swap(x1, x2);
			std::swap(y1, y2);
		}
		for (int x(x1); x <= x2; ++x) {
				int y = y1 + dy * (x - x1) / dx;
				drawPoint(x, y, c);
		}
	} else if(dy != 0) {
		if (y1 > y2) {
			std::swap(x1, x2);
			std::swap(y1, y2);
		}
		for (int y(y1); y <= y2; ++y) {
				int x = x1 + dx * (y - y1) / dy;
				drawPoint(x, y, c);
		}
	}
}	

void GraphicsEngine::drawPoint(int x, int y, short c) {
	if (x >= 0 && x < drawWidth && y >= 0 && y < drawHeight) {
		buffer[y][x] = c;
	}
}

void GraphicsEngine::eventResize() {
	getmaxyx(stdscr, height, width);
	drawWidth = 2 * width;
	drawHeight = 2 * height;
	updatePerspective();
}

void GraphicsEngine::updatePerspective() {
	perspective = Matrix4D::make_perspective(
			nearPlane, farPlane, FOV,
			double(height)*2.0/double(width));
}
