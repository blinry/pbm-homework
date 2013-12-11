#include "Triangle.h"
#include "Asteroid.h"
#include <GL/glfw.h>

void Triangle::draw() const {
	glNormal3f((y1 - y0) * (z2 - z0) - (z1 - z0) * (y2 - y0),
	           (z1 - z0) * (x2 - x0) - (x1 - x0) * (z2 - z0),
			   (x1 - x0) * (y2 - y0) - (y1 - y0) * (x2 - x0));
	glVertex3f(x0, y0, z0);
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y2, z2);
}

