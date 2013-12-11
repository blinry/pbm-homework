#ifndef _SPACESHIP_H
#define _SPACESHIP_H

#include "Object.h"
#include "Triangle.h"

class Asteroid;

class Spaceship: public Object {
	public:
		Spaceship();
		void draw() const;

		bool collides(const Asteroid *other) const;

		void set_camera() const;
		void update(float t);
		void rotate_leftright(float angle);
		void rotate_updown(float angle);

		void speed(float s) { _speed = s; }
		float speed() const { return _speed; }

	private:
		static float clamp(float x, float lower, float upper); //< clamp a value between lower and upper bounds
		void update_bb(); //< update the bounding box

		float _speed; //< speed in forward direction
		float _x, _y, _z; //< position
		float _fx, _fy, _fz; //< forward vector
		float _ux, _uy, _uz; //< up vector
		float _rx, _ry, _rz; //< right vector

		std::vector<Triangle> _triangles;
};

#endif

