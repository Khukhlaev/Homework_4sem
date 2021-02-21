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
    std::uniform_real_distribution<> dis(-1.8e180, 1.8e180);

    std::set<size_t> keys;

    int size = 100000;

    for (int i = 0; i < 10; ++i) {
        keys.clear();

        for (int j = 0; j < size; ++j) {
            keys.insert(double_hash(dis(mersenne)) % size);
        }

        std::cout << "Size: " << size << ", number of collisions: " << size - keys.size() << "\n";
        size += 100000;
    }

    // Результаты:
//    Size: 100000, number of collisions: 52317
//    Size: 200000, number of collisions: 104574
//    Size: 300000, number of collisions: 156759
//    Size: 400000, number of collisions: 209496
//    Size: 500000, number of collisions: 261841
//    Size: 600000, number of collisions: 314474
//    Size: 700000, number of collisions: 367219
//    Size: 800000, number of collisions: 419488
//    Size: 900000, number of collisions: 471813
//    Size: 1000000, number of collisions: 523887
//    Кажется, что достаточно равномерно

}

