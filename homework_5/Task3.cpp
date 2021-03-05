#include <iostream>
#include <regex>
#include <fstream>
#include <vector>


int main() {
    std::regex pattern(R"(([\n[:space:]]+\w+[-\w\.]*)@(\w+[\.\w]*)\.([[:alpha:]]{2,6}))");
    std::smatch matches;

    std::ifstream input("text_task3.txt");

    std::string buffer;

    while (std::getline(input, buffer)) {
        std::copy(
                std::sregex_token_iterator(buffer.begin(), buffer.end(), pattern, {2}),
                std::sregex_token_iterator(),
                std::ostream_iterator < std::string >(std::cout, "\n"));
    }

    // Результат:
//    yandex
//    phystech
//    cia
//    vor
//    ru_gov
//    gmail

    return 0;
}