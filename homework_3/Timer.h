

#ifndef HOMEWORK_3_TIMER_H
#define HOMEWORK_3_TIMER_H

#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <fstream>

class Timer {
private:
    using clock_t = std::chrono::steady_clock;
    using time_point_t = clock_t::time_point;
    using milliseconds = std::chrono::milliseconds;

    time_point_t start;
    milliseconds ms{};

public:

    Timer() {
        ms = milliseconds(0);
        start = clock_t::now();
    }

    void start_timer() {
        start = clock_t::now();
    };

    void pause_timer() {
        ms += std::chrono::duration_cast<milliseconds>(clock_t::now() - start);
    }

    void reset_timer() {
        ms = milliseconds(0);
    }

    void print_time(std::ofstream& f) {
        this->pause_timer();
        f << ms.count() << " ";
    }
};


#endif //HOMEWORK_3_TIMER_H
