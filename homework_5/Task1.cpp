#include <iostream>
#include <iomanip>
#include <string>
#include <locale>
#include <Windows.h>

int main() {

    const double euro_to_rub = 88.94;

    SetConsoleCP      (CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    std::cout.imbue(std::locale("ru_RU.utf8"));
    std::cin.imbue(std::locale("deu_DEU.utf8"));

    std::string money;
    double euros;

    std::cin >> std::get_money(money);
    euros = std::stod(money);
    money = std::to_string(euros * euro_to_rub);
    std::cout << std::showbase << std::put_money(money, false);

    return 0;
}