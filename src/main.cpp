#include <signal.h>

#include "graphics.h"
#include "mesh.h"
#include <iostream>
#include <memory>
#include "graphics.h"
#include "matrix.h"

GraphicsEngine engine;

void handleResize(int sig) {
	engine.eventResize();
}

int main() {
	signal(SIGWINCH, handleResize);

	std::cout << Matrix4D::make_rotation_x(3.14/3.0) << std::endl;
	std::cout << Matrix4D::make_rotation_y(3.14/3.0) << std::endl;
	std::cout << (Matrix4D::make_translation(0, 0,3.0) * Vec4D({1, 2, 0, 1})).to_3D() << std::endl;

	engine.init();
	engine.start();
}
