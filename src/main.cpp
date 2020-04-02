#include <curses.h>
#include <iostream>

#include "vect.h"
#include "matrix.h"

int main() {
	using namespace std;

	Matrix3D I = Matrix3D::make_identity();
	I[1][2] = 2;
	Vec3D v({1, 2, 3});
	cout << (I * v) << endl;

	/*initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	curs_set(0);


	while(true) {
		wrefresh(stdscr);
		mvwaddch(stdscr, 10, 10, 'A');
	}

	endwin();*/
}
