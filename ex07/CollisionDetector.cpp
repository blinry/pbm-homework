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
	return intersection;
}

