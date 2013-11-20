#include "MassSpringSystem.h"
#include "RungeKuttaSolver.h"
#include "Plane.h"

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
float modelview_matrix[16] = {1, 0, 0, 0,
                              0, 1, 0, 0,
                              0, 0, 1, 0,
                              0, 0, -6, 1};
bool running = true;

const Time stepsize = 1e-3 * s; // Simulation time step in simulated time.
const size_t dimx = 2, dimy = 2, dimz = 2; // Number of particles in the solid body.
const Stiffness stiffness = 90e4 * g / s / s; // Stiffness of springs in the solid body.
const SpringDamping springDamping = 90.0 * g / s; // Damping in springs.
const ParticleDamping particleDamping = 1.0 * g / s; // Damping in particles.
const Mass mass = 150.0 * g / (dimx * dimy); // Mass of each particle.
const Length scale = 2.0 * m; // Length scale for display.
float shrinkage = 1.0; // Shrinkage of springs with respect to their initial length.
ReflectionCoefficient bounciness = 3e4 * kg / s / s; // Bounciness of the planes.
Number friction = 0.6; // Friction coefficient of the planes.

std::vector<Particle> particles; // List of particles.
std::vector<Spring> springs; // List of springs.
std::vector<Obstacle*> obstacles; // List of obstacles.
System *particle_system = NULL; // The particle system.
Solver *solver = NULL; // The solver.
GLUquadricObj *quadObj = NULL; // A quadric object for drawing the flag pole.

void display() {
	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, windowWidth / float(windowHeight), 0.01, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(modelview_matrix);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw the obstacles.
	srand(0);
	for (std::vector<Obstacle*>::const_iterator p = obstacles.begin(); p != obstacles.end(); ++p) {
		glColor3f(rand() / double(RAND_MAX), 0.5 * rand() / double(RAND_MAX), rand() / double(RAND_MAX));
		(*p)->draw(scale);
	}

	// Draw faces between adjacent particles.
	glBegin(GL_TRIANGLES);
	glColor3d(1.0, 0.5, 0.0);
	for (size_t z = 0; z < dimz - 1; ++z) {
		for (size_t y = 0; y < dimy - 1; ++y) {
			for (size_t x = 0; x <= dimx - 1; x += (dimx > 1 ? dimx - 1 : 1)) {
				Length3D p1 = particles[z * dimy * dimx + y * dimx + x].position;
				Length3D p2 = particles[z * dimy * dimx + (y + 1) * dimx + x].position;
				Length3D p3 = particles[(z + 1) * dimy * dimx + (y + 1) * dimx + x].position;
				Length3D p4 = particles[(z + 1) * dimy * dimx + y * dimx + x].position;
				glVertex3d(p1[0] / scale, p1[1] / scale, p1[2] / scale);
				glVertex3d(p2[0] / scale, p2[1] / scale, p2[2] / scale);
				glVertex3d(p3[0] / scale, p3[1] / scale, p3[2] / scale);
				glVertex3d(p1[0] / scale, p1[1] / scale, p1[2] / scale);
				glVertex3d(p3[0] / scale, p3[1] / scale, p3[2] / scale);
				glVertex3d(p4[0] / scale, p4[1] / scale, p4[2] / scale);
			}
		}
	}
	glColor3d(0.0, 1.0, 0.5);
	for (size_t z = 0; z < dimz - 1; ++z) {
		for (size_t y = 0; y <= dimy - 1; y += (dimy > 1 ? dimy - 1 : 1)) {
			for (size_t x = 0; x < dimx - 1; ++x) {
				Length3D p1 = particles[z * dimy * dimx + y * dimx + x].position;
				Length3D p2 = particles[(z + 1) * dimy * dimx + y * dimx + x].position;
				Length3D p3 = particles[(z + 1) * dimy * dimx + y * dimx + x + 1].position;
				Length3D p4 = particles[z * dimy * dimx + y * dimx + x + 1].position;
				glVertex3d(p1[0] / scale, p1[1] / scale, p1[2] / scale);
				glVertex3d(p2[0] / scale, p2[1] / scale, p2[2] / scale);
				glVertex3d(p3[0] / scale, p3[1] / scale, p3[2] / scale);
				glVertex3d(p1[0] / scale, p1[1] / scale, p1[2] / scale);
				glVertex3d(p3[0] / scale, p3[1] / scale, p3[2] / scale);
				glVertex3d(p4[0] / scale, p4[1] / scale, p4[2] / scale);
			}
		}
	}
	glColor3d(0.5, 0.0, 1.0);
	for (size_t z = 0; z <= dimz - 1; z += (dimz > 1 ? dimz - 1 : 1)) {
		for (size_t y = 0; y < dimy - 1; ++y) {
			for (size_t x = 0; x < dimx - 1; ++x) {
				Length3D p1 = particles[z * dimy * dimx + y * dimx + x].position;
				Length3D p2 = particles[z * dimy * dimx + (y + 1) * dimx + x].position;
				Length3D p3 = particles[z * dimy * dimx + (y + 1) * dimx + x + 1].position;
				Length3D p4 = particles[z * dimy * dimx + y * dimx + x + 1].position;
				glVertex3d(p1[0] / scale, p1[1] / scale, p1[2] / scale);
				glVertex3d(p2[0] / scale, p2[1] / scale, p2[2] / scale);
				glVertex3d(p3[0] / scale, p3[1] / scale, p3[2] / scale);
				glVertex3d(p1[0] / scale, p1[1] / scale, p1[2] / scale);
				glVertex3d(p3[0] / scale, p3[1] / scale, p3[2] / scale);
				glVertex3d(p4[0] / scale, p4[1] / scale, p4[2] / scale);
			}
		}
	}
	glEnd();
	
	glutSwapBuffers();
}

void idle() {
	if (running)
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
			glTranslatef(0, 0, -0.01 * dy);
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
		case ' ':
			running = !running;
			glutPostRedisplay();
			break;
		case 'r':
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslated(0.0, 0.0, -6.0);
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
	// Create particles and springs.
	particles.reserve(dimx * dimy * dimz);
	for (size_t z = 0; z < dimz; ++z) {
		for (size_t y = 0; y < dimy; ++y) {
			for (size_t x = 0; x < dimx; ++x) {
				Length3D p;
				p[0] = (x / float(dimx > 1 ? dimx - 1 : 1) - 0.5) * m - 6 * m;
				p[1] = (y / float(dimy > 1 ? dimy - 1 : 1) - 0.5) * m + 5 * m;
				p[2] = (z / float(dimz > 1 ? dimz - 1 : 1) - 0.5) * m;
				particles.push_back(Particle(mass, p, Velocity3D()));
				// create connections to all existing neighbors
				std::vector<size_t> other_xs, other_ys, other_zs;
				if (z > 0) other_zs.push_back(z - 1);
				if (y > 0) other_ys.push_back(y - 1);
				if (x > 0) other_xs.push_back(x - 1);
				other_zs.push_back(z);
				other_ys.push_back(y);
				other_xs.push_back(x);
				if (z + 1 < dimz) other_zs.push_back(z + 1);
				if (y + 1 < dimy) other_ys.push_back(y + 1);
				if (x + 1 < dimx) other_xs.push_back(x + 1);
				for (std::vector<size_t>::const_iterator oz = other_zs.begin(); oz != other_zs.end(); ++oz) {
					for (std::vector<size_t>::const_iterator oy = other_ys.begin(); oy != other_ys.end(); ++oy) {
						for (std::vector<size_t>::const_iterator ox = other_xs.begin(); ox != other_xs.end(); ++ox) {
							size_t idx = *oz * dimy * dimx + *oy * dimx + *ox;
							if (idx < particles.size() - 1) {
								springs.push_back(Spring(particles[idx], particles.back(), stiffness, springDamping, shrinkage));
							}
						}
					}
				}
			}
		}
	}

	// Create obstacles.
	Number3D plane_normal;
	plane_normal[0] = 0.5; plane_normal[1] = 1.0; plane_normal[2] = 0.0;
	obstacles.push_back(new Plane(plane_normal / norm(plane_normal), -1.0 * m, bounciness, friction));
	plane_normal[0] = 0.0; plane_normal[1] = 1.0; plane_normal[2] = 0.0;
	obstacles.push_back(new Plane(plane_normal / norm(plane_normal), -1.5 * m, bounciness, friction));
	plane_normal[0] = -1.0; plane_normal[1] = 0.0; plane_normal[2] = 0.0;
	obstacles.push_back(new Plane(plane_normal / norm(plane_normal), -5.0 * m, bounciness, friction));

	// Create a particle system.
	particle_system = new MassSpringSystem(particles, springs, obstacles, particleDamping);

	// Create a solver.
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
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();

	return 0;
}

