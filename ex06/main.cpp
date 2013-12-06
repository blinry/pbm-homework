#include <vector>
#include <cstdlib>

#if defined(__APPLE_CC__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define M_PI 3.141592

#include <bullet/btBulletDynamicsCommon.h>
#include <GLDebugDrawer.h>

GLint windowWidth = 800, windowHeight = 600; // window dimensions
btCollisionShape* groundShape;
btDiscreteDynamicsWorld* dynamicsWorld;
std::vector<btRigidBody*> objects;


// display callback: renders the scene
void display() {
	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-0.1 * windowWidth, 0.1 * windowWidth, -0.1 * windowHeight, 0.1 * windowHeight, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);

    dynamicsWorld->debugDrawWorld();

    glFlush();
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
	//world.Step(1.0 / 60.0, 6, 2);
	dynamicsWorld->stepSimulation(1/60.f,10);
	//world->ClearForces();

	glutPostRedisplay();
}

void make_body(double x0, double x1, double y0, double y1, const bool fixed = false) 
{
	btCollisionShape* domino;
	domino = new btBoxShape(btVector3(btScalar(abs(x1-x0)/2.0),btScalar(abs(y1-y0)/2.0),btScalar(1)));

	btDefaultMotionState* dominoMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3((x0+x1)/2.0,(y1+y0)/2.0,0)));
	btScalar mass = 0.1;
	if(fixed)
		mass = 0.0;

	btVector3 fallInertia(0,0,0);
	domino->calculateLocalInertia(mass,fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,dominoMotionState,domino,fallInertia);
	btRigidBody* dominoRigidBody = new btRigidBody(fallRigidBodyCI);
	objects.push_back(dominoRigidBody);
	dynamicsWorld->addRigidBody(dominoRigidBody);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 27:
			exit(0);
			break;
	}
}

int main(int argc, char **argv) {

    // Build the broadphase
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();

    // Set up the collision configuration and dispatcher
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

    // The actual physics solver
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    // The world.
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0,-10,0));

    GLDebugDrawer dd;

    dynamicsWorld->setDebugDrawer(&dd);
    dynamicsWorld->getDebugDrawer()->setDebugMode(true);

	//// create bodies
	//for (int y = 0; y < 6; ++y) {
	//	for (int x = -29 - 2 * y; x <= 29 + 2 * y; x += 2)
	//		make_body(x - 0.25, x + 0.25, 50 - 10 * y, 50 - 10 * y + 4);
	//	make_body(-30 - 2 * y, 30 + 2 * y, 49 - 10 * y, 50 - 10 * y, true);
	//}

	//for (int y = 0; y < 11; ++y) {
	//	for (int x = -58 + 2 * y; x <= 58 - 2 * y; x += 4)
	//		make_body(x - 0.25, x + 0.25, -59 + 4.5 * y, -59 + 4.5 * y + 4);
	//	for (int x = -56 + 2 * y; x <= 56 - 2 * y; x += 4)
	//		make_body(x - 2, x + 2, -55 + 4.5 * y, -54.5 + 4.5 * y);
	//}

	///* To start your domino chain, rotate the first body by a few degrees. */

	//// create surrounding box
	//make_body(-80.0, 80.0, -60.0, -59.0, true);
	//make_body(-80.0, 80.0, 59.0, 60.0, true);
	//make_body(-80.0, -79.0, -60.0, 60.0, true);
	//make_body(79.0, 80.0, -60.0, 60.0, true);

	//ground plane
	make_body(-30, 30, -2, -2.1, true);

	for (int x = -29; x <= 29; x += 2)
	{
		make_body(x - 0.5, x + 0.5, 1, 5);
	}
    objects[1]->applyForce(btVector3(0,-20,0),btVector3(3,0,0));

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
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	// Clean up behind ourselves like good little programmers
    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;

	return 0;
}

