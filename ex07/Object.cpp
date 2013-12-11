#include "Object.h"

Object::Object(float minx, float maxx, float miny, float maxy, float minz, float maxz) {
	std::vector<float> minvals(3), maxvals(3);
	minvals[0] = minx;
	minvals[1] = miny;
	minvals[2] = minz;
	maxvals[0] = maxx;
	maxvals[1] = maxy;
	maxvals[2] = maxz;
	_bounding_box = new AABB(this, minvals, maxvals);
}

