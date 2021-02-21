#include <iostream>
#include <functional>
#include <string>
#include <random>
#include <set>
#include <fstream>
#include <chrono>

template < typename T >
void hash_combine(std::size_t & seed, const T & value) noexcept
{
    seed ^= std::hash < T > ()(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template < typename T >
void hash_value(std::size_t & seed, const T & value) noexcept
{
    hash_combine(seed, value);
}

template < typename T, typename ... Types >
void hash_value(std::size_t & seed, const T & value, const Types & ... args) noexcept
{
    hash_combine(seed, value);
    hash_value(seed, args...);
}

template < typename ... Types >
std::size_t hash_value(const Types & ... args) noexcept
{
    std::size_t seed = 0;
    hash_value(seed, args...);
    return seed;
}

std::set <std::string> makeRandomWords (size_t length) {
    std::uniform_int_distribution letter(97, 122);
    std::default_random_engine e(static_cast <std::size_t>
        (std::chrono::system_clock::now().time_since_epoch().count()));

    std::set <std::string> words;

    for (std::string str(length, '_'); words.size() < 6000000u; words.insert(str)) {
        for (auto &c : str)
            c = letter(e);
    }

    return words;
}


int main() {
    std::vector<std::string> letters(26, "a");
    std::ofstream file ("collision.txt");

    // This is variables for 'struct'
    std::string name;
    int some_int;
    double some_double;
    bool is_drunk;
    char symbol;

    std::set<std::string> words = makeRandomWords(10);

    std::set<int> codes;

    std::random_device rd;
    std::mt19937 mersenne(rd());
    std::uniform_int_distribution<> distrib_int(-INT32_MAX, INT32_MAX);
    std::uniform_real_distribution<> distrib_double(-1.8e180, 1.8e180);
    std::bernoulli_distribution distrib_bool(0.5);
    std::uniform_int_distribution<> distrib_char(0, 65535);

    int size = 300000;

    for (int j = 0; j < 45; ++j) {

        if (j % 5 == 4) std::cout << j+1 << "/40" << std::endl;

        codes.clear();
        auto it = words.begin();
        for (int i = 0; i < size; ++i) {

            name = *it;
            some_double = distrib_double(mersenne);
            some_int = distrib_int(mersenne);
            is_drunk = distrib_bool(mersenne);
            symbol = distrib_char(mersenne);
            codes.insert(hash_value(name, some_int, some_double, is_drunk, symbol));

            it++;
        }
        file << size << " " << size - codes.size() << std::endl;
        size += 100000;
    }


    // График в файле graph_task_3.png
}