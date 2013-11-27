#ifndef _PARTICLE_H
#define _PARTICLE_H

#include <string>

#include "Quantity.h"

using namespace quantity;

/**
 * A particle storing several physical attributes.
 */
template <typename T>
class Particle {
	public:
		typedef T length_type;
		typedef typename length_type::time_derivative_type velocity_type;
		typedef typename velocity_type::time_derivative_type acceleration_type;

		/**
		 * Construct a particle with given attributes.
		 */
		Particle(const Mass &mass, const length_type &position, const velocity_type &velocity, bool fixed=false, const std::string name=""):
			mass(mass), position(position), velocity(velocity), acceleration(), fixed(fixed), name(name) {}

		Mass mass; //< Mass of the particle.
		length_type position; //< Position of the particle.
		velocity_type velocity; //< Velocity of the particle.
		acceleration_type acceleration; //< Acceleration of the particle, as computed by a system.
		bool fixed; //< Flag is true if the particle cannot move.
		std::string name; //< Optional name of the particle for displaying purposes.
};

#endif

