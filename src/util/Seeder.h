// Copyright (c) 2016-2019 Dalton Hildreth
// This file is under the MIT license. See the LICENSE file for details.
#pragma once

#include <chrono>
#include <cstdint>
#include <random>

typedef std::chrono::high_resolution_clock HRClock;

// singleton seed generator
class Seeder {
public:
    Seeder();

    void reseed();
    uint64_t seed();
    void seed(uint64_t s);
    std::default_random_engine& gen();

private:
    static uint64_t _seed;
    static HRClock::time_point _first;
    static std::default_random_engine _gen;
};
