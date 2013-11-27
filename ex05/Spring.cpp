#include "Spring.h"

Force3D Spring::getForce() const {
	// Precompute the vector between the two particles, its length and direction.
	Length3D r = p2->position - p1->position;
	Length l = norm(r);
	Number3D direction = r / l;

	// Compute the forces contributed by stiffness and damping.
	Force stiffness_term = stiffness * (l - length);
	Force damping_term = damping * dot(p2->velocity - p1->velocity, direction);

	// Return the total force with the correct direction.
	return (stiffness_term + damping_term) * direction;
}

