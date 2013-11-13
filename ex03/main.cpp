#include "MassSpringSystem.h"
#include "EulerSolver.h"
#include "AdaptiveEulerSolver.h"
#include "RungeKuttaSolver.h"

#include <cmath>
#include <cstdlib>
#include <fstream>

#if defined(__APPLE_CC__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

GLint windowWidth = 800, windowHeight = 600;
int old_button = -1, old_state = -1, old_x = -1, old_y = -1;
float modelview_matrix[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

const Time stepsize = 0.01 * s; // Simulation time step in simulated time.
const size_t num_rows = 15; // Number of particle rows in the cloth.
const size_t num_cols = 15; // Number of particle columns in the cloth.
const Stiffness stiffness = 2000.0 * g / s / s; // Stiffness of springs in the cloth.
const SpringDamping springDamping = 1.0 * g / s; // Damping in springs.
const ParticleDamping particleDamping = 1.0 * g / s; // Damping in particles.
const Mass mass = 150.0 * g / (num_rows * num_cols); // Mass of each particle.
const Length scale = 2.0 * m; // Length scale for display.
const Force windForce = 10.0 * g * m / s / s; // The acceleration due to "wind".
const Time windPeriod = 0.3 * s; // The period of the oscillating wind direction.
float shrinkage = 0.98; // Shrinkage of springs with respect to their initial length.

std::vector<Particle> particles; // List of particles.
std::vector<Spring> springs; // List of springs.
System *particle_system = NULL; // The particle system.
Solver *solver = NULL; // The solver.
GLUquadricObj *quadObj = NULL; // A quadric object for drawing the flag pole.

void display() {
	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float aspect = windowWidth / float(windowHeight);
	if (aspect >= 1.0) {
		glOrtho(-aspect, aspect, -1.0, 1.0, -10.0, 10.0);
	} else {
		glOrtho(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect, -10.0, 10.0);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(modelview_matrix);
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw the flag pole.
	glColor3f(0.5, 0.5, 0.5);
	glPushMatrix();
	glTranslatef(-0.5 * m / scale, +0.5 * m / scale, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluCylinder(quadObj, 0.04 * m / scale, 0.04 * m / scale, 2.0 * m / scale, 90, 1);
	glPopMatrix();

	// Draw the springs. Colors indicate stress.
	glBegin(GL_LINES);
	for (std::vector<Spring>::const_iterator s = springs.begin(); s != springs.end(); ++s) {
		float c = fabs(norm(s->p1->position - s->p2->position) / s->length - 1.0);
		glColor3f(c, 1.0 - c, 0.0);
		glVertex3d(s->p1->position[0] / scale, s->p1->position[1] / scale, s->p1->position[2] / scale);
		glVertex3d(s->p2->position[0] / scale, s->p2->position[1] / scale, s->p2->position[2] / scale);
	}
	glEnd();

	// Draw the particles.
	glColor3f(0.0, 0.0, 1.0);
	for (std::vector<Particle>::const_iterator p = particles.begin(); p != particles.end(); ++p) {
		glPushMatrix();
		glTranslatef(p->position[0] / scale, p->position[1] / scale, p->position[2] / scale);
		glutSolidSphere(0.002 * m / scale, 90, 45);
		glPopMatrix();
	}
	
	glutSwapBuffers();
}

void idle() {
	solver->step(stepsize);
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
	// Create a particle system.
	particles.reserve(num_rows * num_cols);
	for (size_t row = 0; row < num_rows; ++row) {
		for (size_t col = 0; col < num_cols; ++col) {
			Length3D p;
			p[0] = (col / float(num_cols - 1) - 0.5) * m;
			p[1] = (row / float(num_rows - 1) - 0.5) * m;
			p[2] = col == 0 ? 0.0 * m : (rand() / double(RAND_MAX) - 0.5) * 0.01 * m;
			particles.push_back(Particle(mass, p, Velocity3D(), col == 0));

			if (row > 0 && col > 0)
				springs.push_back(Spring(particles[(row - 1) * num_cols + (col - 1)], particles.back(), stiffness, springDamping, shrinkage));
			if (row > 0 && col < num_cols - 1)
				springs.push_back(Spring(particles[(row - 1) * num_cols + (col + 1)], particles.back(), stiffness, springDamping, shrinkage));
			if (row > 0)
				springs.push_back(Spring(particles[(row - 1) * num_cols + col], particles.back(), stiffness, springDamping, shrinkage));
			if (col > 0)
				springs.push_back(Spring(particles[row * num_cols + (col - 1)], particles.back(), stiffness, springDamping, shrinkage));
		}
	}
	particle_system = new MassSpringSystem(particles, springs, particleDamping, windForce, windPeriod);

	// Create a solver.
	//solver = new EulerSolver(particle_system);
	//solver = new AdaptiveEulerSolver(particle_system, 1e3 * m);
	solver = new RungeKuttaSolver(particle_system);

	quadObj = gluNewQuadric();
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

	return 0;
}

