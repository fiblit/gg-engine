// Copyright (c) 2016-2019 Dalton Hildreth
// This file is under the MIT license. See the LICENSE file for details.
#pragma once

#include "Agent.h"
#include "BVH.h"
#include "BoundVolume.h"
#include "Dynamics.h"
#include "GMP.h"
#include "LMP.h"
#include "PRM.h"

namespace ai {
extern Cspace2d* std_cspace;
extern PRM* std_prm;
extern BVH* static_bvh;
extern BVH* dynamic_bvh;
void init();
void update_agents();
} // namespace ai
