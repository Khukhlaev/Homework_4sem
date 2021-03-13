#include <iostream>
#include <fstream>
#include <filesystem>

enum states {
    no_comments = 0,
    line_comment = 1,
    multiline_comment = 2
};

int main() {
    std::filesystem::path path = "../test.txt", new_path = "../test_no_comment.txt";

    std::fstream file_in(path.string(), std::ios::in);
    std::string text((std::istreambuf_iterator<char> (file_in)), std::istreambuf_iterator<char> ());

    states state = no_comments;
    bool is_string = false, need_add = true;
    std::string result;

    for (int i = 0; i < text.length() - 1; ++i) {
        need_add = true;

        switch (text[i]) {
            case '/':
                if ((text[i + 1] != '/' && text[i + 1] != '*') || state != no_comments || is_string) break;

                if (text[i + 1] == '/') {
                    state = line_comment;
                }
                else { // if next symbol is '*'
                    state = multiline_comment;
                }

                i++; // To not analyze next '/' or '*'

                break;

            case '\n':

                if (is_string) break;

                if (state == line_comment) state = no_comments; // '/n' is end of line comment

                break;

            case '*':
                if (text[i + 1] != '/' || state != multiline_comment || is_string) break;

                i++; // To not analyze next '/'

                need_add = false; // To not accidentally add next '/' to the result

                state = no_comments; // Here we have only situation "*/" as end of multiline comment
                break;

            case '"':
                if (state != no_comments) break;

                if(!is_string) is_string = true;
                else if (text[i - 1] != '\\') is_string = false;

                break;

        }

        if (state == no_comments && need_add) result += text[i];
    }

    std::fstream file_out (new_path.string(), std::ios::out | std::ios::trunc);
    file_out << result;

    return 0;
}