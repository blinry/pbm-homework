#include "GravitationalSystem.h"
#include "EulerSolver.h"
#include "AdaptiveEulerSolver.h"
#include "RungeKuttaSolver.h"

#include <cmath>
#include <fstream>

#if defined(__APPLE_CC__)
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

GLint windowWidth = 800, windowHeight = 600;
int old_button = -1, old_state = -1, old_x = -1, old_y = -1;
float modelview_matrix[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

const size_t debug = 100; // Every debug time steps, status output is produced on stdout.

std::vector<Particle> particles; // The list of particles that will be inserted into the system.
System *particle_system = NULL;
Solver *solver = NULL;
float steps_per_year; // Number of time steps per simulation year.

GLubyte colors[] = {
	255, 242, 103,
	179, 171, 168,
	203, 203, 190,
	120, 120, 150,
	228, 193, 137,
	156, 123, 92,
	214, 184, 148,
	208, 246, 247,
	87, 134, 226,
};

void display() {
	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-windowWidth, windowWidth, -windowHeight, windowHeight, -windowWidth-windowHeight, windowWidth+windowHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(modelview_matrix);
	glScalef(0.5, 0.5, 0.5);
	glTranslatef(-0.5, -0.5, -0.5);
	glClear(GL_COLOR_BUFFER_BIT);

	size_t idx = 0;
	for (std::vector<Particle>::const_iterator p = particles.begin(); p != particles.end(); ++p, ++idx) {
		glPushMatrix();
		glTranslatef(p->position[0] / (2e9 * m), p->position[1] / (2e9 * m), p->position[2] / (2e9 * m));
		glColor3ub(colors[(3 * idx) % sizeof(colors)], colors[(3 * idx + 1) % sizeof(colors)], colors[(3 * idx + 2) % sizeof(colors)]);
		glutSolidSphere(20 * pow(p->mass / (1.989e30 * kg), 1.0 / 7.0), 90, 45);
		glPopMatrix();
	}
	
	glutSwapBuffers();
}

void idle() {
	solver->step(356 * days / steps_per_year);
	glutPostRedisplay();
}

void reshape(int width, int height) {
	windowWidth = width;
	windowHeight = height;
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	old_button = button;
	old_state = state;
	old_x = x;
	old_y = y;
	glutPostRedisplay();
}

void motion(int x, int y) {
	float dx = x - old_x;
	float dy = y - old_y;

	switch (old_button) {
		case GLUT_LEFT_BUTTON:
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glRotatef(sqrt(dx * dx + dy * dy), dy, dx, 0.0);
			glMultMatrixf(modelview_matrix);
			glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
			glutPostRedisplay();
			break;
		case GLUT_RIGHT_BUTTON:
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glScalef(exp(-0.01 * dy), exp(-0.01 * dy), exp(-0.01 * dy));
			glMultMatrixf(modelview_matrix);
			glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
			glutPostRedisplay();
			break;
	}
	
	old_x = x;
	old_y = y;
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'r':
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
			glutPostRedisplay();
			break;
		case 27:
			exit(0);
			break;
	}
}

/**
 * Create or load a particle system and simulate a number of time steps.
 */
int main(int argc, char *argv[]) {
	int years = -1; // Number of simulation years to simulate.
	if (argc == 1) { // interactive with default step size
		steps_per_year = 5000;
	} else if (argc == 2) { // interactive
		steps_per_year = atof(argv[1]);
	} else if (argc == 3) { // file output
		years = atoi(argv[1]);
		steps_per_year = atof(argv[2]);
	} else {
		std::cerr << "Usage:" << std::endl;
		std::cerr << "interactive: " << argv[0] << " steps_per_year" << std::endl;
		std::cerr << "file output: " << argv[0] << " number_of_years steps_per_year" << std::endl;
		exit(1);
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
		particles.push_back(Particle(mass * kg, position * ex * km, velocity * ey * km / s, name)); // Add a new particle.
	}

	// Create a particle system and a solver.
	particle_system = new GravitationalSystem(particles);
	//solver = new EulerSolver(particle_system);
	//solver = new AdaptiveEulerSolver(particle_system, 1e3 * m);
	solver = new RungeKuttaSolver(particle_system);

	if (years < 0) { // interactive
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		glutInitWindowPosition(0, 0);
		glutInitWindowSize(windowWidth, windowHeight);
		glutCreateWindow("Particle System");
		glutIdleFunc(idle);
		glutDisplayFunc(display);
		glutReshapeFunc(reshape);
		glutMouseFunc(mouse);
		glutMotionFunc(motion);
		glutKeyboardFunc(keyboard);
		glutMainLoop();
	} else { // file output
		// Open a data file for dumping the simulation results.
		std::ofstream outfile("ex01.dat");
		outfile << 0;
		for (std::vector<Particle>::const_iterator p = particles.begin(); p != particles.end(); ++p)
			outfile << ", " << p->position[0].value() << ", " << p->position[1].value();
		outfile << std::endl;

		// Print the initial configuration.
		for (std::vector<Particle>::const_iterator p = particles.begin(); p != particles.end(); ++p)
			std::cout << *p << std::endl;
		std::cout << std::endl;

		// Perform the number of iterations that was specified on the command line.
		for (int step = 1; step <= years * steps_per_year; ++step) {
			// Perform one step.
			solver->step(356 * days / steps_per_year);

			// Print the current status every debug time steps.
			if (debug && step % debug == 0) {
				std::cout << "Status after step " << step << ":" << std::endl;
				for (std::vector<Particle>::const_iterator p = particles.begin(); p != particles.end(); ++p)
					std::cout << *p << std::endl;
				std::cout << std::endl;
			}

			// Dump the current status into the data file.
			outfile << step;
			for (std::vector<Particle>::const_iterator p = particles.begin(); p != particles.end(); ++p)
				outfile << ", " << p->position[0].value() << ", " << p->position[1].value();
			outfile << std::endl;
		}

		outfile.close();
	}

	return 0;
}

