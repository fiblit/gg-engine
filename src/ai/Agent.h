#ifndef AGENT_H
#define AGENT_H

#include <vector>

#include "AIComp.h"
//used by local planning/physics
#include "Dynamics.h"
#include "../model/Mesh.h"
#include "../Transform.h"

class Agent {
public:
    //todo: ownership
    //planning
    AIComp* _ai;
    //collision avoidance, collisions, physics...
    BoundVolume* _bv;
    Dynamics* _dyn;
    //rendering
    Transform* _transform;
    Mesh* _mesh;

    //fixes any discontinuities between components (especially position)
    void sync();

    Agent(AIComp* ai = nullptr, BoundVolume* bv = nullptr, Dynamics* dyn = nullptr,
        Transform* transform = nullptr, Mesh* mesh = nullptr);

    bool has_boid_f() {return false;};
    bool has_indy_f() {return true;};
    bool is_agent() {return true;};
};

#endif//AGENT_H
