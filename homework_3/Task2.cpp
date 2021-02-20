#include <iostream>
#include <cmath>
#include <random>
#include <set>

size_t double_hash (double source) {
    // Идея - взять двоичную запись double и перевести ее в size_t

    union Double {
        double db;
        size_t integer;
    };

    Double d{};
    d.db = source;

    // С детерменированностью и скоростью вычисления все ясно
    // C равномерностью - в целом вроде тоже понятно, что будет достаточно равномерно, но все равно проведем тесты,
    // возьмем большие размеры и посмотрим на число коллизий

    return d.integer;
}

int main () {
    std::random_device rd;
    std::mt19937_64 mersenne(rd());

    std::set<size_t> keys;

    union Double {
        double db;
        int64_t integer;
    };

    Double buffer{};

    int size = 100000;

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < size; ++j) {
            buffer.integer = mersenne();
            keys.insert(double_hash(buffer.db) % size);
        }
        std::cout << "Size: " << size << ", number of collisions: " << size - keys.size() << "\n";
        size += 100000;
    }

    // Результаты:
//    Size: 100000, number of collisions: 36774
//    Size: 200000, number of collisions: 50463
//    Size: 300000, number of collisions: 55443
//    Size: 400000, number of collisions: 57132
//    Size: 500000, number of collisions: 57875
//    Size: 600000, number of collisions: 58154
//    Size: 700000, number of collisions: 58364
//    Size: 800000, number of collisions: 57628
//    Size: 900000, number of collisions: 58054
//    Size: 1000000, number of collisions: 58157
//    Кажется, что достаточно равномерно

}

