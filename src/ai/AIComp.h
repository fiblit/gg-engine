#ifndef AI_COMP_H
#define AI_COMP_H

#include "PRM.h"
#include <glm/vec2.hpp>
#include <vector>

struct AIComp {
    glm::vec2 start;
    //this allows for more dynamic path planning.
    glm::vec2 final_goal;
    glm::vec2 local_goal;

    int num_done;
    std::vector<glm::vec2>* plan;

    bool has_plan() {
        return plan != nullptr && static_cast<int>(plan->size()) > 0;
    }

    //planning meta-data
    Cspace2d* cspace;
    PRM* prm;
};

#endif//AI_COMP_H
