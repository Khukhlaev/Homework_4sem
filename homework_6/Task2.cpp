#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <string>

void fill_file(const std::string& name) {
    std::fstream file (name, std::ios::trunc | std::ios::out);

    for (int i = 0; i < 10; ++i) {
        file << "hello" + std::to_string(i) << std::endl;
    }
}


int main() {
    std::string file_name = "task2.txt";
    fill_file(file_name);

    std::fstream file (file_name, std::ios::in);
    std::string line;
    std::getline(file, line);

    const unsigned int line_length = line.length() + 2;

    unsigned int number_line;
    std::cin >> number_line;

    file.seekg(number_line * line_length);
    std::getline(file, line);

    std::cout << line << std::endl;

}