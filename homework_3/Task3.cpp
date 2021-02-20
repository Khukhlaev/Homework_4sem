#include <iostream>
#include <functional>
#include <string>
#include <random>
#include <set>
#include <fstream>

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



int main() {
    std::vector<std::string> letters(26, "a");
    std::ofstream file ("collision.txt");

    // This is variables for 'struct'
    std::string name;
    int age;
    double height;
    bool is_drunk;
    char symbol;

    for (int i = 0; i < 26; i++) {
        letters[i] = 'a' + i;
    }

    std::set<int> codes;
    std::random_device rd;
    std::mt19937 mersenne(rd());

    int size = 300000;

    for (int j = 0; j < 40; ++j) {

        if (j % 5 == 4) std::cout << j+1 << "/40" << std::endl;

        codes.clear();
        for (int i = 0; i < size; ++i) {
            name.clear();
            height = mersenne() / double(mersenne());
            age = mersenne();
            is_drunk = mersenne();
            symbol = mersenne();
            for (int k = 0; k < 7; ++k) {
                name += letters[mersenne() % 26];
            }
            codes.insert(hash_value(name, height, age, is_drunk, symbol));
        }
        file << size << " " << size - codes.size() << std::endl;
        size += 100000;
    }


    // График в файле graph_task_3.png
}