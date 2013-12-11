#ifndef _AABB_H
#define _AABB_H

#include <cassert>
#include <iostream>
#include <vector>

#include "EndPoint.h"

class Object;

class AABB {
	public:
		AABB() {}
		AABB(Object *parent, const std::vector<float> &minvals, const std::vector<float> &maxvals):
			_parent(parent), _minvals(minvals.size()), _maxvals(maxvals.size()) {
			assert(minvals.size() == maxvals.size());
			for (size_t i = 0; i < dims(); ++i) {
				if (minvals[i] < maxvals[i]) {
					_minvals[i] = new EndPoint(this, i, minvals[i], true);
					_maxvals[i] = new EndPoint(this, i, maxvals[i], false);
				} else {
					_minvals[i] = new EndPoint(this, i, maxvals[i], true);
					_maxvals[i] = new EndPoint(this, i, minvals[i], false);
				}
			}
		}
		~AABB() {
			for (size_t i = 0; i < dims(); ++i) {
				delete _minvals[i];
				delete _maxvals[i];
			}
		}

		size_t dims() const { return _minvals.size(); }

		const EndPoint *minval(size_t i) const { return _minvals[i]; }
		EndPoint *minval(size_t i) { return _minvals[i]; }

		const EndPoint *maxval(size_t i) const { return _maxvals[i]; }
		EndPoint *maxval(size_t i) { return _maxvals[i]; }

		void draw() const;

		const Object *parent() const { return _parent; }
		Object *parent() { return _parent; }

	private:
		Object *_parent;
		std::vector<EndPoint*> _minvals, _maxvals;
};

std::ostream &operator<<(std::ostream &o, const AABB &b);

#endif

