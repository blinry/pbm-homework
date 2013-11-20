#ifndef _OBSTACLE_H
#define _OBSTACLE_H

#include "Particle.h"

typedef Quantity<double, 1, 1, 0, -2> ReflectionCoefficient;
typedef Quantity<double, 1, 0, 0, 0> FrictionCoefficient;

class Obstacle {
	public:
		/**
		 * Create an obstacle with the given reflection and friction coefficients.
		 */
		Obstacle(const ReflectionCoefficient &r=ReflectionCoefficient(), const FrictionCoefficient &f=FrictionCoefficient()): _r(r), _f(f) {}

		/**
		 * Destroy the obstacle.
		 */
		virtual ~Obstacle() {};

		/**
		 * Compute the reflection force on a particle.
		 */
		virtual Force3D computeReflectionForce(const Particle &p) const = 0;

		/**
		 * Compute the friction force on a particle.
		 */
		virtual Force3D computeFrictionForce(const Particle &p) const = 0;

		/**
		 * Draw the obstacle on the screen.
		 */
		virtual void draw(const Length &scale) const = 0;
	protected:
		ReflectionCoefficient _r; //< The reflection coefficient.
		FrictionCoefficient _f; //< The friction coefficient.
};

#endif

