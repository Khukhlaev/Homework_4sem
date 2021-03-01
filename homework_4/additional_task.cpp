#include <iostream>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <random>
#include <vector>
#include <iterator>

int main () {
    std::vector<int> vec(10000);
    std::iota(std::begin(vec), std::end(vec), 1);
    std::default_random_engine dre (std::chrono::system_clock::now().time_since_epoch().count());
    std::shuffle(std::begin(vec), std::end(vec), dre);
    std::vector<int> vec_copy = vec;

    int counter_sort = 0, counter_nth = 0;

    std::sort(std::begin(vec), std::end(vec), [&counter_sort](auto x, auto y)
        {counter_sort++; return x < y;});

    std::nth_element(std::begin(vec_copy), std::next(std::begin(vec_copy), std::size(vec_copy) / 2),
            std::end(vec_copy), [&counter_nth](auto x, auto y)
            {counter_nth++; return x < y;});

    std::cout << "Sort: " << counter_sort << ", nth: " << counter_nth << "\n";

    // Результат: Sort: 162315, nth: 33932

    return 0;
}