#ifndef _OBJECT_H
#define _OBJECT_H

#include "AABB.h"

class Object {
	public:
		Object(float minx, float maxx, float miny, float maxy, float minz, float maxz);
		virtual ~Object() { delete _bounding_box; }

		const AABB *bounding_box() const { return _bounding_box; }
		AABB *bounding_box() { return _bounding_box; }

		virtual void draw() const {}
	private:
		AABB *_bounding_box;
};

#endif

