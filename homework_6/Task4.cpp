#include "json.hpp"
#include <vector>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <iomanip>

using nlohmann::json;

struct Person {
    std::string name;
    std::string surname;
    int age;
    double height;
    std::string nationality;
};

std::istream& operator>> (std::istream &in, Person &person) {
    in >> person.name >> person.surname >> person.age >> person.height >> person.nationality;
    return in;
}

int main () {
    const int number_persons = 3;

    std::vector<Person> persons(number_persons);

    std::vector<json> jsons(number_persons);

    for (auto & person : persons) {
        std::cin >> person;
    }

    for (int i = 0; i < number_persons; ++i) {
        jsons[i]["name"] = persons[i].name;
        jsons[i]["surname"] = persons[i].surname;
        jsons[i]["age"] = persons[i].age;
        jsons[i]["height"] = persons[i].height;
        jsons[i]["nationality"] = persons[i].nationality;
    }

    auto path = std::filesystem::current_path();
    std::filesystem::create_directory(path / "../jsons");

    std::vector<std::fstream> files (number_persons);

    for (int i = 0; i < number_persons; i++) {
        files[i].open("../jsons/json" + std::to_string(i) + ".txt", std::ios::trunc | std::ios::out);
        files[i] << std::setw(4) << jsons[i];
    }

    return 0;
}