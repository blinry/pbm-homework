#include "Spring.h"

Force3D Spring::getForce() const {
    Number3D dir = (p2->position - p1->position)/norm(p2->position - p1->position);
    Force3D fs = stiffness*(norm(p2->position - p1->position) - length)*dir;
    Force3D fd = dot(damping*(p2->velocity - p1->velocity),dir)*dir;

    return fs+fd;
}

