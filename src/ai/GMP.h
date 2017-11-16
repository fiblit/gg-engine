#ifndef GMP_H
#define GMP_H

#include "PRM.h"
#include "Agent.h"
#include <vector>

namespace GMP {
//custom A* search for a PRM graph. Note: I would like to generalize this to
//any graph & cost function.
PointPath* find_path_astar(float e, Graph<glm::vec2>* roadmap,
    NodeId start, NodeId goal);
//Custom uniform cost search (aka Djikstra's search) for a PRM Graph
//simplifciation of A*. (h = 0)
PointPath* find_path_ucs(Graph<glm::vec2>* roadmap, NodeId start, NodeId goal);
//Plan every agent.
void replan(std::vector<Agent*> agents);
//plan the given agent
void plan_one(Agent* agent);
//true = need to replan; false = don't please.
//mainly for when there is a moving target.
bool invalid(Agent* agent);
}//GMP::
#endif//GMP_H
