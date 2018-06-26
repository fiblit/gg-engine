// Copyright (c) 2016-2018 Dalton Hildreth
// This file is under the MIT license. See the LICENSE file for details.
#ifndef ENTITY_H
#define ENTITY_H

#include <utility>
#include <string>
#include <cstdint>

const unsigned N_COMP = 5;

struct Entity {
    Entity(uint16_t i) : id(i), comp{} {};

    //the only important thing is the ID
    /*const*/ uint16_t id;
    //reverse pointers to the components attached to this entity; maybe these
    //are unnecessary or wasteful
    uint16_t comp[N_COMP];
};

#endif//ENTITY_H
