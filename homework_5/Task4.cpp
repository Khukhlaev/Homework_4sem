#include <iostream>
#include <regex>
#include <fstream>
#include <vector>


int main() {
    std::regex pattern1(R"((0[1-9]|[12][0-9]|3[01])[- :/\.](0[1-9]|1[012])[- :/\.]([012]\d{3}))"); // DD:MM:YYYY
    std::regex pattern2(R"(([01]\d|2[0-3])(:[0-5]\d){2})");
    std::smatch matches;

    std::ifstream input("text_task4.txt");

    std::string buffer;
    std::vector<std::string> times, dates;
    while (std::getline(input, buffer)) {
        std::copy(
                std::sregex_token_iterator(buffer.begin(), buffer.end(), pattern1, {0 }),
                std::sregex_token_iterator(),
                std::back_inserter(dates));
        std::copy(
                std::sregex_token_iterator(buffer.begin(), buffer.end(), pattern2, {0 }),
                std::sregex_token_iterator(),
                std::back_inserter(times));
    }

    std::cout << "Times: ";
    std::copy(std::begin(times), std::end(times),
            std::ostream_iterator < std::string >(std::cout, " "));
    std::cout << "\nDates: ";
    std::copy(std::begin(dates), std::end(dates),
              std::ostream_iterator < std::string >(std::cout, " "));

    // Результаты:
//    Times: 11:11:11 23:11:11 12:24:48
//    Dates: 21.12.2001 22.06.1941
    return 0;
}
