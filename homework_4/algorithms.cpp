#include <iostream>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <random>
#include <vector>
#include <iterator>
#include <cmath>

int main () {
    // 1.
    std::vector<int> p1(10);
    std::iota(std::begin(p1), std::end(p1), 1);

    // 2.
    std::copy( std::istream_iterator < int > (std::cin),
            std::istream_iterator < int > (),
            std::back_inserter(p1));

    // 3.
    std::default_random_engine dre (std::chrono::system_clock::now().time_since_epoch().count());
    std::shuffle(std::begin(p1), std::end(p1), dre);

    // 4.
    std::sort(p1.begin(), p1.end());
    p1.erase(std::unique(std::begin(p1), std::end(p1)), std::end(p1));

    // 5.
    int counter_odd = std::count_if(std::begin(p1), std::end(p1), [](auto x){return x % 2;});

    // 6.
    auto minmax_pair = std::minmax_element(std::begin(p1), std::end(p1));
    int min_element = *minmax_pair.first;
    int max_element = *minmax_pair.second;

    // 7.
    auto it_prime_number = std::find_if(std::begin(p1), std::end(p1),
            [](auto x){
                for (int i = 2; i <= sqrt(x); ++i) {
                    if (x % i == 0) return false;
                }
                return true;
            });

    // 8.
    std::transform(std::begin(p1), std::end(p1), std::begin(p1), [](auto x){return x*x;});

    // 9.
    std::vector<int> p2(std::size(p1));
    std::uniform_int_distribution<> uid (0, 250);
    std::generate(std::begin(p2), std::end(p2), [&](){return uid(dre);});

    // 10.
    int sum_p2 = std::accumulate(std::begin(p2), std::end(p2), 0);

    // 11.
    std::fill(std::begin(p2), std::begin(p2) + 3, 1);

    // 12.
    std::vector<int> p3(std::size(p2));
    std::transform(std::begin(p1), std::end(p1), std::begin(p2), std::begin(p3),
            [](auto x, auto y){return y - x;});

    // 13.
    std::replace_if(std::begin(p3), std::end(p3), [](auto x){return x < 0;}, 0);

    // 14.
    p3.erase(std::remove_if(std::begin(p3), std::end(p3),
            [](auto x){return x == 0;}), std::end(p3));

    // 15.
    std::reverse(std::begin(p3), std::end(p3));

    // 16.
    std::nth_element(std::begin(p3), std::end(p3) - 3,std::end(p3));
    std::cout << "Top 3 max elements from p3: "
        << p3[std::size(p3) - 1] << " " << p3[std::size(p3) - 2] << " " << p3[std::size(p3) - 3] << std::endl;

    // 17.
    std::sort(std::begin(p1), std::end(p1));
    std::sort(std::begin(p2), std::end(p2));

    // 18.
    std::vector<int> p4;
    std::merge(std::begin(p1), std::end(p1), std::begin(p2), std::end(p2), std::back_inserter(p4));

    // 19.
    auto result_equal_range = std::equal_range(std::begin(p4), std::end(p4), 1);

    // 20.
    std::copy(std::cbegin(p1), std::cend(p1),std::ostream_iterator < int > (std::cout, " "));
    std::cout << std::endl;
    std::copy(std::cbegin(p2), std::cend(p2),std::ostream_iterator < int > (std::cout, " "));
    std::cout << std::endl;
    std::copy(std::cbegin(p3), std::cend(p3),std::ostream_iterator < int > (std::cout, " "));
    std::cout << std::endl;
    std::copy(std::cbegin(p4), std::cend(p4),std::ostream_iterator < int > (std::cout, " "));

    return 0;
}

