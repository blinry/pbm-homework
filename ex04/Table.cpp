#include "Table.h"

#include <cstdlib>
#include <math.h>

#if defined(__APPLE_CC__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

Table::Table(const Length3D &v, const Number3D &n, const Length &r, const ReflectionCoefficient &refl, const Number &fric):
	Obstacle(refl, fric), _origin(v), _normal(n / norm(n)), _radius(r) {}

void Table::draw(const Length &scale) const
{
	Number3D b = normalized(cross(normalized(_origin), _normal));
	Number3D c = normalized(cross(b, _normal));

	Number3D o = _origin/scale;

	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(o[0], o[1], o[2]);
	for(double i=0; i<2*3.1415; i += 2*3.1415/32) {
		Number3D p = (_origin + _radius * (sin(i)*b + cos(i)*c))/scale;
		glVertex3d(p[0], p[1], p[2]);
	}
	glEnd();
}

bool Table::hasCollided(const Length3D& newPos) const
{
	return (dot(newPos - _origin, _normal) < Length() && norm((_origin - newPos) - (dot(_origin - newPos, _normal) * _normal)) < _radius);
}

Force3D Table::computeReflectionForce(const Particle &p) const {
	
	Force3D f;
	if(this->hasCollided(p.position))
	{
		f = (abs(dot(p.position - _origin, _normal))) * _normal * _r; 
	}

	
	return f;
}

Force3D Table::computeFrictionForce(const Particle &p) const {
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
	if(hasCollided(p.position))
	{
		//f  =  dot(p.acceleration * p.mass,_n) * ( p.velocity / norm(p.velocity)) * _f;
	}
	return f;
}