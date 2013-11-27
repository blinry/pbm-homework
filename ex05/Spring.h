#ifndef _SRPING_H
#define _SPRING_H

#include "Quantity.h"
#include "Particle.h"

using namespace quantity;

typedef Quantity<double, 1, 1, 0, -2> Stiffness;
typedef Quantity<double, 1, 1, 0, -1> SpringDamping;

class Spring {
	public:
		typedef Particle<Length3D> particle_type;

		/**
		 * Construct a spring with given attributes connecting two particles.
		 */
		Spring(particle_type &p1, particle_type &p2, Stiffness stiffness, SpringDamping damping, Length length):
			p1(&p1), p2(&p2), stiffness(stiffness), damping(damping), length(length) {}

		/**
		 * Construct a spring with given attributes connecting two particles.
		 *
		 * The length is specified in multiples of the current distance of the particles.
		 */
		Spring(particle_type &p1, particle_type &p2, Stiffness stiffness, SpringDamping damping, double length=1.0):
			p1(&p1), p2(&p2), stiffness(stiffness), damping(damping), length(length * norm(p1.position - p2.position)) {}

		/**
		 * Return the force exerted on the first particle. The force on the second particle is the same, but with opposite sign.
		 */
		Force3D getForce() const;

		particle_type *p1;
		particle_type *p2;
		Stiffness stiffness;
		SpringDamping damping;
		Length length;
};

#endif

