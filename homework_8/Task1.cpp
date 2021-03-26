#include <iostream>
#include <thread>
#include <future>
#include <random>
#include <chrono>
#include <cmath>
#include <atomic>

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

// service function for parallel_counting_pi
void count_points(int64_t number_points, std::atomic<int64_t>& number_points_in_circle) {

    std::mt19937_64 mersenne (std::hash<std::thread::id> {}(std::this_thread::get_id()));
    std::uniform_real_distribution<double> real_dist(-0.5, 0.5);

    double x, y;

    for (int64_t i = 0; i < number_points; ++i) {
        x = real_dist(mersenne);
        y = real_dist(mersenne);
        if (x * x + y * y < 0.25) number_points_in_circle++;
    }

}



double parallel_counting_pi (int64_t number_points = 8000000) {

    const int64_t min_per_thread = 100000;
    const int64_t max_threads = number_points / min_per_thread;

    const int64_t hardware_threads = std::thread::hardware_concurrency();

    const int64_t num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

    const int64_t block_size = number_points / num_threads;

    std::atomic<int64_t> number_points_int_circle(0);

    std::vector < std::future < void > > futures(num_threads - 1);
    std::vector < std::thread >		  threads(num_threads - 1);

    Threads_Guard guard(threads);

    for (int64_t i = 0; i < (num_threads - 1); ++i)
    {

        std::packaged_task < void(int64_t, std::atomic<int64_t>&) > task{ count_points };

        futures[i] = task.get_future();
        threads[i] = std::thread(std::move(task), block_size, std::ref(number_points_int_circle));

    }

    count_points(block_size, std::ref(number_points_int_circle));

    for (int64_t i = 0; i < (num_threads - 1); ++i)
    {
        futures[i].get();
    }

    return 4.0 * number_points_int_circle / number_points;

}

int main () {

    std::cout << parallel_counting_pi(30000000);
    // Результат:
//    3.14164

    return 0;
}