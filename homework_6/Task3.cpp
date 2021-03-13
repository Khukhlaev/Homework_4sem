#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <ctime>

template <typename TP>
std::time_t to_time_t(TP tp)
{
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
                                                        + system_clock::now());
    return system_clock::to_time_t(sctp);
}


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

    std::cout << std::left << std::setw(30) << "File (or folder) name"
        << std::left << std::setw(30) << "Size in bytes" << std::left << std::setw(30) << "Date modified" << std::endl;
    if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
    {
        for (const auto & entry : std::filesystem::directory_iterator(path))
        {
            std::cout << std::left << std::setw(30) << entry.path().filename()
                << std::left << std::setw(30) << compute_directory_size(entry);

            if (std::filesystem::is_regular_file(entry.status())) {
                auto ftime = std::filesystem::last_write_time(entry);
                time_t t = to_time_t(ftime);
                std::cout << std::asctime(std::localtime(&t));
            } else std::cout << std::endl;
        }
    }
}

int main () {
    std::string user_path;

    std::cin >> user_path;
    view_directory(user_path);

    return 0;
}
