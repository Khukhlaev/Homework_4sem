#include <iostream>
#include <thread>
#include <boost/lockfree/stack.hpp>
#include <boost/lockfree/queue.hpp>
#include "threadsafe_queue.h"
#include "threadsafe_stack.h"
#include "Timer.h"
#include <atomic>

class Threads_Guard
{
public:

    explicit Threads_Guard(std::vector < std::thread > & threads) :
            m_threads(threads)
    {}

    Threads_Guard			(Threads_Guard const&) = delete;

    Threads_Guard& operator=(Threads_Guard const&) = delete;

    ~Threads_Guard() noexcept
    {
        try
        {
            for (auto & m_thread : m_threads)
            {
                if (m_thread.joinable())
                {
                    m_thread.join();
                }
            }
        }
        catch (...)
        {
            // std::abort();
        }
    }

private:

    std::vector < std::thread > & m_threads;
};


template <typename Stack>
void pushStack(Stack& stack, int number_to_push, std::atomic<bool>& flag) {

    while (!flag.load()) {
        std::this_thread::yield();
    }

    for (int i = 0; i < number_to_push; i++) {
        stack.push(i);
    }

}

template <typename Stack>
void popStack(Stack& stack, int number_to_pop, std::atomic<bool>& flag) {

    while (!flag.load()) {
        std::this_thread::yield();
    }

    int res;
    for (int i = 0; i < number_to_pop; i++) {
        stack.pop(res);
    }

}


void testStack() {

    const int number_threads = 4;
    const int number_tasks = 1000000;

    Threadsafe_Stack<int> stack_1;
    boost::lockfree::stack<int> stack_2(2 * number_tasks * number_threads);

    for (int i = 0; i < number_threads * number_tasks; i++) {
        stack_1.push(i);
        stack_2.push(i);
    }

    Timer t;


    {
        std::atomic<bool> flag(false);

        std::vector<std::thread> threads(2 * number_threads);
        Threads_Guard guard(threads);

        for (int i = 0; i < 2 * number_threads; i++) {
            if (i < number_threads) {

                threads[i] = std::thread(pushStack<Threadsafe_Stack<int>>,
                                         std::ref(stack_1), number_tasks, std::ref(flag));
            }
            else {

                threads[i] = std::thread(popStack<Threadsafe_Stack<int>>,
                                         std::ref(stack_1), number_tasks, std::ref(flag));
            }
        }

        t.start_timer();
        flag.store(true);

    }
    t.print_time(std::cout);

    std::cout << "\n";

    t.reset_timer();

    {
        std::atomic<bool> flag(false);

        std::vector<std::thread> threads(2 * number_threads);
        Threads_Guard guard(threads);

        for (int i = 0; i < 2 * number_threads; i++) {
            if (i < number_threads) {

                threads[i] = std::thread(pushStack<boost::lockfree::stack<int>>,
                                         std::ref(stack_2), number_tasks, std::ref(flag));
            }
            else {

                threads[i] = std::thread(popStack<boost::lockfree::stack<int>>,
                                         std::ref(stack_2), number_tasks, std::ref(flag));
            }
        }

        t.start_timer();
        flag.store(true);

    }
    t.print_time(std::cout);

}


template <typename Queue>
void pushQueue(Queue& queue, int number_to_push, std::atomic<bool>& flag) {

    while (!flag.load()) {
        std::this_thread::yield();
    }

    for (int i = 0; i < number_to_push; i++) {
        queue.push(i);
    }

}

void popBoostQueue(boost::lockfree::queue<int>& queue, int number_to_pop, std::atomic<bool>& flag) {

    while (!flag.load()) {
        std::this_thread::yield();
    }

    int res;
    for (int i = 0; i < number_to_pop; i++) {
        queue.pop(res);
    }

}


void popOurQueue(Threadsafe_Queue<int>& queue, int number_to_pop, std::atomic<bool>& flag) {

    while (!flag.load()) {
        std::this_thread::yield();
    }

    int res;
    for (int i = 0; i < number_to_pop; i++) {
        queue.wait_and_pop(res);
    }
}


void testQueue() {
    const int number_threads = 8;
    const int number_tasks = 1000000;

    Threadsafe_Queue<int> queue_1;
    boost::lockfree::queue<int> queue_2(2 * number_tasks * number_threads);

    Timer t;

    {
        std::atomic<bool> flag(false);

        std::vector<std::thread> threads(2 * number_threads);
        Threads_Guard guard(threads);

        for (int i = 0; i < 2 * number_threads; i++) {
            if (i < number_threads) {

                threads[i] = std::thread(pushQueue<Threadsafe_Queue<int>>,
                                         std::ref(queue_1), number_tasks, std::ref(flag));
            }
            else {

                threads[i] = std::thread(popOurQueue, std::ref(queue_1), number_tasks, std::ref(flag));
            }
        }

        t.start_timer();
        flag.store(true);

    }
    t.print_time(std::cout);

    std::cout << "\n";

    t.reset_timer();

    {
        std::atomic<bool> flag(false);

        std::vector<std::thread> threads(2 * number_threads);
        Threads_Guard guard(threads);

        for (int i = 0; i < 2 * number_threads; i++) {
            if (i < number_threads) {

                threads[i] = std::thread(pushStack<boost::lockfree::queue<int>>,
                                         std::ref(queue_2), number_tasks, std::ref(flag));
            }
            else {

                threads[i] = std::thread(popBoostQueue, std::ref(queue_2), number_tasks, std::ref(flag));
            }
        }

        t.start_timer();
        flag.store(true);

    }
    t.print_time(std::cout);


}


int main() {

    // testStack();

    // Threadsafe_Stack при большом количестве задач сильно быстрее версии из boost
    // Пример: при 4 потоках (т.е. суммарно 8) и 1000000 задач (на поток)
    // Threadsafe_Stack - 346 ms
    // boost version - 945 ms
    // Однако при большом количестве потоков и малом количестве задач можно найти момент, когда быстрее версия из boost
    // (16 потоков и 10000 задач), но отличие небольшое (несколько милисекунд на масштабе десятков милисекунд)
    // и можно сказать, что на таком масштабе они работают примерно одинаково (может быть как один быстрее, так и другой)



    testQueue();

    // Аналогично stack на большом количестве задач наша версия явно быстрее версии из boost
    // Пример: при 8 потоках и 1000000 задач:
    // Threadsafe_queue - 770 ms
    // boost version - 1439 ms
    // Однако при меньшем количестве задач обе версии начинают работать примерно одинаково, иногда быстрее версия из boost:
    // Пример: при 8 потоках и 100000 задач:
    // Threadsafe_queue - 175 ms
    // boost version - 132 ms


    return 0;
}

