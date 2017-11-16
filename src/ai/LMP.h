#ifndef LMP_H
#define LMP_H

#include "BoundVolume.h"
#include "BVH.h"
#include "Agent.h"
#include "Object.h"
#include "PRM.h"

#include <glm/glm.hpp>

namespace LMP {
    float ttc(BoundVolume* i, glm::vec2 iv, BoundVolume* j, glm::vec2 jv);
    float ttc_(Circ* i, glm::vec2 iv, Circ* j, glm::vec2 jv);
    float ttc_(Rect* i, glm::vec2 iv, Rect* j, glm::vec2 jv);
    float ttc_(Circ* i, glm::vec2 iv, Rect* j, glm::vec2 jv);

    glm::vec2 lookahead(Agent* a, glm::vec2 target);

    glm::vec2 ttc_forces_(double ttc, glm::vec2 dir);
    glm::vec2 ttc_forces(Agent* a, Circ* b, float ttc);
    glm::vec2 ttc_forces(Agent* a, Agent* b, float ttc);
    glm::vec2 calc_sum_force(Agent* a,
        BVH<Object>* static_bvh,
        BVH<Agent>* dynamic_bvh,
        std::vector<Object*> statics,//shouldn't be here; temp hack
        std::vector<Agent*> dynamics);//shouldn't be here; temp hack
}

#endif//LMP_H
