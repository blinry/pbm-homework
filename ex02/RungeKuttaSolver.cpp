#include "RungeKuttaSolver.h"
using namespace std;

void RungeKuttaSolver::step(const Time stepsize)  {
    vector<Length3D> r1, r2, r3, r4;
    vector<Velocity3D> v1, v2, v3, v4;
    vector<Acceleration3D> a1, a2, a3, a4;

    // store original position and velocity
    for(unsigned int i = 0; i < _system->particles.size(); ++i) {
        Particle* p = &_system->particles[i];
        r1.push_back(p->position);
        v1.push_back(p->velocity);
    }
    // ... and compute the particles accelerations
    compAcc(r1, &a1);

    // do a half explicit Euler step using original velocity and the computed acceleration,
    // store new positions and velocities in r2 and v2
    eulerStep(r1, v1, a1, &r2, &v2, stepsize*0.5);
    // recompute accelerations
    compAcc(r2, &a2);

    // another half step: Start at original position and velocity, but use velocity and
    // acceleration computed in the prior step
    eulerStep(r1, v2, a2, &r3, &v3, stepsize*0.5);
    compAcc(r3, &a3);

    // and another, use full stepsize this time
    eulerStep(r1, v3, a3, &r4, &v4, stepsize);
    compAcc(r4, &a4);

    // calculate final position and velocity by weighting the individual values
    for(unsigned int i = 0; i < _system->particles.size(); ++i) {
        Particle* p = &_system->particles[i];
        p->position = r1[i] + stepsize/6.0*(v1[i] + 2*v2[i] + 2*v3[i] + v4[i]);
        p->velocity = p->velocity + stepsize/6.0*(a1[i] + 2*a2[i] + 2*a3[i] + a4[i]);
    }

    _system->time += stepsize;
}

// compute each particles' acceleration a for positions r. Note: Overwrites the positions!
void RungeKuttaSolver::compAcc(vector<Length3D> r, vector<Acceleration3D>* a) {
    for(unsigned int i = 0; i < _system->particles.size(); ++i) {
        Particle* p = &_system->particles[i];
        p->position = r[i];
    }
    _system->computeAccelerations();
    for(unsigned int i = 0; i < _system->particles.size(); ++i) {
        Particle* p = &_system->particles[i];
        a->push_back(p->acceleration);
    }
}

// do an explicit Euler step, starting at r and the particles' velocity
void RungeKuttaSolver::eulerStep(vector<Length3D> r, vector<Velocity3D> v, vector<Acceleration3D> a, vector<Length3D>* r2, vector<Velocity3D>* v2, Time stepsize) {
    for(unsigned int i = 0; i < _system->particles.size(); ++i) {
        Particle* p = &_system->particles[i];
        r2->push_back(r[i]+stepsize*v[i]);
        v2->push_back(p->velocity+stepsize*a[i]);
    }
}
