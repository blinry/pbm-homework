#include "RungeKuttaSolver.h"

void RungeKuttaSolver::step(const Time stepsize)  {
	// Store old values of x and v.
	std::vector<Length3D> x0;
	std::vector<Velocity3D> v0;
	for (size_t i = 0; i < _system->particles.size(); ++i) {
		x0.push_back(_system->particles[i].position);
		v0.push_back(_system->particles[i].velocity);
	}

	// Compute a(x0).
	_system->computeAccelerations();
	std::vector<Velocity3D> dx1;
	std::vector<Acceleration3D> dv1;
	for (size_t i = 0; i < _system->particles.size(); ++i) {
		// Compute  dx1 = v0  and  dv1 = a(x0).
		dx1.push_back(_system->particles[i].velocity);
		dv1.push_back(_system->particles[i].acceleration);
		// Set position and velocity for computation of dx2 and dv2.
		_system->particles[i].position = x0[i] + 0.5 * stepsize * dx1[i];
		_system->particles[i].velocity = v0[i] + 0.5 * stepsize * dv1[i];
	}

	// Set  t = t0 + 1/2 h.
	_system->time += 0.5 * stepsize;
	
	// Compute a(x0 + 1/2 h dx1).
	_system->computeAccelerations();
	std::vector<Velocity3D> dx2;
	std::vector<Acceleration3D> dv2;
	for (size_t i = 0; i < _system->particles.size(); ++i) {
		// Compute  dx2 = v0 + 1/2 h dv1  and  dv2 = a(x0 + 1/2 h dx1).
		dx2.push_back(_system->particles[i].velocity);
		dv2.push_back(_system->particles[i].acceleration);
		// Set position and velocity for computation of dx3 and dv3.
		_system->particles[i].position = x0[i] + 0.5 * stepsize * dx2[i];
		_system->particles[i].velocity = v0[i] + 0.5 * stepsize * dv2[i];
	}
	
	// Compute a(x0 + 1/2 h dx2).
	_system->computeAccelerations();
	std::vector<Velocity3D> dx3;
	std::vector<Acceleration3D> dv3;
	for (size_t i = 0; i < _system->particles.size(); ++i) {
		// Compute  dx3 = v0 + 1/2 h dv2  and  dv3 = a(x0 + 1/2 h dx2).
		dx3.push_back(_system->particles[i].velocity);
		dv3.push_back(_system->particles[i].acceleration);
		// Set position and velocity for computation of dx4 and dv4.
		_system->particles[i].position = x0[i] + stepsize * dx3[i];
		_system->particles[i].velocity = v0[i] + stepsize * dv3[i];
	}

	// Set  t = t0 + h.
	_system->time += 0.5 * stepsize;
	
	// Compute a(x0 + h dx3).
	_system->computeAccelerations();
	std::vector<Velocity3D> dx4;
	std::vector<Acceleration3D> dv4;
	for (size_t i = 0; i < _system->particles.size(); ++i) {
		// Compute  dx4 = v0 + h dv3  and  dv4 = a(x0 + h dx3).
		dx4.push_back(_system->particles[i].velocity);
		dv4.push_back(_system->particles[i].acceleration);
		// Set  x = x0 + 1/6 h (dx1 + 2 dx2 + 2 dx3 + dx4)  and  v = v0 + 1/6 h (dv1 + 2 dv2 + 2 dv3 + dv4).
		_system->particles[i].position = x0[i] + stepsize / 6.0 * (dx1[i] + 2.0 * dx2[i] + 2.0 * dx3[i] + dx4[i]);
		_system->particles[i].velocity = v0[i] + stepsize / 6.0 * (dv1[i] + 2.0 * dv2[i] + 2.0 * dv3[i] + dv4[i]);
	}
}

