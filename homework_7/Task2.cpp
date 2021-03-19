#include <algorithm>
#include <future>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>
#include <fstream>
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

template < typename Iterator, typename T >
struct accumulate_block
{
    T operator()(Iterator first, Iterator last)
    {
        return std::accumulate(first, last, T());
    }
};

template < typename Iterator, typename T >
T parallel_accumulate(Iterator first, Iterator last, T init, std::size_t num_threads = 4)
{
    const std::size_t length = std::distance(first, last);

    if (!length)
        return init;

    const std::size_t block_size = length / num_threads;

    std::vector < std::future < T > > futures(num_threads - 1);
    std::vector < std::thread >		  threads(num_threads - 1);

    Threads_Guard guard(threads);

    Iterator block_start = first;

    for (std::size_t i = 0; i < (num_threads - 1); ++i)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);

        std::packaged_task < T(Iterator, Iterator) > task{
                accumulate_block < Iterator, T >() };

        futures[i] = task.get_future();
        threads[i] = std::thread(std::move(task), block_start, block_end);

        block_start = block_end;
    }

    T last_result = accumulate_block < Iterator, T > ()(block_start, last);

    T result = init;

    for (std::size_t i = 0; i < (num_threads - 1); ++i)
    {
        result += futures[i].get();
    }

    result += last_result;

    return result;
}

int main() {
    const int vec_size = 400000000;
    const int size_vec_number_threads = 25;

    std::vector < int > v(vec_size);

    std::iota(v.begin(), v.end(), 1);

    std::vector<std::size_t> number_threads(size_vec_number_threads);
    std::iota(number_threads.begin(), number_threads.end(), 1);

    std::fstream file ("task2.txt", std::ios::trunc | std::ios::out);

    Timer t;

    for (auto n_threads : number_threads) {
        file << n_threads << " ";
        t.reset_timer();
        t.start_timer();

        parallel_accumulate (v.begin(), v.end(), 0, n_threads);
        t.print_time(file);

        file << std::endl;
    }


    // График в файле graph_task_2.png. При N около 8 время испольнения выходит практически на константу
    // и уже незначительно флуктуирует около значение при таком N. В моем процессоре как раз 8 потоков,
    // поэтому достижение минимального времени при таком значении N вполне понятно.
    // Видимо при дальнейшем увеличении N время меняется не сильно из-за хорошей оптимизации.
    // Я попробовал также сделать number_threads.push_back(500); и при таком большом количестве потоков (500 штук)
    // время исполнения уже заметно увеличилось (до 68 мс).
    return 0;

}
