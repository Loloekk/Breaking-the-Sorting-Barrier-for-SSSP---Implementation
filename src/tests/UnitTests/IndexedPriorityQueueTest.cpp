#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <utility>
#include <stdexcept>

#include "BatchPriorityQueue.h"
#include "TestExecutor.h"
#include "IndexedPriorityQueue.hpp"

void test_basic_insert_and_pop()
{
    std::vector<int> pointers(100, -1);
    IndexedPriorityQueue<int> queue(pointers);

    queue.set_value(2, 5);
    queue.set_value(3, 2);
    queue.set_value(68, 3);

    if (queue.top() != std::pair<int, int>{3, 2})
        throw std::runtime_error("Queue is uncorrect: Wrong top after insert");

    queue.pop();
    if (queue.top() != std::pair<int, int>{68, 3})
        throw std::runtime_error("Queue is uncorrect: Wrong top after first pop");

    queue.pop();
    if (queue.top() != std::pair<int, int>{2, 5})
        throw std::runtime_error("Queue is uncorrect: Wrong top after second pop");

    queue.pop();
    if (!queue.empty())
        throw std::runtime_error("Queue is uncorrect: Queue should be empty");
}

void test_update_decrease_key()
{
    std::vector<int> pointers(100, -1);
    IndexedPriorityQueue<int> queue(pointers);

    queue.set_value(2, 5);
    queue.set_value(3, 2);
    queue.set_value(68, 3);

    queue.set_value(2, -1);

    if (queue.top() != std::pair<int, int>{2, -1})
        throw std::runtime_error("Queue is uncorrect: Wrong top after decreasing value");

    queue.pop();
    if (queue.top() != std::pair<int, int>{3, 2})
        throw std::runtime_error("Queue is uncorrect: Wrong top after pop");

    queue.pop();
    if (queue.top() != std::pair<int, int>{68, 3})
        throw std::runtime_error("Queue is uncorrect: Wrong top after pop");

    queue.pop();
    if (!queue.empty())
        throw std::runtime_error("Queue is uncorrect: Queue should be empty");
}

void test_update_increase_key()
{
    std::vector<int> pointers(10, -1);
    IndexedPriorityQueue<int> queue(pointers);

    queue.set_value(1, 10);
    queue.set_value(2, 20);
    queue.set_value(3, 30);

    queue.set_value(1, 40);

    if (queue.top() != std::pair<int, int>{2, 20})
        throw std::runtime_error("Queue is uncorrect: Key 2 should be the new top");

    queue.pop();
    if (queue.top() != std::pair<int, int>{3, 30})
        throw std::runtime_error("Queue is uncorrect: Key 3 should be next");

    queue.pop();
    if (queue.top() != std::pair<int, int>{1, 40})
        throw std::runtime_error("Queue is uncorrect: Key 1 should be last");
}

void test_exceptions_and_bounds()
{
    std::vector<int> pointers(5, -1);
    IndexedPriorityQueue<int> queue(pointers);

    bool exception_thrown = false;

    try
    {
        queue.top();
    }
    catch (const std::out_of_range &)
    {
        exception_thrown = true;
    }
    if (!exception_thrown)
        throw std::runtime_error("Queue is uncorrect: No exception on empty top()");

    exception_thrown = false;
    try
    {
        queue.pop();
    }
    catch (const std::out_of_range &)
    {
        exception_thrown = true;
    }
    if (!exception_thrown)
        throw std::runtime_error("Queue is uncorrect: No exception on empty pop()");

    exception_thrown = false;
    try
    {
        queue.set_value(10, 100);
    }
    catch (const std::out_of_range &)
    {
        exception_thrown = true;
    }
    if (!exception_thrown)
        throw std::runtime_error("Queue is uncorrect: No exception on out of bounds set_value()");
}

void test_reinsertion()
{
    std::vector<int> pointers(5, -1);
    IndexedPriorityQueue<int> queue(pointers);

    queue.set_value(2, 50);
    if (queue.top() != std::pair<int, int>{2, 50})
        throw std::runtime_error("Queue is uncorrect: Wrong top before reinsertion");

    queue.pop();
    if (!queue.empty())
        throw std::runtime_error("Queue is uncorrect: Should be empty before reinsertion");

    queue.set_value(2, 10);
    queue.set_value(1, 5);
    if (queue.top() != std::pair<int, int>{1, 5})
        throw std::runtime_error("Queue is uncorrect: Wrong top after reinsertion");

    queue.pop();
    if (queue.top() != std::pair<int, int>{2, 10})
        throw std::runtime_error("Queue is uncorrect: Key 2 should work again");
}

void test_heap_sort_stress()
{
    int MAX_N = 1000;
    std::vector<int> pointers(MAX_N, -1);
    IndexedPriorityQueue<int> queue(pointers);

    for (int i = 0; i < 500; ++i)
    {
        queue.set_value(i, 1000 - i);
    }

    int prev_value = -1;
    for (int i = 0; i < 500; ++i)
    {
        auto [k, v] = queue.top();
        if (v < prev_value)
            throw std::runtime_error("Queue is uncorrect: Min-Heap property broken during stress test");
        prev_value = v;
        queue.pop();
    }

    if (!queue.empty())
        throw std::runtime_error("Queue is uncorrect: Should be empty after stress test");
}

int main()
{
    std::cout << "--- Starting Indexed Priority Queue Unit Tests ---" << std::endl;

    runTest(test_basic_insert_and_pop, "test_basic_insert_and_pop");
    runTest(test_update_decrease_key, "test_update_decrease_key");
    runTest(test_update_increase_key, "test_update_increase_key");
    runTest(test_exceptions_and_bounds, "test_exceptions_and_bounds");
    runTest(test_reinsertion, "test_reinsertion");
    runTest(test_heap_sort_stress, "test_heap_sort_stress");

    std::cout << "--- Indexed Priority Queue Tests Finished ---" << std::endl
              << std::endl
              << std::endl;
    return 0;
}