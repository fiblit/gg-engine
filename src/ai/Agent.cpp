#include "Agent.h"

Agent::Agent(AIComp* ai, BoundVolume* bv, Dynamics* dyn, Transform* transform, Mesh* mesh)
        : _ai(ai), _bv(bv), _dyn(dyn), _transform(transform), _mesh(mesh) {}

void Agent::sync() {
    _ai->start = _bv->_o = glm::vec2(_dyn->pos.x, _dyn->pos.z);
    _transform->set_pos(_dyn->pos);
}
