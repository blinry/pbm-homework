#include "CollisionDetector.h"

void CollisionDetector::sort(std::vector<const EndPoint*> &endpoints, std::set<std::pair<const AABB*, const AABB*> > &overlaps) {
	/* Implement a simple in-place "bubble sort" algorithm for the endpoints here.
	 * Whenever a start point and an end point are swapped, the corresponding
	 * parent boxes start or cease to overlap. Update the set of overlaps accordingly
	 * by inserting or erasing pairs of overlapping bounding boxes. In order for the
	 * pairs to be uniquely defined, you need to maintain a consistent ordering for
	 * the two members of the pair. Use the function make_sorted_pair to create an
	 * appropriately ordered pair of bounding boxes.
	 */
    std::sort(endpoints.begin(), endpoints.end(), EndPoint::comp_ptr);

    std::set<const AABB*> open_aabbs;
    for(int i=0; i<endpoints.size(); i++) {
        const EndPoint *e = endpoints[i];
        if (e->is_start()) {
            for(std::set<const AABB*>::iterator i = open_aabbs.begin(); i != open_aabbs.end(); i++) {
                overlaps.insert(make_sorted_pair(*i, e->parent()));
            }
            open_aabbs.insert(e->parent());
        } else {
            open_aabbs.erase(e->parent());
        }
    }
}

std::vector<std::pair<const AABB*, const AABB*> > CollisionDetector::detect_possible_collisions() {
	std::vector<std::pair<const AABB*, const AABB*> > intersection;
	/* Implement an algorithm for detecting overlapping bounding boxes here.
	 * For each dimension (x, y, and z), there is a sorted list of bounding box
	 * start and end points stored in _endpoints. A set of bounding boxes whose
	 * coordinate ranges in this dimension overlap is stored in _overlaps.
	 * First, for each dimension, update the endpoints and overlaps using the
	 * sort function implemented above. Then, the set of possible collisions
	 * is simply the intersection of the sets of overlaps in all dimensions,
	 * that is, the set of pairs of bounding boxes whose coordinate ranges
	 * overlap in every dimension.
	 */
    std::vector<std::set<std::pair<const AABB*, const AABB*> > > possible_intersections;
    for(int i=0; i<3; i++) {
        possible_intersections.push_back(std::set<std::pair<const AABB*, const AABB*> >());
        sort(_endpoints[i], possible_intersections[i]);
    }
    for(std::set<std::pair<const AABB*, const AABB*> >::iterator i = possible_intersections[0].begin(); i != possible_intersections[0].end(); i++) {
        std::pair<const AABB*, const AABB*> pair = *i;
        if (possible_intersections[1].find(pair) == possible_intersections[1].end()) {
            continue;
        }
        if (possible_intersections[2].find(pair) == possible_intersections[2].end()) {
            continue;
        }
        intersection.push_back(pair);
    }
	return intersection;
}

