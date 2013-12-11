#include "Asteroid.h"

Asteroid::Asteroid(float x, float y, float z, float size): Object(x - size, x + size, y - size, y + size, z - size, z + size), _x(x), _y(y), _z(z), _size(size) {
}

void Asteroid::draw() const {
	glEnable(GL_LIGHTING);
	GLfloat mat_ambient[] = { 0.1, 0.1, 0.3, 1.0 };
	GLfloat mat_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat mat_specular[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat mat_shininess[] = { 10.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glPushMatrix();
	glTranslatef(_x, _y, _z);
	gluSphere(_quadric, _size, 60, 30);
	glPopMatrix();
}

GLUquadric *Asteroid::_quadric = gluNewQuadric();

