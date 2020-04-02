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
		std::cout << perspective * view * face.vertices[0]->pos << std::endl;
		Vec3D pos[3] = {
			(transform * face.vertices[0]->pos).to_3D(),
			(transform * face.vertices[1]->pos).to_3D(),
			(transform * face.vertices[2]->pos).to_3D(),
		};
		std::cout << pos[0] << " ||| " << pos[1] << " ||| " << pos[2] << std::endl;
	}
	//while (true) {}
	
	while(true) {
		erase();
		draw();
		refresh();
	}
}

void GraphicsEngine::draw() {
	static double theta = 0;
	theta += 0.0001;
	static double phi = 0;
	phi += 0.000037;
	Matrix4D transform = perspective * view;
	transform = transform * Matrix4D::make_rotation_y(theta);
	transform = transform * Matrix4D::make_rotation_x(phi);
	for (Face& face: mesh.faces) {

		Vec3D pos[3] = {
			(transform * face.vertices[0]->pos).to_3D(),
			(transform * face.vertices[1]->pos).to_3D(),
			(transform * face.vertices[2]->pos).to_3D(),
		};

		for (int i(0); i < 3; ++i) {
			pos[i] += Vec3D({1, 1, 0});
			pos[i].set_x(pos[i].x() * double(width)/2.0);
			pos[i].set_y(pos[i].y() * double(height)/2.0);
		}

		char c = 'X';
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

void GraphicsEngine::drawLine(int x1, int y1, int x2, int y2, char c) {
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

void GraphicsEngine::drawPoint(int x, int y, char c) {
	if (x >= 0 && x < width && y >= 0 && y < height) {
		mvwaddch(stdscr, y, x, c);
	}
}

void GraphicsEngine::eventResize() {
	getmaxyx(stdscr, height, width);
	updatePerspective();
}

void GraphicsEngine::updatePerspective() {
	perspective = Matrix4D::make_perspective(
			nearPlane, farPlane, FOV,
			double(height)*2.0/double(width));
}
