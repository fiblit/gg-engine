#ifndef OBJECT_H
#define OBJECT_H

#include <vector>

#include "BoundVolume.h"
#include "../model/Mesh.h"
#include "../Transform.h"

class Object {
public:
    //todo: ownership
    //planning
    //collisions...
    BoundVolume* _bv;
    //rendering
    Transform* _transform;
    Mesh* _mesh;

    //fixes any discontinuities between components (especially position)
    void sync();

    Object(BoundVolume* bv = nullptr, Transform* transform = nullptr,
        Mesh* mesh = nullptr);
};

#endif//AGENT_H
