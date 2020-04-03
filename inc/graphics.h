#pragma once

#include <vector>
#include <curses.h>

#include "vect.h"
#include "matrix.h"
#include "math.h"
#include "mesh.h"

class GraphicsEngine {
	public:
		~GraphicsEngine();
		
		void init();
		void start();
		void draw();
		void drawLine(Vec2D p1, Vec2D p2, short c);
		void drawPoint(int x, int y, short c);
		void fillSimplex(Simplex const& s, short c);
		void rasterize(std::vector<Simplex> const& simplices);
		void fillFlatTopTriangle(Vec2D p1, Vec2D p2, Vec2D p3, short c);
		void fillFlatBottomTriangle(Vec2D p1, Vec2D p2, Vec2D p3, short c);


		void clearBuffer();
		void renderBuffer();

		void eventResize();
		void updatePerspective();

	private:
		int width;
		int height;

		int drawWidth;
		int drawHeight;

		std::vector<std::vector<short>> buffer;

		Matrix4D perspective;
		Matrix4D view;

		double nearPlane = 0.1;
		double farPlane = 1000;
		double FOV = M_PI/2.0;

		Mesh mesh;
};
