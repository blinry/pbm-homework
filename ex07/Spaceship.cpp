#include "Spaceship.h"
#include <cmath>
#include <cfloat>
#include <cstdlib>
#include <GL/glfw.h>
#include "Asteroid.h"
#include <limits>
#include <stdio.h>

Spaceship::Spaceship(): Object(0, 0, 0, 0, 0, 0), _speed(0.0),_x(0.0), _y(0.0), _z(0.0), _fx(0.0), _fy(0.0), _fz(1.0), _ux(0.0), _uy(1.0), _uz(0.0), _rx(1.0), _ry(0.0), _rz(0.0) {
	/***** You can also define a more complex spaceship here if you like! *****/
	_triangles.push_back(Triangle(-0.2, 0.0, 0.4,  0.2, 0.0, 0.4,  0.0, 0.1, 0.4)); // back plate
	_triangles.push_back(Triangle(-0.2, 0.0, 0.4,  0.0, 0.1, 0.4,  0.0, 0.0, -0.3)); // left plate
	_triangles.push_back(Triangle(0.0, 0.1, 0.4,  0.2, 0.0, 0.4,  0.0, 0.0, -0.3)); // right plate
	_triangles.push_back(Triangle(-0.2, 0.0, 0.4,  0.0, 0.0, -0.3,  0.2, 0.0, 0.4)); // bottom plate

	update_bb();
}

void Spaceship::draw() const {
	glEnable(GL_LIGHTING);
	GLfloat mat_ambient[] = { 0.2, 0.2, 0.5, 1.0 };
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_specular[] = { 0.3, 0.6, 0.6, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glPushMatrix();

	// transform to global coordinates
	glTranslatef(_x, _y, _z);
	float matrix[16] = {
		_rx, _ry, _rz, 0.0,
		_ux, _uy, _uz, 0.0,
		_fx, _fy, _fz, 0.0,
		0.0, 0.0, 0.0, 1.0
	};
	glMultMatrixf(matrix);

	glBegin(GL_TRIANGLES);
	srand(0);
	for (std::vector<Triangle>::const_iterator i = _triangles.begin(); i != _triangles.end(); ++i) {
		glColor3f(rand() / double(RAND_MAX), rand() / double(RAND_MAX), rand() / double(RAND_MAX));
		i->draw();
	}
	glEnd();
	glPopMatrix();
}

float Spaceship::clamp(float x, float lower, float upper) {
	if (x < lower) return lower;
	if (x > upper) return upper;
	return x;
}

bool Spaceship::collides(const Asteroid *other) const {
	/* Please fill in your code for detecting collisions between the spaceship
	 * and a spherical asteroid here. You can compute collisions with the ship
	 * by testing for collision with any one of the triangles in "_triangles".
	 * To test for intersection of a triangle with a sphere, you need to compute
	 * the smallest distance between the triangle and the center of the sphere,
	 * e.g. like this:
	 *
	 * http://www.geometrictools.com/Documentation/DistancePoint3Triangle3.pdf
	 *
	 * The vertices of the triangle are stored as (x0, y0, z0) etc., while the
	 * center of the asteroid can be read using x(), y() and z(), and its radius
	 * using the size() method. Remember to transform the triangle vertices to
	 * global coordinates using the transform
	 *
	 * ( _rx  _ux  _fx )   ( x ) + ( _x )
	 * ( _ry  _uy  _fy ) . ( y ) + ( _y )
	 * ( _rz  _uz  _fz )   ( z ) + ( _z ).
	 */

    float x, y, z;
    for(int i=0; i<_triangles.size(); i++) {
        for(int j=0; j<3; j++) {
            switch (j) {
                case 0:
                    x = _triangles[i].x0;
                    y = _triangles[i].y0;
                    z = _triangles[i].z0;
                    break;
                case 1:
                    x = _triangles[i].x1;
                    y = _triangles[i].y1;
                    z = _triangles[i].z1;
                    break;
                case 2:
                    x = _triangles[i].x2;
                    y = _triangles[i].y2;
                    z = _triangles[i].z2;
                    break;
            }

            float tx = _rx*x + _ux*y + _fx*z + _x;
            float ty = _ry*x + _uy*y + _fy*z + _y;
            float tz = _rz*x + _uz*y + _fz*z + _z;

            if (pow(pow(other->x() - tx, 2) + pow(other->y() - ty, 2) + pow(other->z() - tz, 2), 1.0/2) < other->size()) {
                return true;
            }
        }
    }
	return false;
}

void Spaceship::set_camera() const {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// add a small offset for nicer perspective
	glTranslatef(0.0, 0.0, -1.6);
	glRotatef(20.0, 1.0, 0.0, 0.0);

	float matrix[16] = {
		_rx, _ux, _fx, 0.0,
		_ry, _uy, _fy, 0.0,
		_rz, _uz, _fz, 0.0,
		0.0, 0.0, 0.0, 1.0
	};
	glMultMatrixf(matrix);
	glTranslatef(-_x, -_y, -_z);
}

void Spaceship::update(float t) {
	_x -= _speed * _fx * t;
	_y -= _speed * _fy * t;
	_z -= _speed * _fz * t;
	update_bb();
}

void Spaceship::rotate_leftright(float angle) {
	float c = cos(angle);
	float s = sin(angle);
	float new_fx = c * _fx + s * _rx;
	float new_fy = c * _fy + s * _ry;
	float new_fz = c * _fz + s * _rz;
	_rx = -s * _fx + c * _rx;
	_ry = -s * _fy + c * _ry;
	_rz = -s * _fz + c * _rz;
	_fx = new_fx;
	_fy = new_fy;
	_fz = new_fz;
	update_bb();
}

void Spaceship::rotate_updown(float angle) {
	float c = cos(angle);
	float s = sin(angle);
	float new_fx = c * _fx + s * _ux;
	float new_fy = c * _fy + s * _uy;
	float new_fz = c * _fz + s * _uz;
	_ux = -s * _fx + c * _ux;
	_uy = -s * _fy + c * _uy;
	_uz = -s * _fz + c * _uz;
	_fx = new_fx;
	_fy = new_fy;
	_fz = new_fz;
	update_bb();
}

void Spaceship::update_bb() {
	/* Update the coordinates of the bounding box according to the
	 * coordinates of the triangles in the new rotated and translated
	 * system. The transform from the triangle's local coordinate
	 * system to the global system is described by
	 *
	 * ( _rx  _ux  _fx )   ( x ) + ( _x )
	 * ( _ry  _uy  _fy ) . ( y ) + ( _y )
	 * ( _rz  _uz  _fz )   ( z ) + ( _z ),
	 *
	 * where x, y, and z are the coordinates of one of the triangle's
	 * vertices.
	 *
	 * Store the new minimal and maximal x, y, and z coordinates in
	 * the bounding box, e.g. like this:
	 *
	 * bounding_box()->minval(0)->coordinate() = new_minimal_x;
	 */
    float x, y, z, tx, ty, tz;
    float minx = std::numeric_limits<float>::max();
    float maxx = -std::numeric_limits<float>::max();
    float miny = std::numeric_limits<float>::max();
    float maxy = -std::numeric_limits<float>::max();
    float minz = std::numeric_limits<float>::max();
    float maxz = -std::numeric_limits<float>::max();

    for(int i=0; i<_triangles.size(); i++) {
        for(int j=0; j<3; j++) {
            switch (j) {
                case 0:
                    x = _triangles[i].x0;
                    y = _triangles[i].y0;
                    z = _triangles[i].z0;
                    break;
                case 1:
                    x = _triangles[i].x1;
                    y = _triangles[i].y1;
                    z = _triangles[i].z1;
                    break;
                case 2:
                    x = _triangles[i].x2;
                    y = _triangles[i].y2;
                    z = _triangles[i].z2;
                    break;
            }
            tx = _rx*x + _ux*y + _fx*z + _x;
            ty = _ry*x + _uy*y + _fy*z + _y;
            tz = _rz*x + _uz*y + _fz*z + _z;

            minx = fminf(minx, tx);
            miny = fminf(miny, ty);
            minz = fminf(minz, tz);
            maxx = fmaxf(maxx, tx);
            maxy = fmaxf(maxy, ty);
            maxz = fmaxf(maxz, tz);
        }
    }
    bounding_box()->minval(0)->coordinate() = minx;
    bounding_box()->maxval(0)->coordinate() = maxx;
    bounding_box()->minval(1)->coordinate() = miny;
    bounding_box()->maxval(1)->coordinate() = maxy;
    bounding_box()->minval(2)->coordinate() = minz;
    bounding_box()->maxval(2)->coordinate() = maxz;
}

