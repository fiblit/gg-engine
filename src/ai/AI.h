#ifndef AI_H
#define AI_H

#include "Object.h"
#include "Agent.h"
#include "PRM.h"
#include "BoundVolume.h"
#include "GMP.h"
#include "LMP.h"
#include "Dynamics.h"
#include "AIComp.h"
#include "BVH.h"

namespace ai {
    extern Cspace2d* std_cspace;
    extern PRM* std_prm;
    extern BVH<Object>* static_bvh;
    extern BVH<Agent>* dynamic_bvh;
    void init(std::vector<Agent*> dynamics, std::vector<Object*> statics);
    void update_agents(std::vector<Object*> statics, std::vector<Agent*> dynamics);
}

#endif//AI_H
