#ifndef _TRIANGLE_H
#define _TRIANGLE_H

class Asteroid;

class Triangle {
	public:
		Triangle(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2):
			x0(x0), y0(y0), z0(z0), x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2) {}

		void draw() const;

		float x0, y0, z0, x1, y1, z1, x2, y2, z2;
};

#endif

