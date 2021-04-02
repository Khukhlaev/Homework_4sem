
#include <iostream>
#include <string>
#include <mutex>

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>

int main () {
    const std::string name = "sm";
    system("pause");

    boost::interprocess::managed_shared_memory sm (boost::interprocess::open_only, name.c_str());

    using allocator_t = boost::interprocess::allocator <int,
            boost::interprocess::managed_shared_memory::segment_manager>;

    using vector_t = boost::interprocess::vector< int, allocator_t>;

    auto v = sm.find<vector_t>("v").first;

    auto m = sm.find<boost::interprocess::interprocess_mutex>("m").first;

    auto c = sm.find<boost::interprocess::interprocess_condition>("c").first;

    int x = 0;

    do {
        std::unique_lock lock(*m);

        c->wait(lock, [v](){return !v->empty();});

        x = v->back();

        std::cout << x;

        v->pop_back();
    }
    while (x != 0);

    system("pause");
}