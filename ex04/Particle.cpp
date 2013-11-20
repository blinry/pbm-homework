#include "Particle.h"

std::ostream &operator<<(std::ostream &o, const Particle &p) {
	// Set stream parameters while storing the former values.
	std::ios::fmtflags flags = o.flags(std::ios::left | std::ios::showpos | std::ios::scientific);
	std::streamsize precision = o.precision(1);
	std::streamsize width = o.width(8);

	// Write particle name and attributes.
	o << (p.name + std::string(":"));
	o << "m = " << p.mass;
	o << ", p = " << p.position;
	o << ", v = " << p.velocity;
	o << ", a = " << p.acceleration;

	// Reset former stream parameters.
	o.width(width);
	o.precision(precision);
	o.flags(flags);

	return o;
}

