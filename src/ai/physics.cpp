#include "physics.h"
#include "Pool.h"

namespace physics {
void init() {}

void simulate(float dt) {
    //NO COLLISION DETECTION :(
    POOL.for_<Dynamics>([&](Dynamics& d, const Entity&) {
        //midpoint integration, I think...
        glm::vec3 next_a = d.force/d.mass;
        glm::vec3 half_a = (d.acc + next_a)/2.f;
        /*glm::vec3 next_v = d.vel + dt * half_a;
        glm::vec3 half_v = (d.vel + next_v)/2.f;
        d.pos = d.pos + dt * half_v;
        d.vel = next_v;*/
        d.pos += dt * d.vel + 0.5f * dt * dt * half_a;
        d.vel += dt * half_a;
        d.acc = next_a;
    });
}
}
