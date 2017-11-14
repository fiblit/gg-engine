#ifndef SEEDER_H
#define SEEDER_H

#include <random>
#include <chrono>

typedef unsigned long long U64;
typedef std::chrono::high_resolution_clock HRClock;

class Seeder {
public:
    Seeder();

    void reseed();
    U64 seed();
    void seed(U64 s);
    std::default_random_engine& gen();
private:
    static U64 _seed;
    static HRClock::time_point _first;
    static std::default_random_engine _gen;
};
#endif//SEEDER_H
