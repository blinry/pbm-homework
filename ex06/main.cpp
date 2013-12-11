#include <vector>
#include <cstdlib>

#if defined(__APPLE_CC__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <cmath>

#include <btBulletDynamicsCommon.h>
#include <GLDebugDrawer.h>

GLint windowWidth = 800, windowHeight = 600; // window dimensions
btCollisionShape* groundShape;
btDiscreteDynamicsWorld* dynamicsWorld;
std::vector<btRigidBody*> objects;
btRigidBody* player;
btRigidBody* first;

int old_button = -1, old_state = -1, old_x = -1, old_y = -1;
float modelview_matrix[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};


// display callback: renders the scene
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

    dynamicsWorld->debugDrawWorld();

    glFlush();
	glutSwapBuffers();
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

    bool clean = true;
    for(int i=0; i<objects.size(); i++) {
        btRigidBody *o = objects[i];
        if (o == player) {
            continue;
        }
        if(o->getCenterOfMassPosition().y() > -10) {
            clean = false;
        }
    }

	glutPostRedisplay();
}

void make_body(float x, float y, float z, float a, float b, float c, float yaw, float pitch, float roll, const bool fixed = false) {
	btCollisionShape* box;
	box = new btBoxShape(btVector3(a/2, b/2, c/2));

	btDefaultMotionState* boxMotionState = new btDefaultMotionState(btTransform(btQuaternion(yaw, pitch, roll),btVector3(x, y, z)));
	btScalar mass = a*b*c;
	if(fixed)
		mass = 0.0;

	btVector3 fallInertia(0,0,0);
	box->calculateLocalInertia(mass,fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,boxMotionState,box,fallInertia);
	btRigidBody* boxRigidBody = new btRigidBody(fallRigidBodyCI);
	objects.push_back(boxRigidBody);
	dynamicsWorld->addRigidBody(boxRigidBody);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 27:
			exit(0);
			break;
        case 97:
            player->activate();
            player->applyCentralForce(btVector3(-10000,0,0));
            break;
        case 100:
            player->activate();
            player->applyCentralForce(btVector3(10000,0,0));
            break;
        case 115:
            player->activate();
            player->applyCentralForce(btVector3(0,0,10000));
            break;
        case 119:
            player->activate();
            player->applyCentralForce(btVector3(0,0,-10000));
            break;
        case 32: 
            first->activate();
            first->applyForce(btVector3(2/1000.0,0,4/1000.0),btVector3(0,0,0));
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
    dynamicsWorld->getDebugDrawer()->setDebugMode(1);

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
	//make_body(-30, 30, -2, -2.1, true);
    make_body(0, -0.1, 0, 80, 0.1, 80, 0, 0, 0, true);

    make_body(30, 0.2, 30, 10, 10, 10, 0, 0, 0);
    player = objects[1];

	//for (int x = -29; x <= 29; x += 2)
    //for(double i=0; i<2*M_PI-M_PI/32; i+=M_PI/32)
    for(double i=0; i<150; i++)
	{
        double r = 3+pow(i,1.2)/2.0;
        double t = 5;
        double f = r/10;
        make_body(r*cos(i/t)/10.0, f*2/10.0, r*sin(i/t)/10.0, f*0.5/10.0, f*4/10.0, f*1.5/10.0, -i/t+M_PI/2, 0, 0);
	}

    first = objects[2];

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
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
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

