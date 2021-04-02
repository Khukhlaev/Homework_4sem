
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
    boost::interprocess::shared_memory_object::remove(name.c_str());

    boost::interprocess::managed_shared_memory sm (boost::interprocess::create_only, name.c_str(), 4096U);


    using allocator_t = boost::interprocess::allocator <int,
            boost::interprocess::managed_shared_memory::segment_manager>;

    using vector_t = boost::interprocess::vector< int, allocator_t>;

    auto v = sm.construct<vector_t>("v")(sm.get_segment_manager());

    auto m = sm.construct<boost::interprocess::interprocess_mutex>("m")();

    auto c = sm.construct<boost::interprocess::interprocess_condition>("c")();

    int x = 0;

    do {
        std::cin >> x;

        std::scoped_lock lock (*m);

        v->push_back(x);

        c->notify_one();
    }
    while (x != 0);

    system("pause");
    boost::interprocess::shared_memory_object::remove(name.c_str());

    system("pause");
}