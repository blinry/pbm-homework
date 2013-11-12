#include "GravitationalSystem.h"
#include "EulerSolver.h"
#include "AdaptiveEulerSolver.h"
#include "RungeKuttaSolver.h"

#include <cmath>
#include <fstream>
#include <ctime>

std::vector<Particle> particles_gt; 
std::vector<Particle> particles_ee; 
std::vector<Particle> particles_ae; 
std::vector<Particle> particles_rk; 

System *particle_system_gt = NULL;
System *particle_system_ee = NULL;
System *particle_system_ae = NULL;
System *particle_system_rk = NULL;

Solver *solver_gt = NULL;
Solver *solver_ee = NULL;
Solver *solver_ae = NULL;
Solver *solver_rk = NULL;

int main(int argc, char* argv[])
{
	clock_t t_ee; //for timing evaluation
	clock_t t_ae; //for timing evaluation
	clock_t t_rk; //for timing evaluation

	Time stepsize;
	float we;
	if(argc < 2)
	{
		stepsize = 7 * days;
	}
	else
	{
		we = atof(argv[1]);
		stepsize = we * days; 
	}

	// Read the sun and planets of the solar system from a data file.
	Number3D ex; ex[0] = 1.0; // Unit vector in x direction.
	Number3D ey; ey[1] = 1.0; // Unit vector in y direction.
	std::ifstream infile("solarsystem.dat");
	while (infile.good()) {
		std::string name;
		double mass, position, velocity;
		infile >> name >> mass >> position >> velocity; // Read data from file.
		if (!infile.good()) // If the end of the file is reached, ignore this (incomplete) entry.
			break;
		particles_gt.push_back(Particle(mass * kg, position * ex * km, velocity * ey * km / s, name)); // Add a new particle.
		particles_ee.push_back(Particle(mass * kg, position * ex * km, velocity * ey * km / s, name)); // Add a new particle.
		particles_ae.push_back(Particle(mass * kg, position * ex * km, velocity * ey * km / s, name)); // Add a new particle.
		particles_rk.push_back(Particle(mass * kg, position * ex * km, velocity * ey * km / s, name)); // Add a new particle.
	}

	// Create a particle system and a solver.
	particle_system_gt = new GravitationalSystem(particles_gt);
	particle_system_ee = new GravitationalSystem(particles_ee);
	particle_system_ae = new GravitationalSystem(particles_ae);
	particle_system_rk = new GravitationalSystem(particles_rk);

	solver_gt = new EulerSolver(particle_system_gt);
	solver_ee = new EulerSolver(particle_system_ee);
	solver_ae = new AdaptiveEulerSolver(particle_system_ae, 1e3 * m);
	solver_rk = new RungeKuttaSolver(particle_system_rk);
	
	//create ground truth positions
	const int groundTruthDivision = 10000;
	for(unsigned int i = 0; i < groundTruthDivision; i++)
	{
		solver_gt->step(stepsize / (float) groundTruthDivision);
	}

	//create other solvers' positions and take compution time
	t_ee = clock();
	solver_ee->step(stepsize);
	t_ee = clock() - t_ee;

	t_ae = clock();
	solver_ae->step(stepsize);
	t_ae = clock() - t_ae;

	t_rk = clock();
	solver_rk->step(stepsize);
	t_rk = clock() - t_rk;

	//cumulate error over all planets:
	Length e_ee;
	Length e_ae;
	Length e_rk;

	for(unsigned int i = 0; i < particles_gt.size(); i++)
	{
		e_ee += norm(particles_gt[i].position - particles_ee[i].position);
		e_ae += norm(particles_gt[i].position - particles_ae[i].position);
		e_rk += norm(particles_gt[i].position - particles_rk[i].position);
	}

	//print errors and times on std::cout
	std::cout << "Cumulated Absolute Error of Explicit Euler: " << e_ee  << " Compution Time: " << ((float)t_ee)/CLOCKS_PER_SEC << std::endl;
	std::cout << "Cumulated Absolute Error of Adaptive Euler: " << e_ae  << " Compution Time: " << ((float)t_ae)/CLOCKS_PER_SEC << std::endl;
	std::cout << "Cumulated Absolute Error of Runge Kutta (4th order): " << e_rk  << " Compution Time: " << ((float)t_rk)/CLOCKS_PER_SEC << std::endl;


	delete solver_gt;
	delete solver_ee;
	delete solver_ae;
	delete solver_rk;

	delete particle_system_gt;
	delete particle_system_ae;
	delete particle_system_ee;
	delete particle_system_rk;
}
