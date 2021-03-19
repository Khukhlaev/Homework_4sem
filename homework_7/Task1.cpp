#include <iostream>
#include <thread>
#include <future>
#include <random>
#include <chrono>
#include <cmath>
#include "Timer.h"

class Threads_Guard
{
public:

    explicit Threads_Guard(std::vector < std::thread > & threads) :
            m_threads(threads)
    {}

    Threads_Guard			(Threads_Guard const&) = delete;

    Threads_Guard& operator=(Threads_Guard const&) = delete;

    ~Threads_Guard() noexcept
    {
        try
        {
            for (auto & m_thread : m_threads)
            {
                if (m_thread.joinable())
                {
                    m_thread.join();
                }
            }
        }
        catch (...)
        {
            // std::abort();
        }
    }

private:

    std::vector < std::thread > & m_threads;
};

double counting_pi (int number_points = 1000000) {

    std::mt19937_64 mersenne (std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> real_dist(-0.5, 0.5);

    int number_points_in_circle = 0;

    double x, y;
    for (int i = 0; i < number_points; ++i) {
        x = real_dist(mersenne);
        y = real_dist(mersenne);
       if (x * x + y * y < 0.25) number_points_in_circle++;
    }

    return 4.0 * number_points_in_circle / number_points;
}


// service function for parallel_counting_pi
int count_points(int number_points) {

    std::mt19937_64 mersenne (std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> real_dist(-0.5, 0.5);

    int number_points_in_circle = 0;
    double x, y;

    for (int i = 0; i < number_points; ++i) {
        x = real_dist(mersenne);
        y = real_dist(mersenne);
        if (x * x + y * y < 0.25) number_points_in_circle++;
    }

    return number_points_in_circle;
}



double parallel_counting_pi (int number_points = 8000000) {

    const int min_per_thread = 100000;
    const int max_threads = number_points / min_per_thread;

    const int hardware_threads = std::thread::hardware_concurrency();

    const int num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

    const int block_size = number_points / num_threads;

    std::vector < std::future < int > > futures(num_threads - 1);
    std::vector < std::thread >		  threads(num_threads - 1);

    Threads_Guard guard(threads);

    for (std::size_t i = 0; i < (num_threads - 1); ++i)
    {

        std::packaged_task < int(int) > task{ count_points };

        futures[i] = task.get_future();
        threads[i] = std::thread(std::move(task), block_size);

    }

    int last_result = count_points(block_size);

    int result = 0;

    for (std::size_t i = 0; i < (num_threads - 1); ++i)
    {
        result += futures[i].get();
    }

    result += last_result;

    return 4.0 * result / number_points;

}

int main () {
    Timer t;

    t.start_timer();
    std::cout << counting_pi() << " ";
    t.print_time(std::cout);
    std::cout << std::endl;

    t.reset_timer();
    t.start_timer();
    std::cout << parallel_counting_pi() << " ";
    t.print_time(std::cout);

    // Результат:
//    3.13846 365
//    3.14095 403 - паралелльной способ, хотя в нем в 8 раз больше точек и выше соответсвенно выше точность

    return 0;
}