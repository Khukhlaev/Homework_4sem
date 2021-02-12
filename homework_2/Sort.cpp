#include "Timer.h"
#include <deque>
#include <list>
#include <forward_list>
#include <array>
#include <fstream>

const size_t SIZE = 500000;

int main () {
    Timer timer;
    std::ofstream file ("times.txt");

    std::vector<int> vec (SIZE, 0);
    std::array<int, SIZE> array{};
    std::list<int> list;
    std::forward_list<int> forwardList;
    // Собственный sort есть у list и forward_list, при этом у них не работает std::sort из библиотеки algorithm
    std::deque<int> deque;

    for (int i = 0; i < SIZE; ++i) {
        vec[i] = SIZE - i;
        array[i] = SIZE - i;
        list.push_back(SIZE - i);
        forwardList.push_front(i + 1);
        deque.push_back(SIZE - i);
    }

    file << "Vector: ";
    timer.start_timer();
    std::sort(vec.begin(), vec.end());
    timer.print_time(file);

    file << "Array: ";
    timer.reset_timer();
    timer.start_timer();
    std::sort(array.begin(), array.end());
    timer.print_time(file);

    file << "List, built-in sort: ";
    timer.reset_timer();
    timer.start_timer();
    list.sort();
    timer.print_time(file);

    file << "Forward list, built-in sort: ";
    timer.reset_timer();
    timer.start_timer();
    forwardList.sort();
    timer.print_time(file);

    file << "Deque: ";
    timer.reset_timer();
    timer.start_timer();
    std::sort(deque.begin(), deque.end());
    timer.print_time(file);


    // Результаты:
    // 1. Array:  time = 31 milliseconds
    // 2. Vector:  time = 78 milliseconds
    // 3. Deque:  time = 145 milliseconds
    // 4. List, built-in sort:  time = 233 milliseconds
    // 5. Forward list, built-in sort:  time = 241 milliseconds
    // Фактически, forward list и list неотличимы, иногда быстрее один, иногда другой; остальные отличимы.
    return 0;
}
