#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <atomic>

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>


using allocator_str = boost::interprocess::allocator < char,
        boost::interprocess::managed_shared_memory::segment_manager > ;

using string_t = boost::interprocess::basic_string < char,
        std::char_traits < char >, allocator_str> ;

using allocator_vec = boost::interprocess::allocator <string_t,
        boost::interprocess::managed_shared_memory::segment_manager>;

using vector_t = boost::interprocess::vector< string_t, allocator_vec>;




void write_messages (string_t& buffer,
                     vector_t* messages, boost::interprocess::interprocess_mutex* mutex,
                     boost::interprocess::interprocess_condition* condition_var, bool& written_message,
                     bool& end) {

    std::string buf;

    while (true) {

        std::getline(std::cin, buf);

        if (buf == "exit") break;

        buffer = buf;

        std::scoped_lock lock (*mutex);
        written_message = true;

        messages->push_back(buffer);

        condition_var->notify_all();
    }

    end = true;

    condition_var->notify_all();

}

void read_messages (vector_t* messages, boost::interprocess::interprocess_mutex* mutex,
                    boost::interprocess::interprocess_condition* condition_var, bool& written_message,
                    bool& end) {

    //All messages up to date
    std::size_t prev_messages_size = 0;
    {
        std::scoped_lock lock(*mutex);

        prev_messages_size = messages->size();

        for (const auto &i : *messages) {
            std::cout << i << "\n";
        }

    }

    // Printing new messages
    while (true) {

        std::unique_lock lock(*mutex);

        condition_var->wait(lock, [messages, prev_messages_size, &end]()
            { return messages->size() != prev_messages_size || end; });

        if (end) break;

        prev_messages_size = messages->size();

        // To not print message if it was written by this user
        if (!written_message) std::cout << messages->back() << std::endl;

        written_message = false;

    }

}

int main () {

    const std::string name = "shared memory";

    system("pause");


    boost::interprocess::managed_shared_memory sm (boost::interprocess::open_or_create, name.c_str(), 65536U);

    auto messages = sm.find_or_construct<vector_t>("messages")(sm.get_segment_manager());

    auto mutex = sm.find_or_construct<boost::interprocess::interprocess_mutex>("mutex")();

    auto condition_var = sm.find_or_construct<boost::interprocess::interprocess_condition>("condition_var")();

    auto counter_processes = sm.find_or_construct<std::atomic<int>>("counter")();
    counter_processes->store(*counter_processes + 1);

    bool written_message = false; // True if there is a message written by this user

    bool end = false; // True if user write end and we want to close program

    string_t buffer(sm.get_segment_manager());

    std::thread thread(write_messages, std::ref(buffer),
                       messages, mutex, condition_var, std::ref(written_message), std::ref(end));

    thread.detach();

    read_messages(messages, mutex, condition_var, written_message, end);

    if (counter_processes->load() != 1) counter_processes->store(*counter_processes - 1);
    else boost::interprocess::shared_memory_object::remove(name.c_str());

}