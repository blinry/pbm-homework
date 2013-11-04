#include "../Quantity.h"

using namespace quantity;

Length3D fall(Length3D p0, Velocity3D v0, Acceleration3D g, Time t)
{
	return p0 + v0 * t + 0.5 * g * t * t;
}

int main()
{
	Length3D p0 = {0.0 * m, 50.0 * m, 0.0 * m};
	Velocity3D v0 = {10.0 * m/s, 0.0 * m/s, 0 * m/s};
	Acceleration3D g = {0.0 * m/(s*s), -9.81 * m/(s*s), 0.0 * m/(s*s)};

	std::cout << "p0 = " << p0 << std::endl;
	std::cout << "v0 = " << v0 << std::endl;
	std::cout << "g = " << g << std::endl << std::endl;

	std::cout << "Position after 3.0s: "<< fall(p0, v0, g, 3.0 * s) << std::endl;
}
