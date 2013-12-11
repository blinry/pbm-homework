#include <cmath>
#include <GL/glfw.h>
#include "CollisionDetector.h"
#include "Spaceship.h"
#include "Asteroid.h"
#include <stdio.h>

GLint windowWidth = 512, windowHeight = 512;
float angle_step = 0.5, speed_step = 1.0;
float modelview_matrix[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
float rotation_speed = 0.1, translation_speed = 0.01;
int old_x, old_y;
double last_t;
bool you_lose = false;

std::vector<Object*> objects;
CollisionDetector *cd = NULL;
Spaceship *ship = NULL;

// window reshape callback
void reshape(int width, int height) {
	windowWidth = width;
	windowHeight = height;
}

// display callback
void display() {
	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, windowWidth / float(windowHeight), 0.01, 100.0);
	glMultMatrixf(modelview_matrix);
	ship->set_camera();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_LIGHTING);
	if (you_lose) {
		glColor3f(1.0, 0.0, 0.0);
	} else {
		glColor3f(0.0, 1.0, 0.0);
	}
    std::vector<std::pair<const AABB*, const AABB*> > collisions = cd->detect_possible_collisions();
	for (size_t i = 0; i < objects.size(); ++i) {
        glColor3f(0.0, 1.0, 0.0);
        for (size_t j = 0; j < collisions.size(); ++j) {
            if (objects[i]->bounding_box() == collisions[j].first || 
                objects[i]->bounding_box() == collisions[j].second) {
                glColor3f(1.0, 0.0, 0.0);
                break;
            }
        }
		objects[i]->bounding_box()->draw();
    }

	for (size_t i = 0; i < objects.size(); ++i)
		objects[i]->draw();

	glfwSwapBuffers();
}

// idle callback
void idle() {
	if (you_lose)
		return;

	double t = glfwGetTime();
	double dt = t - last_t;
	last_t = t;

	// move the spaceship
	if (glfwGetKey('+') == GLFW_PRESS || glfwGetKey('W') == GLFW_PRESS)
		ship->speed(ship->speed() + speed_step * dt);
	if (glfwGetKey('-') == GLFW_PRESS || glfwGetKey('S') == GLFW_PRESS)
		ship->speed(ship->speed() - speed_step * dt);
	if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS)
		ship->rotate_leftright(angle_step * dt);
	if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS)
		ship->rotate_leftright(-angle_step * dt);
	if (glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS)
		ship->rotate_updown(-angle_step * dt);
	if (glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS)
		ship->rotate_updown(angle_step * dt);
	ship->update(dt);

	// detect collisions
	std::vector<std::pair<const AABB*, const AABB*> > collisions = cd->detect_possible_collisions();
	for (size_t i = 0; i < collisions.size(); ++i)
		if ((collisions[i].first->parent() == ship && ship->collides(dynamic_cast<const Asteroid*>(collisions[i].second->parent()))) ||
				(collisions[i].second->parent() == ship && ship->collides(dynamic_cast<const Asteroid*>(collisions[i].first->parent()))))
			you_lose = true;
}

// mouse motion callback
void motion(int x, int y) {
	float dx = x - old_x;
	float dy = old_y - y;

	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) { // rotate
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(rotation_speed * sqrt(dx * dx + dy * dy), dy, -dx, 0.0);
		glMultMatrixf(modelview_matrix);
		glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
	}

	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) { // strafe
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(translation_speed * dx, translation_speed * dy, 0.0);
		glMultMatrixf(modelview_matrix);
		glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
	}

	if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) { // translate
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0, 0, translation_speed * dy);
		glMultMatrixf(modelview_matrix);
		glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
	}

	old_x = x;
	old_y = y;
}

int main(int argc, const char *argv[]) {
	for (size_t i = 0; i < 100; ++i) {
		objects.push_back(new Asteroid(
					10.0 * (rand() / double(RAND_MAX) - 0.5),
					10.0 * (rand() / double(RAND_MAX) - 0.5),
					10.0 * (rand() / double(RAND_MAX) - 0.5),
					0.5 * rand() / double(RAND_MAX)
					));
	}

	// initialize spaceship
	ship = new Spaceship;
	objects.push_back(ship);

	// initialize collision detection
	cd = new CollisionDetector(objects.begin(), objects.end());

	// initialize GLFW window
	glfwInit();
	glfwOpenWindow(windowWidth, windowHeight, 8, 8, 8, 0, 8, 0, GLFW_WINDOW);
	glfwSetWindowSizeCallback(reshape);
	glfwSetMousePosCallback(motion);
	last_t = glfwGetTime();

	// enable lighting
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);

	// main loop
	bool running = true;
	while (running) {
		idle();
		display();
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	}

	// terminate GLFW window
	glfwTerminate();

	return 0;
}

