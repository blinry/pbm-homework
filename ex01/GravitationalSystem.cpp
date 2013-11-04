#include "GravitationalSystem.h"

GravitationalSystem::GravitationalSystem(std::vector<Particle> &particles): System(particles) {}

void GravitationalSystem::computeAccelerations() {
	/* Please fill in your code for computing the gravitational acceleration here.
	 * The list of particles is available in the variable "particles". Each
	 * particle has a member "acceleration" to which you will write the
	 * acceleration which is exerted on that particle by the gravitational
	 * influence of all the other particles. The acceleration a for a particle is
	 * given as
	 *
	 *   a = F / m,
	 *
	 * where F is the sum of all the gravitational forces from other particles,
	 * as defined on the exercise sheet, and m is the mass of the particle that is
	 * accelerated.
	 *
	 * You may use the functions "dot", "squared_norm", "norm" and "pow" from
	 * Quantity.h to compute dot products, norms and powers of physical quantities.
	 */
}

