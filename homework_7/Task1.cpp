#include <iostream>
#include <thread>
#include <future>
#include <random>
#include <chrono>
#include <cmath>

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



struct Point {
    double x;
    double y;

    Point (double x, double y) {
        this->x = x;
        this->y = y;
    }

    double distance_to_zero() const {
        return sqrt(x * x + y * y);
    }
};

Point generate_point(std::mt19937_64& mersenne, std::uniform_real_distribution<double>& real_dist) {
    return Point(real_dist(mersenne), real_dist(mersenne));
}

double counting_pi (int number_points = 1000000) {

    std::mt19937_64 mersenne (std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> real_dist(-0.5, 0.5);

    double number_points_in_circle = 0;

    for (int i = 0; i < number_points; ++i) {
       if (generate_point(mersenne, real_dist).distance_to_zero() < 0.5) number_points_in_circle++;
    }

    return 4 * number_points_in_circle / number_points;
}


// service function for parallel_counting_pi
double count_points(std::mt19937_64& mersenne, std::uniform_real_distribution<double>& real_dist, int number_points) {

    double number_points_in_circle = 0;

    for (int i = 0; i < number_points; ++i) {
        if (generate_point(mersenne, real_dist).distance_to_zero() < 0.5) number_points_in_circle++;
    }

    return number_points_in_circle;
}



double parallel_counting_pi (int number_points = 8000000) {

    const int min_per_thread = 100000;
    const int max_threads = number_points / min_per_thread;

    const int hardware_threads = std::thread::hardware_concurrency();

    const int num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

    const int block_size = number_points / num_threads;

    std::vector < std::future < double > > futures(num_threads - 1);
    std::vector < std::thread >		  threads(num_threads - 1);

    Threads_Guard guard(threads);

    std::mt19937_64 mersenne (std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> real_dist(-0.5, 0.5);

    for (std::size_t i = 0; i < (num_threads - 1); ++i)
    {

        std::packaged_task < double(std::mt19937_64&, std::uniform_real_distribution<double>&, int) > task{
            count_points };

        futures[i] = task.get_future();
        threads[i] = std::thread(std::move(task), std::ref(mersenne), std::ref(real_dist), block_size);

    }

    double last_result = count_points(mersenne, real_dist, block_size);

    double result = 0;

    for (std::size_t i = 0; i < (num_threads - 1); ++i)
    {
        result += futures[i].get();
    }

    result += last_result;

    return 4 * result / number_points;

}

int main () {
    std::cout << counting_pi() << std::endl << parallel_counting_pi();

    // Результат:
    // 3.14465
    // 3.14116

    return 0;
}