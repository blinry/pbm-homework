#ifndef _ASTEROID_H
#define _ASTEROID_H

#include "Object.h"
#include <GL/glfw.h>

class Asteroid: public Object {
	public:
		Asteroid(float x, float y, float z, float size);
		void draw() const;

		float x() const { return _x; }
		float y() const { return _y; }
		float z() const { return _z; }
		float size() const { return _size; }

	private:
		float _x, _y, _z; //< position of asteroid
		float _size; //< radius of asteroid
		static GLUquadric *_quadric;
};

#endif

