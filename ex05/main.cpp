#define _USE_MATH_DEFINES

#include "MassSpringSystem.h"
#include "EulerSolver.h"
#include "AdaptiveEulerSolver.h"
#include "RungeKuttaSolver.h"
#include "DoublePendulum.h"

#include <cmath>
#include <cstdlib>
#include <fstream>

#if defined(__APPLE_CC__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// http://www.physics.usyd.edu.au/~wheat/dpend_html/

GLint windowWidth = 800, windowHeight = 600;
int old_button = -1, old_state = -1, old_x = -1, old_y = -1;
float modelview_matrix[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

const Time stepsize = 1e-2 * s; // Simulation time step in simulated time.
//const Time stepsize = 1.1e-2 * s; //For task 3
//const Time stepsize = 1e-3 * s; //For task 3

const Stiffness stiffness = 3e7 * g / s / s; // Stiffness of springs in the cloth.
//const Stiffness stiffness = 3.4e7 * g / s / s; //For task 3
//const Stiffness stiffness = 3e6 * g / s / s; //For task 3

const SpringDamping damping = 0 * g / s; // Damping in Springs
//const SpringDamping damping = 1e2 * g / s; //For task 3
//const SpringDamping damping = 1e5 * g / s; //For task 3

const Length scale = 2.0 * m; // Length scale for display.

std::vector<Particle<Length3D> > particles; // List of particles.
std::vector<Spring> springs; // List of springs.
System<Particle<Length3D> > *ms_system = NULL; // The particle system.
Solver<Particle<Length3D> > *ms_solver = NULL; // The solver.
std::vector<Length3D> ms_positions;

const size_t num_el_systems = 1;
//const size_t num_el_systems = 50; // For task 3

std::vector<DoublePendulum *> el_systems;
std::vector<Solver<Particle<Angle> >*> el_solvers;
std::vector<Number3D> el_colors;
std::vector<Length2D> el_positions;

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

	// Draw the track of the Mass-Spring solution.
	glColor3d(0.5, 0.0, 0.0);
	glLineWidth(1.5);
	glBegin(GL_LINE_STRIP);
	for (std::vector<Length3D>::const_iterator i = ms_positions.begin(); i != ms_positions.end(); ++i) {
		glVertex2d((*i)[0] / scale, (*i)[1] / scale);
	}
	glEnd();

	// Draw the track of the Euler-Lagrange solution.
	glLineWidth(1.5);
	if (el_colors.size()) {
		glBegin(GL_LINE_STRIP);
		glColor3d(0.5 * el_colors[0][0], 0.5 * el_colors[0][1], 0.5 * el_colors[0][2]);
		for (std::vector<Length2D>::const_iterator i = el_positions.begin(); i != el_positions.end(); ++i) {
			glVertex2d((*i)[0] / scale, (*i)[1] / scale);
		}
		glEnd();
	}

	// Draw the springs. Colors indicate stress.
	glColor3d(1.0, 0.0, 0.0);
	glLineWidth(3.5);
	glBegin(GL_LINES);
	for (std::vector<Spring>::const_iterator s = springs.begin(); s != springs.end(); ++s) {
		glVertex3d(s->p1->position[0] / scale, s->p1->position[1] / scale, s->p1->position[2] / scale);
		glVertex3d(s->p2->position[0] / scale, s->p2->position[1] / scale, s->p2->position[2] / scale);
	}
	glEnd();

	// Draw the particles.
	for (std::vector<Particle<Length3D> >::const_iterator p = particles.begin(); p != particles.end(); ++p) {
		glPushMatrix();
		glTranslatef(p->position[0] / scale, p->position[1] / scale, p->position[2] / scale);
		glutSolidSphere(0.04 * m / scale, 90, 45);
		glPopMatrix();
	}
	
	if (ms_positions.size() > 30 * seconds / stepsize) ms_positions.clear();
	ms_positions.push_back(particles[2].position);

	// Draw the Euler-Lagrange solution.
	if (el_positions.size() > 30 * seconds / stepsize) el_positions.clear();
	glLineWidth(3.5);
	for (size_t i = 0; i < el_systems.size(); ++i) {
		glColor3d(el_colors[i][0], el_colors[i][1], el_colors[i][2]);

		Length2D p1, p2;
		p1[0] = el_systems[i]->l1 * sin(el_systems[i]->particles[0].position);
		p1[1] = -el_systems[i]->l1 * cos(el_systems[i]->particles[0].position);
		p2[0] = el_systems[i]->l1 * sin(el_systems[i]->particles[0].position) + el_systems[i]->l2 * sin(el_systems[i]->particles[1].position);
		p2[1] = -el_systems[i]->l1 * cos(el_systems[i]->particles[0].position) - el_systems[i]->l2 * cos(el_systems[i]->particles[1].position);

		glBegin(GL_LINE_STRIP);
		glVertex2d(0, 0);
		glVertex2d(p1[0] / scale, p1[1] / scale);
		glVertex2d(p2[0] / scale, p2[1] / scale);
		glEnd();

		glPushMatrix();
		glTranslatef(p1[0] / scale, p1[1] / scale, 0.0);
		glutSolidSphere(0.04 * m / scale, 90, 45);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(p2[0] / scale, p2[1] / scale, 0.0);
		glutSolidSphere(0.04 * m / scale, 90, 45);
		glPopMatrix();

		if (i == 0) el_positions.push_back(p2);
	}

	glutSwapBuffers();
}

void idle() {
	for (std::vector<Solver<Particle<Angle> >*>::iterator i = el_solvers.begin(); i != el_solvers.end(); ++i) {
		(*i)->step(stepsize);
	}
	ms_solver->step(stepsize);
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
	Length3D p;

    // first particle
	particles.push_back(Particle<Length3D>(1 * kg, p, Velocity3D(), true));

    // second particle
	p[0] = sin(120.0 * M_PI / 180.0) * m;
	p[1] = -cos(120.0 * M_PI / 180.0) * m;
	p[2] = 0 * m;
	particles.push_back(Particle<Length3D>(1 * kg, p, Velocity3D()));

    // third particle
	p[0] += sin(60.0 * M_PI / 180.0) * m;
	p[1] += -cos(60.0 * M_PI / 180.0) * m;
	particles.push_back(Particle<Length3D>(1 * kg, p, Velocity3D()));

    // springs
	springs.push_back(Spring(particles[0], particles[1], stiffness, damping));
	springs.push_back(Spring(particles[1], particles[2], stiffness, damping));

	ms_system = new MassSpringSystem(particles, springs, damping); //no particle damping
	ms_solver = new RungeKuttaSolver<Particle<Length3D> >(ms_system);

	for(size_t i = 0; i < num_el_systems; ++i) {
		std::vector<Particle<Angle> > *particles = new std::vector<Particle<Angle> >;
		particles->push_back(Particle<Angle>(1.0 * kg, (i == 0 ? 120 : (120 + rand() / double(RAND_MAX))) * M_PI / 180, 0 / s));
		particles->push_back(Particle<Angle>(1.0 * kg, (i == 0 ?  60 : ( 60 + rand() / double(RAND_MAX))) * M_PI / 180, 0 / s));

		DoublePendulum *el_system = new DoublePendulum(*particles, 1 * m, 1 * m);
		el_systems.push_back(el_system);

		el_solvers.push_back(new RungeKuttaSolver<Particle<Angle> >(el_system));

		Number3D color;
		color[0] = i == 0 ? 0.0 : rand() / double(RAND_MAX);
		color[1] = i == 0 ? 1.0 : rand() / double(RAND_MAX);
		color[2] = i == 0 ? 0.0 : rand() / double(RAND_MAX);
		el_colors.push_back(color);
	}

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

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutMainLoop();

	delete ms_system;
	delete ms_solver;
	return 0;
}

