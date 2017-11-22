#ifndef ENTITY_H
#define ENTITY_H

#include <utility>
#include <string>
#include <cstdint>

const int N_COMP = 5;
enum class Comp {TRANSFORM, MESH, DYNAMICS, BOUND_VOLUME, AGENT};

struct Entity {
    Entity(uint16_t i) : id(i), comp({0}) {};
    //the only important thing is the ID
    const uint16_t id;
    //reverse pointers to the components attached to this entity; maybe these
    //are unnecessary or wasteful
    uint16_t comp[N_COMP];
};

#endif//ENTITY_H
