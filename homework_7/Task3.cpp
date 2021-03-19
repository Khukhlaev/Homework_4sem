#include <algorithm>
#include <future>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

template < typename Iterator, typename Function >
void parallel_for_each(Iterator first, Iterator last, Function func)
{
    const std::size_t length = std::distance(first, last);

    const std::size_t max_size = 25;

    if (length <= max_size) {
        std::for_each(first, last, func);
    }
    else {
        Iterator middle = first;
        std::advance(middle, length / 2);

        std::future < void > first_half_result =
                std::async(parallel_for_each< Iterator, Function >, first, middle, func);

        parallel_for_each(middle, last, func);

        first_half_result.get();

    }
}

int main()
{
    std::vector < int > v(100);

    std::iota(v.begin(), v.end(), 1);

    parallel_for_each(v.begin(), v.end(), [](auto & x){x++;});

    for(auto i : v) std::cout << i << " ";

}