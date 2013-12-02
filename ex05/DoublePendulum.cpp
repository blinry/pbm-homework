#include "DoublePendulum.h"

DoublePendulum::DoublePendulum(std::vector<particle_type> &particles, const Length &l1, const Length &l2):
	System(particles), l1(l1), l2(l2) {}

void DoublePendulum::computeAccelerations() {
	/* Read the angle ("position") and angular velocity ("velocity") of both particles from the "particles" vector.
	 * Then compute the angular accelerations and store them in the particles' "acceleration" variable.
	 */

	Particle<Angle>::length_type theta1 = particles[0].position;
	Particle<Angle>::length_type theta2 = particles[1].position;
	Particle<Angle>::length_type delta = theta2 - theta1;

	Particle<Angle>::velocity_type omega1 = particles[0].velocity;
	Particle<Angle>::velocity_type omega2 = particles[1].velocity;

	Mass m1 = particles[0].mass;
	Mass m2 = particles[1].mass;
	Acceleration g = 9.81 * m / s / s;

	particles[0].acceleration = (m2 * l1 * omega1 * omega1 * sin(delta) * cos(delta) + m2 * g * sin(theta2) * cos(delta) + m2 * l2 * omega2 * omega2 * sin(delta) - (m1 + m2) * g * sin(theta1)) /
		((m1 + m2) * l1 - m2 * l1 * cos(delta) * cos(delta)); //d omega1 / dt

	particles[1].acceleration = (-m2 * l2 * omega2 * omega2 * sin(delta) * cos(delta) + (m1 + m2) * (g * sin(theta1) * cos(delta) - l1 * omega1 * omega1 * sin(delta) - g * sin(theta2))) /
		((m1 + m2) * l2 - m2 * l2 * cos(delta) * cos(delta)); //d omega2 / dt
}

