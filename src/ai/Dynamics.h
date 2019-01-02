// Copyright (c) 2016-2019 Dalton Hildreth
// This file is under the MIT license. See the LICENSE file for details.

#pragma once
struct Dynamics {
    // TODO: remove redundancies that need to be synced.
    glm::vec3 pos;
    glm::vec3 vel;
    glm::vec3 acc;
    glm::vec3 force;
    float mass;
};
