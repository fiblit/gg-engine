#ifndef PRM_H_GUARD
#define PRM_H_GUARD

#include "BoundVolume.h"
#include "../util/Graph.h"
#include <glm/glm.hpp>
#include <memory>

typedef std::unordered_set<NodeId> Points;

class Cspace2d {
public:
    //generates a configuartion space given a list of obstacles and agent
    Cspace2d(std::vector<BoundVolume*> obs, BoundVolume* agent);
    //detects if a point collides with anything in the configuration space
    bool collides(glm::vec2 a);
    //detects if a line segment between a and b collides with the C-space 
    bool line_of_sight(glm::vec2 a, glm::vec2 b);
private:
   std::vector<BoundVolume*> _obstacles;
};

class PRM {
public:
    //samples and connects a Pobabilistic Road Map
    PRM(glm::vec2 start, glm::vec2 goal, std::unique_ptr<Cspace2d> cspace);
    std::unique_ptr<Graph<glm::vec2>> _roadmap;
private:
    std::unique_ptr<Cspace2d> _cspace;

    //Uniformly samples the c-space to generate points for the roadmap.
    //All sampled points will not collide with the static environment 
    void sample_space();
    //threshold search to find nearest neighbors
    std::unique_ptr<Points> nearby(NodeId source, float threshold);
    //connects nearest neighbors by Graph edges.
    void connect();
};

#endif // PRM_H_GUARD
