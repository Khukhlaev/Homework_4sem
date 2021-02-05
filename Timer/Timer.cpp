#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>

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

    void print_time() {
        std::cout << "Current time: " << ms.count() << " milliseconds\n";
    }
};

int main() {
    // Test timer
    std::vector<int> vec(1000000);
    Timer t;

    for (int i = 0; i < vec.size(); ++i) {
        vec[i] = vec.size() - i;
    }

    t.start_timer();

    std::sort(vec.begin(), vec.end());

    t.pause_timer();
    t.print_time();

    return 0;
}
