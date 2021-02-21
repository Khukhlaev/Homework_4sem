#include "Timer.h"

#include <set>

int main() {
    int size = 100000;
    std::ofstream file ("times.txt");
    Timer t;

    std::vector<int> vec;
    std::set <int> set;

    for (int i = 0; i < 35; i++) {
        vec.clear();
        set.clear();

        file << size << " ";

        vec.resize(size);
        for (int j = 0; j < size; ++j) {
            vec[j] = size - j;
        }

        t.start_timer();
        std::sort(vec.begin(), vec.end());
        t.print_time(file);

        t.reset_timer();

        t.start_timer();
        for (int k = 0; k < size; ++k) {
            set.insert(size - k);
        }
        t.print_time(file);

        t.reset_timer();

        file << "\n";
        if (i >= 4) size += 200000;
        else size += 20000;
    }


    // Vector всегда быстрее, чем set
    // График в файле graph_task_1.png

}