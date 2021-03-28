#include <algorithm>
#include <atomic>
#include <future>
#include <iostream>
#include <iterator>
#include <numeric>
#include <thread>
#include <vector>
#include <random>
#include <chrono>
#include <set>

class Threads_Guard
{
public:

    explicit Threads_Guard(std::vector < std::thread > & threads) :
            m_threads(threads)
    {}

    Threads_Guard(Threads_Guard const &) = delete;

    Threads_Guard& operator=(Threads_Guard const &) = delete;

    ~Threads_Guard() noexcept
    {
        try
        {
            for (std::size_t i = 0; i < m_threads.size(); ++i)
            {
                if (m_threads[i].joinable())
                {
                    m_threads[i].join();
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


struct Searcher
{
    void operator()(std::size_t start_index, std::size_t last_index, const std::string& data,
            const std::string& pattern, std::set< std::size_t > & result, std::mutex& mutex) noexcept {


        for (std::size_t index = data.find(pattern, start_index);
            start_index <= last_index && index != std::string::npos;
                start_index = index + pattern.length(), index = data.find(pattern, start_index)) {

            std::lock_guard lock(mutex);
            result.insert(index);

        }
    }
};

std::set<std::size_t> parallel_find(const std::string& data, const std::string& pattern) {

    std::set<std::size_t> result;

    if (data.empty())
        return result;

    const std::size_t min_per_thread = 25;
    const std::size_t max_threads =
            (data.length() + min_per_thread - 1) / min_per_thread;

    const std::size_t hardware_threads =
            std::thread::hardware_concurrency();

    const std::size_t num_threads =
            std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

    const std::size_t block_size = data.length() / num_threads;

    std::mutex mutex;

    std::vector < std::thread > threads(num_threads - 1);

    {
        Threads_Guard guard(threads);

        std::size_t block_start = 0;

        for (std::size_t i = 0; i < (num_threads - 1); ++i) {

            std::size_t block_end = block_start + block_size + pattern.length() - 1;

            threads[i] = std::thread(Searcher (), block_start, block_end, std::ref(data), std::ref(pattern),
                                     std::ref(result), std::ref(mutex));

            block_start += block_size;
        }

        Searcher()(block_start, data.length() - 1, std::ref(data), std::ref(pattern),
                std::ref(result), std::ref(mutex));

    }

    return result;
}

std::string generateDNA(std::size_t length) {
    std::string result;
    std::vector<std::string> letters = {"A", "G", "T", "C"};

    std::mt19937_64 mersenne (std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> int_distribution (0, 3);

    for (std::size_t i = 0; i < length; i++) {
        result += letters[int_distribution(mersenne)];
    }

    return result;
}

int main() {
    std::string data = generateDNA(50);

    std::string pattern;

    std::cout << data << "\n";
    std::cin >> pattern;

    auto result = parallel_find(data, pattern);

    for (auto i : result) std::cout << i << " ";
    return 0;
}
