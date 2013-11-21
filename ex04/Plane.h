#ifndef _PLANE_H
#define _PLANE_H

#include "Obstacle.h"

class Plane: public Obstacle {
	public:
		/**
		 * Create a plane with given vectorial distance from the origin.
		 */
		Plane(const Length3D &v, const ReflectionCoefficient &r=ReflectionCoefficient(), const Number &f=Number());

		/**
		 * Create a plane with given normal vector and scalar distance from the origin.
		 */
		Plane(const Number3D &n, const Length &d, const ReflectionCoefficient &r=ReflectionCoefficient(), const Number &f=Number());

		/**
		 * Destroy the plane.
		 */
		~Plane() {}

		/**
		 * Get a read-only reference to the normal vector.
		 */
		const Number3D &normal() const { return _n; }

		/**
		 * Get a read-only reference to the scalar distance from the origin.
		 */
		const Length &offset() const { return _d; }

		/**
		 * Compute the reflection force on a particle.
		 */
		Force3D computeReflectionForce(Particle &p) const;

		/**
		 * Compute the friction force on a particle.
		 */
		Force3D computeFrictionForce(Particle &p) const;

		/**
		 * Draw the obstacle on the screen.
		 */
		void draw(const Length &scale) const;
	private:
		Number3D _n; //< The normal vector.
		Length _d; //< The scalar distance from the origin.
};

#endif

