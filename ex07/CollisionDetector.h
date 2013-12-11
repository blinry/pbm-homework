#ifndef _COLLISION_DETECTOR_H
#define _COLLISION_DETECTOR_H

#include <algorithm>
#include <vector>
#include <set>
#include <utility>

#include "AABB.h"

template <typename T> inline std::pair<T*, T*> make_sorted_pair(T *a, T *b) {
	return std::make_pair(std::min(a, b), std::max(a, b));
}

class CollisionDetector {
	public:
		template <typename InputIterator> CollisionDetector(InputIterator begin, InputIterator end): _bounding_boxes(end - begin) {
			for (InputIterator i = begin; i != end; ++i)
				_bounding_boxes[i - begin] = (*i)->bounding_box();

			// for each dimension (xyz), create a sorted list of start and end points
			for (size_t dim = 0; dim < _bounding_boxes.front()->dims(); ++dim) {
				_endpoints.push_back(std::vector<const EndPoint*>());
				for (size_t j = 0; j < _bounding_boxes.size(); ++j) {
					_endpoints.back().push_back(_bounding_boxes[j]->minval(dim));
					_endpoints.back().push_back(_bounding_boxes[j]->maxval(dim));
				}
				std::sort(_endpoints.back().begin(), _endpoints.back().end(), EndPoint::comp_ptr);
			}

			// for each dimension (xyz), create a set of boxes whose coordinate ranges in this dimension overlap
			for (size_t dim = 0; dim < _bounding_boxes.front()->dims(); ++dim) {
				_overlaps.push_back(std::set<std::pair<const AABB*, const AABB*> >());
				for (size_t i = 0; i < _bounding_boxes.size(); ++i) {
					for (size_t j = 0; j < _bounding_boxes.size(); ++j) {
						if (i == j) continue;
						if ((_bounding_boxes[i]->minval(dim)->coordinate() >= _bounding_boxes[j]->minval(dim)->coordinate()
									&& _bounding_boxes[i]->minval(dim)->coordinate() <= _bounding_boxes[j]->maxval(dim)->coordinate())
								|| (_bounding_boxes[j]->minval(dim)->coordinate() >= _bounding_boxes[i]->minval(dim)->coordinate()
									&& _bounding_boxes[j]->minval(dim)->coordinate() <= _bounding_boxes[i]->maxval(dim)->coordinate()))
							_overlaps[dim].insert(make_sorted_pair(_bounding_boxes[i], _bounding_boxes[j]));
					}
				}
			}
		}

		std::vector<std::pair<const AABB*, const AABB*> > detect_possible_collisions();

		void dump() const;

	private:
		void sort(std::vector<const EndPoint*> &endpoints, std::set<std::pair<const AABB*, const AABB*> > &overlaps);

		std::vector<const AABB*> _bounding_boxes; //< the list of bounding boxes
		std::vector<std::vector<const EndPoint*> > _endpoints; //< a sorted list of start and end points of the bounding boxes for each dimension (xyz)
		std::vector<std::set<std::pair<const AABB*, const AABB*> > > _overlaps; //< a set of sorted pairs of overlapping bounding boxes for each dimension
};

#endif

