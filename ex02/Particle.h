#ifndef _PARTICLE_H
#define _PARTICLE_H

#include <string>

#include "Quantity.h"

using namespace quantity;

/**
 * A particle storing several physical attributes.
 */
class Particle {
	public:
		/**
		 * Construct a particle with given attributes.
		 */
		Particle(const Mass &mass, const Length3D &position, const Velocity3D &velocity, const std::string name=""):
			mass(mass), position(position), velocity(velocity), acceleration(), name(name) {}

		Mass mass; //< Mass of the particle.
		Length3D position; //< Position of the particle.
		Velocity3D velocity; //< Velocity of the particle.
		Acceleration3D acceleration; //< Acceleration of the particle, as computed by a system.
		std::string name; //< Optional name of the particle for displaying purposes.
};

/**
 * Stream output of a particle and its attributes.
 */
std::ostream &operator<<(std::ostream &o, const Particle &p);

#endif

