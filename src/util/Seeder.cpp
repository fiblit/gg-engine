#include "Seeder.h"

typedef std::chrono::high_resolution_clock hrclock;

U64 Seeder::_seed = 0;
HRClock::time_point Seeder::_first = HRClock::now();
std::default_random_engine Seeder::_gen = std::default_random_engine();


Seeder::Seeder() {
    static bool seeded = false;
    if (!seeded) {
        _first = hrclock::now();
        reseed();
    }
}

void Seeder::reseed() {
    seed(static_cast<U64>(_first.time_since_epoch().count()));
}

U64 Seeder::seed() {
    return _seed;
}

void Seeder::seed(U64 s) {
    _seed = s;
    _gen.seed(_seed);
}

std::default_random_engine& Seeder::gen() {
    return _gen;
}
