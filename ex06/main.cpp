#include <vector>
#include <cstdlib>

#if defined(__APPLE_CC__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <Box2D.h>

GLint windowWidth = 800, windowHeight = 600; // window dimensions
b2World world(b2Vec2(0.0, -9.81), true); // Box2D world with given gravity
std::vector<b2Body*> bodies; // list of all bodies in the world

// display callback: renders the scene
void display() {
	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-0.1 * windowWidth, 0.1 * windowWidth, -0.1 * windowHeight, 0.1 * windowHeight, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);

	// render bodies
	for (std::vector<b2Body*>::const_iterator body = bodies.begin(); body != bodies.end(); ++body) {
		// color
		glColor3f((*body)->IsAwake(), 1.0 - (*body)->IsActive(), 1.0 - (*body)->IsAwake());

		// position and orientation
		b2Vec2 position = (*body)->GetPosition();
		float32 angle = (*body)->GetAngle();
		glPushMatrix();
		glTranslated(position.x, position.y, 0.0);
		glRotated(180.0 * angle / M_PI, 0.0, 0.0, 1.0);

		// vertices
		for (b2Fixture *fixture = (*body)->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
			if (fixture->GetShape()->GetType() == b2Shape::e_polygon) {
				glBegin(GL_POLYGON);
				b2PolygonShape *shape = dynamic_cast<b2PolygonShape*>(fixture->GetShape());
				for (int32 i = 0; i < shape->GetVertexCount(); ++i) {
					b2Vec2 v = shape->GetVertex(i);
					glVertex2d(v.x, v.y);
				}
				glEnd();
			}
		}
		glPopMatrix();
	}

	glutSwapBuffers();
}

// reshape callback: sets window width and height
void reshape(int width, int height) {
	windowWidth = width;
	windowHeight = height;
	glutPostRedisplay();
}

// perform time step
void idle() {
	world.Step(1.0 / 60.0, 6, 2);
	world.ClearForces();

	glutPostRedisplay();
}

void make_body(double x0, double x1, double y0, double y1, bool fixed=false) {
	/* Please insert your code for creating a body here. The body should be
	 * a box with corners at (x0, y0), (x0, y1), (x1, y1), and (x1, y0),
	 * respectively. Note that Box2D uses the center, half width and half
	 * height to specify boxes.
	 *
	 * You can store the properties of the body in a b2BodyDef object. Then
	 * you can use the CreateBody factory method of the Box2D world to
	 * create the body. Remember to set the body type to "dynamic" if it is
	 * not "fixed".
	 *
	 * Then, you should create a shape and a fixture that are necessary for
	 * collisions. The shape stores the dimensions of the box. For a "fixed"
	 * body you can directly create a fixture by passing the shape to the
	 * CreateFixture member function of your body. For a dynamic body, you
	 * will need to create a b2FixtureDef object that also defines the
	 * density, friction and restitution of the body.
	 *
	 * Finally, append the body to the "bodies" list so it will be painted
	 * in the "display" method.
	 */
}

int main(int argc, char **argv) {
	// create bodies
	for (int y = 0; y < 6; ++y) {
		for (int x = -29 - 2 * y; x <= 29 + 2 * y; x += 2)
			make_body(x - 0.25, x + 0.25, 50 - 10 * y, 50 - 10 * y + 4);
		make_body(-30 - 2 * y, 30 + 2 * y, 49 - 10 * y, 50 - 10 * y, true);
	}

	for (int y = 0; y < 11; ++y) {
		for (int x = -58 + 2 * y; x <= 58 - 2 * y; x += 4)
			make_body(x - 0.25, x + 0.25, -59 + 4.5 * y, -59 + 4.5 * y + 4);
		for (int x = -56 + 2 * y; x <= 56 - 2 * y; x += 4)
			make_body(x - 2, x + 2, -55 + 4.5 * y, -54.5 + 4.5 * y);
	}

	/* To start your domino chain, rotate the first body by a few degrees. */

	// create surrounding box
	make_body(-80.0, 80.0, -60.0, -59.0, true);
	make_body(-80.0, 80.0, 59.0, 60.0, true);
	make_body(-80.0, -79.0, -60.0, 60.0, true);
	make_body(79.0, 80.0, -60.0, 60.0, true);

	// enable smoothing
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
	glHint(GL_POINT_SMOOTH, GL_NICEST);
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	// start glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Solid Body System");
	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();

	return 0;
}

