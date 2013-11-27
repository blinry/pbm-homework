#include "DoublePendulum.h"

DoublePendulum::DoublePendulum(std::vector<particle_type> &particles, const Length &l1, const Length &l2):
	System(particles), l1(l1), l2(l2) {}

void DoublePendulum::computeAccelerations() {
	/* Read the angle ("position") and angular velocity ("velocity") of both particles from the "particles" vector.
	 * Then compute the angular accelerations and store them in the particles' "acceleration" variable.
	 */
}

