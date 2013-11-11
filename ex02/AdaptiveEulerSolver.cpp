#include "AdaptiveEulerSolver.h"
using namespace std;

void AdaptiveEulerSolver::step(const Time stepsize) {
    Time passed_time;

    // repeat until enough time has passed
    while (passed_time < stepsize) {
        vector<Length3D> r0, r1, r2_1, r2_2;
        vector<Velocity3D> v0, v2;

        // store original r and v
        for(unsigned int i = 0; i < _system->particles.size(); ++i) {
            Particle* p = &_system->particles[i];
            r0.push_back(p->position);
            v0.push_back(p->velocity);
        }

        // do an Euler step, store positions in r1
        for(unsigned int i = 0; i < _system->particles.size(); ++i) {
            Particle* p = &_system->particles[i];
            r1.push_back(p->position + stepsize*p->velocity);
        }


        // do two Euler steps of half stepsize, store resulting positions in r2_2
        _system->computeAccelerations();
        for(unsigned int i = 0; i < _system->particles.size(); ++i) {
            Particle* p = &_system->particles[i];
            r2_1.push_back(p->position + .5*stepsize*p->velocity);
            v2.push_back(p->velocity + .5*stepsize*p->acceleration);
            p->position = r2_1[i];
        }
        for(unsigned int i = 0; i < _system->particles.size(); ++i) {
            Particle* p = &_system->particles[i];
            r2_2.push_back(r2_1[i] + .5*stepsize*v2[i]);
        }

        // find maximal distance between the two calculations
        Length max;
        for(unsigned int i = 0; i < _system->particles.size(); ++i) {
            Length d = norm(r1[i]-r2_2[i]);
            if (d > max) {
                max = d;
            }
        }

        // calculate step size 'hmax' (from the lecture, slide 22)
        Time hmax;
        Length null;
        if (max > null ) {
            hmax = stepsize*sqrt(_threshold/max);
        } else {
            hmax = stepsize;
        }

        // do an Euler step of this step size
        for(unsigned int i = 0; i < _system->particles.size(); ++i) {
            Particle* p = &_system->particles[i];
            p->position = r0[i];
        }
        _system->computeAccelerations();
        for(unsigned int i = 0; i < _system->particles.size(); ++i) {
            Particle* p = &_system->particles[i];
            p->position += hmax*v0[i];
            p->velocity += hmax*p->acceleration;
        }

        _system->time += hmax;
        passed_time += hmax;
    }
}
