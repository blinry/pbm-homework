#include "AABB.h"

#include <GL/glfw.h>

std::ostream &operator<<(std::ostream &o, const AABB &b) {
	o << "[" << b.minval(0);
	for (size_t i = 1; i < b.dims(); ++i)
		o << ", " << b.minval(i);
	o << " - " << b.maxval(0);
	for (size_t i = 1; i < b.dims(); ++i)
		o << ", " << b.maxval(i);
	o << "]";
	return o;
}

void AABB::draw() const {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);

	glVertex3f(minval(0)->coordinate(), minval(1)->coordinate(), minval(2)->coordinate());
	glVertex3f(minval(0)->coordinate(), maxval(1)->coordinate(), minval(2)->coordinate());
	glVertex3f(maxval(0)->coordinate(), maxval(1)->coordinate(), minval(2)->coordinate());
	glVertex3f(maxval(0)->coordinate(), minval(1)->coordinate(), minval(2)->coordinate());

	glVertex3f(minval(0)->coordinate(), minval(1)->coordinate(), minval(2)->coordinate());
	glVertex3f(minval(0)->coordinate(), minval(1)->coordinate(), maxval(2)->coordinate());
	glVertex3f(maxval(0)->coordinate(), minval(1)->coordinate(), maxval(2)->coordinate());
	glVertex3f(maxval(0)->coordinate(), minval(1)->coordinate(), minval(2)->coordinate());

	glVertex3f(minval(0)->coordinate(), minval(1)->coordinate(), minval(2)->coordinate());
	glVertex3f(minval(0)->coordinate(), minval(1)->coordinate(), maxval(2)->coordinate());
	glVertex3f(minval(0)->coordinate(), maxval(1)->coordinate(), maxval(2)->coordinate());
	glVertex3f(minval(0)->coordinate(), maxval(1)->coordinate(), minval(2)->coordinate());

	glVertex3f(minval(0)->coordinate(), minval(1)->coordinate(), maxval(2)->coordinate());
	glVertex3f(minval(0)->coordinate(), maxval(1)->coordinate(), maxval(2)->coordinate());
	glVertex3f(maxval(0)->coordinate(), maxval(1)->coordinate(), maxval(2)->coordinate());
	glVertex3f(maxval(0)->coordinate(), minval(1)->coordinate(), maxval(2)->coordinate());

	glVertex3f(minval(0)->coordinate(), maxval(1)->coordinate(), minval(2)->coordinate());
	glVertex3f(minval(0)->coordinate(), maxval(1)->coordinate(), maxval(2)->coordinate());
	glVertex3f(maxval(0)->coordinate(), maxval(1)->coordinate(), maxval(2)->coordinate());
	glVertex3f(maxval(0)->coordinate(), maxval(1)->coordinate(), minval(2)->coordinate());

	glVertex3f(maxval(0)->coordinate(), minval(1)->coordinate(), minval(2)->coordinate());
	glVertex3f(maxval(0)->coordinate(), minval(1)->coordinate(), maxval(2)->coordinate());
	glVertex3f(maxval(0)->coordinate(), maxval(1)->coordinate(), maxval(2)->coordinate());
	glVertex3f(maxval(0)->coordinate(), maxval(1)->coordinate(), minval(2)->coordinate());

	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

