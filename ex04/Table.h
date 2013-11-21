#ifndef TABLE_H
#define TABLE_H

#include "Obstacle.h"

class Table : public Obstacle
{
private:
	Length3D _origin;
	Number3D _normal;
	Length _radius;

	/**
	* collision check for
	* Position newPos
	*/
	int hasCollided(Particle& p) const;
public:
	/*
	 * create a circular table from
	 * v center of circle
	 * r radius of circle
	 * n normal of circle
	*/
	Table(const Length3D &v, const Number3D &n, const Length &r, const ReflectionCoefficient &refl=ReflectionCoefficient(), const Number &fric=Number());

	//Destroy the plane
	~Table(){}

	const Length3D& getOrigin(){return _origin;}
	const Number3D& getNormal(){return _normal;}
	const Length& getRadius(){return _radius;}

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
};

#endif