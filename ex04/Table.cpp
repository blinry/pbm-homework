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

int Table::hasCollided(Particle& p) const
{
	static Length epsilon = 0.01 * m;
	if(dot(p.position - p.oldpos, _normal) * dot(p.position - p.oldpos, _normal) > 0 * m * m)
	{
		Number s = (dot(_origin, _normal) - dot(p.oldpos, _normal))/(dot(p.position - p.oldpos, _normal));
		if(Number(-1) <= s && s <= Number(1))
		{
			Length3D hit = p.oldpos + s * (p.position - p.oldpos);
			
			if(norm(hit - _origin) <= _radius)
			{
				p.oldpos = p.position;
				p.position = hit + (s >= Number(0) ? 1:-1) * epsilon * _normal;

				return s >= Number(0) ? 1:-1;
			}
		}
	}
	return false;
}

Force3D Table::computeReflectionForce(Particle &p) const 
{	
	Force3D f;
	int side;
	if((side = this->hasCollided(p)) != 0)
	{
		f = (abs(dot(p.oldpos - _origin, _normal))) * side * _normal * _r; 
		p.fixed = true;
	}

	
	return f;
}

Force3D Table::computeFrictionForce(Particle &p) const 
{
	Force3D f;
	if(hasCollided(p) != 0)
	{
		f  =  dot(p.acceleration * p.mass,_normal) * ( p.velocity / norm(p.velocity)) * _f;
	}
	return f;
}