#include "Timer.h"

#include <set>

int main() {
    int size = 20000;
    std::ofstream file ("times.txt");
    Timer t;

    std::vector<int> vec;
    std::set <int> set;

    for (int i = 0; i < 30; i++) {
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
            set.insert(size - i);
        }
        t.print_time(file);

        t.reset_timer();

        file << "\n";
        if (i >= 9) size += 200000;
        else size += 20000;
    }


    // Я пробовал на еще больших числах запускать, set всегда был быстрее вектора, значит можно сделать вывод,
    // что пороговое N, при котором меняется лидер либо велико, либо его нет
    // График в файле graph_task_1.png

}