#ifndef _ENDPOINT_H
#define _ENDPOINT_H

#include <iostream>

class AABB;

class EndPoint {
	public:
		EndPoint(const AABB *parent, const int axis, const float coordinate, const bool startflag):
			_parent(parent), _axis(axis), _coordinate(coordinate), _startflag(startflag) {}

		const AABB *parent() const { return _parent; }
		
		int axis() const { return _axis; }
		
		float coordinate() const { return _coordinate; }
		float &coordinate() { return _coordinate; }

		bool is_start() const { return _startflag; }
		bool is_end() const { return !_startflag; }

		operator float() const { return _coordinate; }

		static bool comp_ptr(const EndPoint *a, const EndPoint *b) { return a->coordinate() < b->coordinate(); }

	private:
		const AABB *_parent;
		int _axis;
		float _coordinate;
		bool _startflag;
};

#endif

