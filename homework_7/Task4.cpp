#include "Timer.h"
#include <future>
#include <thread>
#include <algorithm>
#include <numeric>
#include <execution>


void test_for_each (Timer& t) {
    t.reset_timer();
    const int vec_size = 1000000;
    std::vector<int> vec(vec_size);

    std::iota(vec.begin(), vec.end(), 1);

    t.start_timer();
    std::for_each(vec.begin(), vec.end(), [](auto& x){x += 1;});
    t.print_time(std::cout);

    std::cout << "\n";

    t.reset_timer();
    t.start_timer();
    std::for_each(std::execution::par, vec.begin(), vec.end(), [](auto& x){x += 1;});
    t.print_time(std::cout);

    // На таких данных параллельный for_each быстрее обычного:
    // обычный: 53 ms; параллельный: 37 ms
    // При других размерах результаты абсолютно аналогичны другим алгоритмам: параллельная версия в целом быстрее,
    // но при уменьшении размера начинают работать примерно одинаково
}


void test_partial_sums(Timer& t) {
    t.reset_timer();

    const int vec_size = 20000000;
    std::vector<double> vec(vec_size, 1.0);
    std::vector<double> res(vec_size);

    auto bin_op = [](const auto lhs, const auto rhs) {
        double result = 0;

        for (int i = 0; i < 10; ++i) {
            result = std::sin(result + lhs + rhs);
        }
        return result;
    };

    t.start_timer();
    std::partial_sum(vec.begin(), vec.end(), res.begin(), bin_op);
    t.print_time(std::cout);

    std::cout << std::endl;

    res.clear();
    res.resize(vec_size);

    t.reset_timer();
    t.start_timer();
    std::inclusive_scan(std::execution::par, vec.begin(), vec.end(), res.begin(), bin_op);
    t.print_time(std::cout);


    // На таких данных inclusive_scan сильно быстрее, чем partial_sum:
    // partial_sum: 5067 ms, inclusive_scan: 1501 ms
    // На простых же операциях partial_sum быстрее

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
    test_partial_sums(t);
    //test_transform(t);

}