#include <iostream>
#include <vector>

void print_vec_size_capacity (const std::vector<int>& v) {
    std::cout << "Size: " << v.size() << "; capacity: " << v.capacity() << std::endl;
}

int main() {
    std::vector<int> v1;
    print_vec_size_capacity(v1);
    v1.push_back(1);
    print_vec_size_capacity(v1);
    v1.push_back(2);
    print_vec_size_capacity(v1);
    v1.push_back(3);
    print_vec_size_capacity(v1);
    v1.push_back(4);
    v1.push_back(5);
    print_vec_size_capacity(v1); // Емкость вектора при нехватке места для новых элементов увеличивается в 2 раза

    std::cout << "\n============================\n";

    std::vector<int> v2(9, 9);
    print_vec_size_capacity(v2);
    v2.reserve(10);
    print_vec_size_capacity(v2);
    v2.reserve(5);
    print_vec_size_capacity(v2);
    v2.push_back(1);
    v2.push_back(2);
    print_vec_size_capacity(v2); // Если число n в reserve меньше текущей емкости, то ничего не происходит,
    // если больше - то новый capacity = n.
    // Если новый элемент уже не влезает, то опять же емкость увеличивается в 2 раза

    std::cout << "\n============================\n";

   /* int64_t max_size = v1.max_size();
    std::vector<int> v3 (max_size, 0);
    print_vec_size_capacity(v3); */ // В таком случае программа выбрасывает исключение std::bad_alloc

    int64_t max_size = v1.max_size();
    int64_t i = 1000000000;
    std::cout << max_size << "\n";
    std::vector<int> v3 (max_size/i, 0); // В таком случае эта строчка программы выполняется достаточно долго
    // (порядка 10 секунд); если i уменьшить в 10 раз, то уже выбросится исключение std::bad_alloc
    print_vec_size_capacity(v3);

    v3.push_back(1); // При попытке добавить новый элемент выбрасывается std::bad_alloc
    print_vec_size_capacity(v3); // Если i увеличить в 2 раза, то push_back будет выполняться достаточно долго,
    // (порядка 10-20 секунд) но выполнится корректно и исключение не выбросится

    return 0;
}