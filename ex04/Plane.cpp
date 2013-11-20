#include "Plane.h"

#include <cstdlib>

#if defined(__APPLE_CC__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

Plane::Plane(const Length3D &v, const ReflectionCoefficient &r, const Number &f):
	Obstacle(r, f), _n(v / norm(v)), _d(norm(v)) {}

Plane::Plane(const Number3D &n, const Length &d, const ReflectionCoefficient &r, const Number &f):
	Obstacle(r, f), _n(n), _d(d) {}

void Plane::draw(const Length &scale) const {
	Number3D rnd; // Random "up" vector for drawing obstacles.
	for (size_t i = 0; i < 3; ++i)
		rnd[i] = rand() / double(RAND_MAX);
	Number3D u = cross(_n, rnd); // u is orthogonal to _n
	u /= norm(u);
	Number3D v = cross(_n, u); // v is orthogonal to _n and u
	v /= norm(v);

	double c = 20.0;
	Number3D p1 = c * (u + v) + _d * _n / scale;
	Number3D p2 = c * (-u + v) + _d * _n / scale;
	Number3D p3 = c * (-u - v) + _d * _n / scale;
	Number3D p4 = c * (u - v) + _d * _n / scale;

	glBegin(GL_QUADS);
	glVertex3d(p1[0], p1[1], p1[2]);
	glVertex3d(p2[0], p2[1], p2[2]);
	glVertex3d(p3[0], p3[1], p3[2]);
	glVertex3d(p4[0], p4[1], p4[2]);
	glEnd();
}

Force3D Plane::computeReflectionForce(const Particle &p) const {
	/* Please fill in your code for computing the reflection force here.
	 *
	 * Check if the particle is on the correct side of the plane, i.e., in direction of
	 * the normal vector "_n". The distance of the plane to the origin is available as
	 * "_d". If the particle is on the wrong side of the plane, apply the reflection
	 * force. The reflection force tries to push the particle back to the correct side
	 * of the plane. It should be proportional to the distance of the particle to the
	 * plane (the proportionality constant is stored in "_r"), and should act in the
	 * direction of the normal of the plane.
	 */
	
	Force3D f;
	if(dot(p.position,_n) < _d)
	{
		f = ( _d - dot(p.position,_n)) * _n * _r; 
	}

	
	return f;
}

Force3D Plane::computeFrictionForce(const Particle &p) const {
	/* Please fill in your code for computing the friction force here.
	 *
	 * Check if the particle is on the correct side of the plane, i.e., in direction of
	 * the normal vector "_n". The distance of the plane to the origin is available as
	 * "_d". If the particle is on the wrong side of the plane, apply the friction force.
	 * The friction force slows down the particle with respect to the plane. It should be
	 * proportional to the normal component of the force that is currently acting on the
	 * particle (the proportionality constant is stored in "_f"), and should act in the
	 * direction of the velocity of the particle.
	 */
	Force3D f;
	if(dot(p.position,_n) < _d)
	{
		f  =  dot(p.acceleration * p.mass,_n) * -( p.velocity / norm(p.velocity)) * _f;
	}
	return f;
}

