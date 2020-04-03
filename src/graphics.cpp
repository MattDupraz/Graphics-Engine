#include "graphics.h"
#include <vector>
#include <utility>
#include <iostream>
#include "math.h"

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
	drawWidth = width;
	drawHeight = height;
	updatePerspective();

	view = Matrix4D::make_translation(0, 0, 3);

	mesh = Mesh({
				-1.0, -1.0, -1.0,
				-1.0, 1.0, -1.0,
				1.0, 1.0, -1.0,
				1.0, -1.0, -1.0,
				-1.0, -1.0, 1.0,
				-1.0, 1.0, 1.0,
				1.0, 1.0, 1.0,
				1.0, -1.0, 1.0
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
			drawHeight, std::vector<short>(drawWidth, 0));
}

void GraphicsEngine::renderBuffer() {
	static wchar_t chars[16] = { L'\u2800', L'\u2598', L'\u259D',
  	L'\u2580', L'\u2596', L'\u258C', L'\u259E', L'\u259B',
	L'\u2597', L'\u259A', L'\u2590', L'\u259C', L'\u2584',
	L'\u2599', L'\u259F', L'\u2588'};
	static wchar_t shades[5] = {L'\u2800', L'\u2591',
		L'\u2592', L'\u2593', L'\u2588'};
	
	int step = drawWidth/width;
	for (std::size_t y(0); y < drawHeight; y += step) {
		for (std::size_t x(0); x < drawWidth; x += step) {
			short type = buffer[y][x];
			//type |= buffer[y][x];
			//type |= buffer[y][x + 1] << 1;
			//type |= buffer[y + 1][x] << 2;
			//type |= buffer[y + 1][x + 1] << 3;
			if (type != 0) {
				move(y/step, x/step);
				cchar_t c{A_NORMAL, shades[type]};
				add_wch(&c);
			}	
		}
	}
}

void GraphicsEngine::draw() {
	static double theta = 0;
	theta += 0.0002;
	static double phi = 0;
	phi += 0.00027;
	Matrix4D view_matrix = perspective * view;
	Matrix4D transform_matrix = Matrix4D::make_rotation_y(theta);
	transform_matrix = transform_matrix * Matrix4D::make_rotation_x(phi);

	Mesh transformed_mesh = mesh.transform(view_matrix * transform_matrix);

	std::vector<Simplex> simplices;
	for (unsigned int i(0); i < transformed_mesh.n_faces(); ++i) {
		Simplex s = transformed_mesh.get_simplex(i);
		s.c = short(((transform_matrix * ~mesh.get_simplex(i).normal()).y() + 1.0) * 2.49);
		if (s.c == 0) s.c = 1;

		if (s.midpoint().z() > 0.0 && s.normal().z() < 0.0) {
			for (int i(0); i < 3; ++i) {
				s.pos[i][1] *= -1;
				s.pos[i] += Vec3D({1, 1, 0});
				s.pos[i][0] *= double(drawWidth)/2.0;
				s.pos[i][1] *= double(drawHeight)/2.0;
			}

			simplices.push_back(s);
		}
	}

	rasterize(simplices);
	for (Simplex const& s : simplices) {
		fillSimplex(s, s.c);
		drawLine({s.pos[0].x(), s.pos[0].y()}, {s.pos[1].x(), s.pos[1].y()}, 3);
		drawLine({s.pos[1].x(), s.pos[1].y()}, {s.pos[2].x(), s.pos[2].y()}, 3);
		drawLine({s.pos[2].x(), s.pos[2].y()}, {s.pos[0].x(), s.pos[0].y()}, 3);
		//break;
	}
}

void GraphicsEngine::fillFlatBottomTriangle(Vec2D p1, Vec2D p2, Vec2D p3, short c) {
	double dy(p2.y() - p1.y());
	double dx1(p2.x() - p1.x());
	double dx2(p3.x() - p1.x());

	double curr_x1(p1.x());
	double curr_x2(p1.x());
	for (int y(p1.y()); y <= p2.y(); ++y) {
		drawLine({curr_x1, y}, {curr_x2, y}, c);
		curr_x1 = p1.x() - (dx1 * (p1.y() - y - 1)) / dy;
		curr_x2 = p1.x() - (dx2 * (p1.y() - y - 1)) / dy;
	}
}

void GraphicsEngine::fillFlatTopTriangle(Vec2D p1, Vec2D p2, Vec2D p3, short c) {
	double dy(p3.y() - p1.y());
	double dx1(p3.x() - p1.x());
	double dx2(p3.x() - p2.x());

	double curr_x1(p3.x());
	double curr_x2(p3.x());
	for (int y(p3.y()); y > p1.y(); --y) {
		drawLine({curr_x1, y}, {curr_x2, y}, c);
		curr_x1 = p3.x() - (dx1 * (p3.y() - y + 1)) / dy;
		curr_x2 = p3.x() - (dx2 * (p3.y() - y + 1)) / dy;
	}
}

void GraphicsEngine::fillSimplex(Simplex const& s, short c) {
	Vec2D p0{s.pos[0].x(), s.pos[0].y()};
	Vec2D p1{s.pos[1].x(), s.pos[1].y()};
	Vec2D p2{s.pos[2].x(), s.pos[2].y()};

	if (p1.y() < p0.y()) std::swap(p0, p1);
	if (p2.y() < p0.y()) std::swap(p0, p2);
	if (p2.y() < p1.y()) std::swap(p1, p2);

	if (std::abs(p1.y() - p2.y()) < 0.01) {
		fillFlatBottomTriangle(p0, p1, p2, c);
	} else if (std::abs(p0.y() - p1.y()) < 0.05) {
		fillFlatTopTriangle(p0, p1, p2, c);
	} else {
		Vec2D p3{p0.x() + ((p1.y() - p0.y())/(p2.y() - p0.y())) * (p2.x() - p0.x()),
			p1.y()};
		//drawLine(p0.x(), p0.y(), p1.x(), p1.y(), 1);
		//drawLine(p1.x(), p1.y(), p3.x(), p3.y(), 1);
		//drawLine(p3.x(), p3.y(), p0.x(), p0.y(), 1);
		fillFlatBottomTriangle(p0, p1, p3, c);
		fillFlatTopTriangle(p1, p3, p2, c);
	}
}


void GraphicsEngine::rasterize(std::vector<Simplex> const& simplices) {
	/*for (unsigned int x(0); x < drawWidth; ++x) {	
		for (unsigned int y(0); y < drawHeight; ++y) {
			for (Simplex const& s: simplices) {
				if (s.contains(x, y)) {
					drawPoint(x, y, 1);
				}
			}
		}
	}*/
}

void GraphicsEngine::drawLine(Vec2D p1, Vec2D p2, short c) {
	double dx = p2.x() - p1.x();
	double dy = p2.y() - p1.y();

	if (dx != 0 && abs(dx) >= abs(dy)) {
		if (p1.x() > p2.x()) {
			std::swap(p1, p2);
		}
		for (int x(std::round(p1.x())); x <= std::round(p2.x()); ++x) {
				double y = p1.y() + dy * (x - p1.x()) / dx;
				drawPoint(x, std::round(y), c);
		}
	} else if(dy != 0) {
		if (p1.y() > p2.y()) {
			std::swap(p1, p2);
		}
		for (int y(std::round(p1.y())); y <= std::round(p2.y()); ++y) {
				double x = p1.x() + dx * (y - p1.y()) / dy;
				drawPoint(std::round(x), y, c);
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
	drawWidth = width;
	drawHeight = height;
	updatePerspective();
}

void GraphicsEngine::updatePerspective() {
	perspective = Matrix4D::make_perspective(
			nearPlane, farPlane, FOV,
			double(height)*2.0/double(width));
}
