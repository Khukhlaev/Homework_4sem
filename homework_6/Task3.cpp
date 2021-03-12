#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <ctime>

using namespace std::chrono_literals;
auto compute_file_size(const std::filesystem::path & path)
{
    if (std::filesystem::exists(path) && std::filesystem::is_regular_file(path))
    {
        auto file_size = std::filesystem::file_size(path);

        if (file_size != static_cast <uintmax_t> (-1))
        {
            return file_size;
        }
    }

    return static_cast <uintmax_t> (-1);
}

auto compute_directory_size(const std::filesystem::path & path)
{
    uintmax_t size = 0ULL;

    if (std::filesystem::exists(path) && (std::filesystem::is_regular_file(path) || std::filesystem::is_symlink(path)))
        return compute_file_size(path);

    if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
    {
        for (auto const & entry : std::filesystem::recursive_directory_iterator(path))
        {
            if (std::filesystem::is_regular_file(entry.status()) || std::filesystem::is_symlink(entry.status()))
            {
                auto file_size = std::filesystem::file_size(entry);

                if (file_size != static_cast <uintmax_t> (-1))
                {
                    size += file_size;
                }
            }
        }
    }

    return size;
}

void view_directory(const std::filesystem::path & path)
{
    if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
    {
        for (const auto & entry : std::filesystem::directory_iterator(path))
        {
            std::cout << entry.path().filename() << " " << compute_directory_size(entry);

            if (std::filesystem::is_regular_file(entry.status())) {

                auto ftime = std::filesystem::last_write_time(entry);
                std::cout << " " << ftime.time_since_epoch().count();
            //    auto time = ftime.time_since_epoch();
             //   std::time_t t = (time_t)time.count();
           //     std::cout << std::asctime(std::localtime(&t));
            }
            std::cout << std::endl;
        }
    }
}

int main () {
    std::string user_path;

    std::cin >> user_path;
    view_directory(user_path);

    return 0;
}
