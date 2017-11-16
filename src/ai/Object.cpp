#include "Object.h"

Object::Object(BoundVolume* bv, Transform* transform, Mesh* mesh)
        : _bv(bv), _transform(transform), _mesh(mesh) {}

void Object::sync() {
    glm::vec3 p = _transform->pos();
    _bv->_o = glm::vec2(p.x, p.z);
}

