#include "Timer.h"
#include <future>
#include <thread>
#include <algorithm>
#include <numeric>
#include <execution>

template < typename Iterator, typename Function >
void parallel_for_each(Iterator first, Iterator last, Function func)
{
    const std::size_t length = std::distance(first, last);

    const std::size_t max_size = 10000000;

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


void test_for_each (Timer& t) {
    t.reset_timer();
    const int vec_size = 100000000;
    std::vector<int> vec(vec_size);

    std::iota(vec.begin(), vec.end(), 1);

    t.start_timer();
    std::for_each(vec.begin(), vec.end(), [](auto& x){x += 1;});
    t.print_time(std::cout);

    std::cout << "\n";

    t.reset_timer();
    t.start_timer();
    parallel_for_each(vec.begin(), vec.end(), [](auto& x){x += 1;});
    t.print_time(std::cout);

    // На таких данных наша версия for_each быстрее обычной:
    // for_each: 52 ms; parallel_for_each: 36 ms
    // Быстрота нашей реализации очевидно сильно зависит от константы max_size, если ее подобрать так,
    // чтобы использовалось не очень много потоков, то наша реализация будет быстрее
}


void test_partial_sums(Timer& t) {
    t.reset_timer();
    std::vector<int> res;

    const int vec_size = 100000000;
    std::vector<int> vec(vec_size);

    std::iota(vec.begin(), vec.end(), 1);

    t.start_timer();
    std::partial_sum(vec.begin(), vec.end(), std::back_inserter(res));
    t.print_time(std::cout);

    std::cout << std::endl;

    res.clear();
    t.reset_timer();
    t.start_timer();
    std::inclusive_scan(std::execution::par, vec.begin(), vec.end(), res.begin());
    t.print_time(std::cout);


    // На таких данных inclusive_scan быстрее, чем partial_sum:
    // partial_sum: 616 ms, inclusive_scan: 219 ms
    // Проверил при различных размерах исходного вектора, inclusive_scan в целом быстрее,
    // но при уменьшении размера начинают работать примерно одинаково

}

void test_transform(Timer& t) {
    t.reset_timer();

    const int vec_size = 200000000;
    std::vector<int> vec1(vec_size);
    std::vector<int> vec2(vec_size);

    std::iota(vec1.begin(), vec1.end(), 1);
    std::iota(vec2.rbegin(), vec2.rend(), 1);

    t.start_timer();
    std::cout << std::inner_product(vec1.begin(), vec1.end(), vec2.begin(), 1) << "\n";
    t.print_time(std::cout);

    std::cout << std::endl;

    t.reset_timer();
    t.start_timer();
    std::cout << std::transform_reduce(std::execution::par, vec1.begin(), vec1.end(), vec2.begin(), 1) << "\n";
    t.print_time(std::cout);

    // На таких данных transform_reduce быстрее, чем inner_product:
    // inner_product: 96 ms, transform_reduce: 56 ms
    // Аналогично предыдущему проверил при различных размерах исходных векторов и получил такие же результаты:
    // обычно transform_reduce быстрее, но при сильном уменьшении размера начинают работать примерно одинаково
}

int main() {
    Timer t;

    //test_for_each(t);
    //test_partial_sums(t);
    test_transform(t);

}