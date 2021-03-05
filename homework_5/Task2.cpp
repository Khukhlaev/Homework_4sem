#include <iostream>
#include <locale>
#include <codecvt>
#include <unordered_map>
#include <fstream>
#include <string>

#include <boost/locale.hpp>

#include <Windows.h>

std::string convert_locale_to_utf(const std::string & string)
{
    boost::locale::generator generator;
    generator.locale_cache_enabled(true);

    std::locale locale =
            generator(boost::locale::util::get_system_locale());

    return boost::locale::conv::to_utf < char >(string, locale);
}

std::string convert_utf_to_locale(const std::string & string)
{
    boost::locale::generator generator;
    generator.locale_cache_enabled(true);

    std::locale locale =
            generator(boost::locale::util::get_system_locale());

    return boost::locale::conv::from_utf < char >(string, locale);
}


std::u32string convert_utf8_to_u32string(const std::string & string)
{
    return boost::locale::conv::utf_to_utf < char32_t, char > (string);
}

std::string convert_u32string_to_utf8(const std::u32string & u32string)
{
    return boost::locale::conv::utf_to_utf < char, char32_t > (u32string);
}

int main() {
    system("chcp 1251");

    std::unordered_map<std::u32string, std::u32string> transliteration;
    std::ifstream tr_table("tr_table.txt");
    tr_table.imbue(std::locale("rus_rus.1251"));
    std::stringstream buffer;
    std::string buf;
    std::vector<std::u32string> vec;

    while (std::getline(tr_table, buf)) {
        vec.clear();
        buffer.clear();
        buffer << buf;
        while(buffer >> buf) {
            vec.push_back(convert_utf8_to_u32string(convert_locale_to_utf(buf)));
        }

        if (vec.size() == 2) {
            transliteration.emplace(vec[0], vec[1]);
        }
        else {
            transliteration.emplace(vec[0], vec[2]);
            transliteration.emplace(vec[1], vec[3]);
        }
    }

    std::string text;
    std::cin >> text;

    std::string u8text = convert_locale_to_utf(text);
    std::u32string u32text = convert_utf8_to_u32string(u8text);
    std::u32string u32result;

    for (char32_t i : u32text) {
        u32result += transliteration[std::u32string(1,i)];
    }

    std::cout << convert_utf_to_locale(convert_u32string_to_utf8(u32result)) << "\n";


    return 0;
}