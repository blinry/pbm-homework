#include "GravitationalSystem.h"

GravitationalSystem::GravitationalSystem(std::vector<Particle> &particles): System(particles) {}

void GravitationalSystem::computeAccelerations() {
	for (std::vector<Particle>::iterator p0 = particles.begin(); p0 != particles.end(); ++p0) {
		p0->acceleration[0] = p0->acceleration[1] = p0->acceleration[2] = 0 * m / s / s;
		for (std::vector<Particle>::iterator p1 = particles.begin(); p1 != particles.end(); ++p1) {
			if (p0 == p1) continue;
			// F = G * M * m / |r|^2 * r / |r|  =>  a = F / m = G * M * r / |r|^2
			p0->acceleration += G * p1->mass * (p1->position - p0->position) / pow<3>(norm(p1->position - p0->position));
		}
	}
}

