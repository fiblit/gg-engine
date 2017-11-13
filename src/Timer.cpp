#include "Timer.h"

Timer::Timer() {
    _time = std::chrono::steady_clock::now();
    _delta = std::chrono::duration<double>::zero();
}

void Timer::tick() {
    auto last = _time;
    _time = std::chrono::steady_clock::now();
    _delta = _time - last;
}

std::chrono::duration<double> Timer::delta() {
    return _delta;
}

double Timer::delta_ms() {
    return std::chrono::duration<double, std::milli>(_delta).count();
}

double Timer::delta_s() {
    return _delta.count();
}

std::chrono::time_point<std::chrono::steady_clock> Timer::time() {
    return _time;
}
