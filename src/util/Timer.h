// Copyright (c) 2016-2019 Dalton Hildreth
// This file is under the MIT license. See the LICENSE file for details.
#pragma once

#include <chrono>

class Timer {
public:
    // create a timer which starts ticking now
    Timer();

    // advance clock to now
    void tick();

    std::chrono::duration<double> delta();
    double delta_ms();
    double delta_s();
    std::chrono::time_point<std::chrono::steady_clock> time();

    double operator-(Timer& timer);
    double operator-(std::chrono::time_point<std::chrono::steady_clock> t);

private:
    // Time of this frame; steady_clock so we can measure intervals
    std::chrono::time_point<std::chrono::steady_clock> _time;
    // Difference in time between this frame and the last frame
    std::chrono::duration<double> _delta;
};
