#include <iostream>
#include <string>
#include <functional>
#include <utility>
#include <vector>
#include <algorithm>
#include "BatchPriorityQueue.h"
#include "TestExecutor.h"

class Ints
{
    int value_;

public:
    Ints(int value) : value_(value) {}
    Ints() {}
    std::string to_string() const
    {
        return std::to_string(value_);
    }
    bool operator<(const Ints &other) const
    {
        return value_ < other.value_;
    }
    bool operator==(const Ints &other) const
    {
        return value_ == other.value_;
    }
    bool operator!=(const Ints &other) const
    {
        return value_ != other.value_;
    }
    operator int() const
    {
        return value_;
    }
};

template <typename V>
void areEqual(const BatchPriorityQueue<V> &queue, const std::string expected)
{
    if (queue.to_string() != expected)
    {
        std::cout << "queue: " << queue.to_string() << std::endl;
        std::cout << "expected: " << expected << std::endl;
        throw std::runtime_error("Queue is uncorrect");
    }
}

void areEqual(const std::string &answear, const std::string &expected)
{
    if (answear != expected)
    {
        std::cout << "answear: " << answear << std::endl;
        std::cout << "expected: " << expected << std::endl;
        throw std::runtime_error("Answer is uncorrect");
    }
}

void areEqual(std::pair<std::vector<int>, Ints> answear, std::pair<std::vector<int>, Ints> expected)
{
    std::sort(answear.first.begin(), answear.first.end());
    std::sort(expected.first.begin(), expected.first.end());
    if (answear.first != expected.first || answear.second != expected.second)
    {
        std::cout << "answear: [";
        for (int i = 0; i < (int)answear.first.size(); ++i)
            std::cout << answear.first[i] << (i + 1 == (int)answear.first.size() ? "" : ", ");
        std::cout << "] {" << answear.second.to_string() << "}" << std::endl;

        std::cout << "expected: [";
        for (int i = 0; i < (int)expected.first.size(); ++i)
            std::cout << expected.first[i] << (i + 1 == (int)expected.first.size() ? "" : ", ");
        std::cout << "] {" << expected.second.to_string() << "}" << std::endl;

        throw std::runtime_error("Answer is uncorrect");
    }
}

void test_empty_queue()
{
    int MAX_N = 20;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);
    areEqual(queue, "[D0:[], D1:[[{1000}]], M: 3, B: 1000, size: 0]");
}

void test_insert_different_keys_and_split()
{
    int MAX_N = 200;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);

    queue.insert(5, 2);
    areEqual(queue, "[D0:[], D1:[[(5,2), {1000}]], M: 3, B: 1000, size: 1]");

    queue.insert(6, 3);
    areEqual(queue, "[D0:[], D1:[[(5,2), (6,3), {1000}]], M: 3, B: 1000, size: 2]");

    queue.insert(7, 5);
    areEqual(queue, "[D0:[], D1:[[(5,2), (6,3), (7,5), {1000}]], M: 3, B: 1000, size: 3]");

    queue.insert(8, 11);
    areEqual(queue, "[D0:[], D1:[[(5,2), (6,3), {5}], [(7,5), (8,11), {1000}]], M: 3, B: 1000, size: 4]");

    queue.insert(9, 12);
    areEqual(queue, "[D0:[], D1:[[(5,2), (6,3), {5}], [(7,5), (8,11), (9,12), {1000}]], M: 3, B: 1000, size: 5]");

    queue.insert(68, 1);
    areEqual(queue, "[D0:[], D1:[[(5,2), (6,3), (68,1), {5}], [(7,5), (8,11), (9,12), {1000}]], M: 3, B: 1000, size: 6]");

    queue.insert(80, 4);
    areEqual(queue, "[D0:[], D1:[[(5,2), (68,1), {3}], [(6,3), (80,4), {5}], [(7,5), (8,11), (9,12), {1000}]], M: 3, B: 1000, size: 7]");

    queue.insert(112, 0);
    areEqual(queue, "[D0:[], D1:[[(5,2), (68,1), (112,0), {3}], [(6,3), (80,4), {5}], [(7,5), (8,11), (9,12), {1000}]], M: 3, B: 1000, size: 8]");
}

void test_erase_without_splitting()
{
    int MAX_N = 200;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);
    queue.insert(5, 2);
    queue.insert(6, 3);
    queue.insert(7, 5);
    areEqual(queue, "[D0:[], D1:[[(5,2), (6,3), (7,5), {1000}]], M: 3, B: 1000, size: 3]");
    areEqual(queue.get_D1_values_string(), "[{1000}]");
    queue.erase(6);
    areEqual(queue, "[D0:[], D1:[[(5,2), (7,5), {1000}]], M: 3, B: 1000, size: 2]");
    areEqual(queue.get_D1_values_string(), "[{1000}]");
    queue.erase(7);
    areEqual(queue, "[D0:[], D1:[[(5,2), {1000}]], M: 3, B: 1000, size: 1]");
    areEqual(queue.get_D1_values_string(), "[{1000}]");
    queue.erase(5);
    areEqual(queue, "[D0:[], D1:[[{1000}]], M: 3, B: 1000, size: 0]");
    areEqual(queue.get_D1_values_string(), "[{1000}]");
}

void test_erase()
{
    int MAX_N = 200;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);
    queue.insert(5, 2);
    queue.insert(6, 3);
    queue.insert(7, 5);
    queue.insert(8, 11);
    queue.insert(9, 12);
    queue.insert(68, 1);
    queue.insert(80, 4);
    queue.insert(112, 0);

    areEqual(queue, "[D0:[], D1:[[(5,2), (68,1), (112,0), {3}], [(6,3), (80,4), {5}], [(7,5), (8,11), (9,12), {1000}]], M: 3, B: 1000, size: 8]");
    areEqual(queue.get_D1_values_string(), "[{3}, {5}, {1000}]");

    queue.erase(68);
    areEqual(queue, "[D0:[], D1:[[(5,2), (112,0), {3}], [(6,3), (80,4), {5}], [(7,5), (8,11), (9,12), {1000}]], M: 3, B: 1000, size: 7]");
    areEqual(queue.get_D1_values_string(), "[{3}, {5}, {1000}]");

    queue.erase(6);
    areEqual(queue, "[D0:[], D1:[[(5,2), (112,0), {3}], [(80,4), {5}], [(7,5), (8,11), (9,12), {1000}]], M: 3, B: 1000, size: 6]");
    areEqual(queue.get_D1_values_string(), "[{3}, {5}, {1000}]");

    queue.erase(7);
    areEqual(queue, "[D0:[], D1:[[(5,2), (112,0), {3}], [(80,4), {5}], [(8,11), (9,12), {1000}]], M: 3, B: 1000, size: 5]");
    areEqual(queue.get_D1_values_string(), "[{3}, {5}, {1000}]");

    queue.erase(9);
    areEqual(queue, "[D0:[], D1:[[(5,2), (112,0), {3}], [(80,4), {5}], [(8,11), {1000}]], M: 3, B: 1000, size: 4]");
    areEqual(queue.get_D1_values_string(), "[{3}, {5}, {1000}]");

    queue.erase(112);
    areEqual(queue, "[D0:[], D1:[[(5,2), {3}], [(80,4), {5}], [(8,11), {1000}]], M: 3, B: 1000, size: 3]");
    areEqual(queue.get_D1_values_string(), "[{3}, {5}, {1000}]");

    queue.erase(8);
    areEqual(queue, "[D0:[], D1:[[(5,2), {3}], [(80,4), {5}], [{1000}]], M: 3, B: 1000, size: 2]");
    areEqual(queue.get_D1_values_string(), "[{3}, {5}, {1000}]");

    queue.erase(80);
    areEqual(queue, "[D0:[], D1:[[(5,2), {3}], [{1000}]], M: 3, B: 1000, size: 1]");
    areEqual(queue.get_D1_values_string(), "[{3}, {1000}]");

    queue.erase(5);
    areEqual(queue, "[D0:[], D1:[[{1000}]], M: 3, B: 1000, size: 0]");
    areEqual(queue.get_D1_values_string(), "[{1000}]");
}

void test_is_key()
{
    int MAX_N = 200;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);
    if (queue.isKey(0))
        throw std::runtime_error("Key error: this key was accepted in empty queue.");
    queue.insert(5, 2);
    if (!queue.isKey(5))
        throw std::runtime_error("Key error: this key was not accepted after insertion.");
    queue.insert(6, 3);
    if (!queue.isKey(6))
        throw std::runtime_error("Key error: this key was not accepted after insertion.");
    queue.insert(7, 5);
    queue.insert(8, 11);
    queue.insert(9, 12);
    queue.insert(68, 1);
    queue.insert(80, 4);
    if (!queue.isKey(68))
        throw std::runtime_error("Key error: this key was not accepted after insertion.");
    queue.insert(112, 0);

    queue.erase(68);
    queue.erase(6);
    if (queue.isKey(6))
        throw std::runtime_error("Key error: this key was accepted after erasure.");
    queue.erase(7);
    queue.erase(9);
    queue.erase(112);
    if (queue.isKey(112))
        throw std::runtime_error("Key error: this key was accepted after erasure.");
    queue.erase(8);
    queue.erase(80);
    queue.erase(5);
    if (queue.isKey(5))
        throw std::runtime_error("Key error: this key was accepted after erasure.");
    if (queue.isKey(0))
        throw std::runtime_error("Key error: this key was accepted in empty queue.");
}

void test_key_bound()
{
    int MAX_N = 20;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);
    try
    {
        queue.erase(-1);
        throw std::runtime_error("Key bound error: negative key was accepted.");
    }
    catch (const std::logic_error &e)
    {
        areEqual(e.what(), "Pointer overflow: key is out of range.");
    }
    try
    {
        queue.erase(21);
        throw std::runtime_error("Key bound error: key greater than MAX_N was accepted.");
    }
    catch (const std::logic_error &e)
    {
        areEqual(e.what(), "Pointer overflow: key is out of range.");
    }
}

void test_insert_value_bound()
{
    int MAX_N = 20;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);
    try
    {
        queue.insert(5, 1001);
        throw std::runtime_error("Value bound error: value greater than B was accepted.");
    }
    catch (const std::logic_error &e)
    {
        areEqual(e.what(), "Cannot insert: inserted object has value over queue's value");
    }
}

void test_insert_key_bound()
{
    int MAX_N = 20;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);
    try
    {
        queue.insert(-1, 5);
        throw std::runtime_error("Key bound error: negative key was accepted.");
    }
    catch (const std::logic_error &e)
    {
        areEqual(e.what(), "Pointer overflow: key is out of range.");
    }
    try
    {
        queue.insert(21, 5);
        throw std::runtime_error("Key bound error: key greater than MAX_N was accepted.");
    }
    catch (const std::logic_error &e)
    {
        areEqual(e.what(), "Pointer overflow: key is out of range.");
    }
}

void test_insert_key_bound_on_batch_insert()
{
    int MAX_N = 20;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);
    try
    {
        queue.insert(std::vector<std::pair<int, Ints>>{{5, 1001}, {6, 3}});
        throw std::runtime_error("Value bound error: value greater than B was accepted.");
    }
    catch (const std::logic_error &e)
    {
        areEqual(e.what(), "Cannot insert: inserted object has value over queue's value");
    }
}

void test_insert_value_bound_on_batch_insert()
{
    int MAX_N = 20;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);
    try
    {
        queue.insert(std::vector<std::pair<int, Ints>>{{5, 1001}, {6, 3}});
        throw std::runtime_error("Value bound error: value greater than B was accepted.");
    }
    catch (const std::logic_error &e)
    {
        areEqual(e.what(), "Cannot insert: inserted object has value over queue's value");
    }
}

void test_insert_with_existing_key()
{
    int MAX_N = 200;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);
    queue.insert(5, 2);
    queue.insert(6, 3);
    queue.insert(7, 5);
    queue.insert(8, 11);
    queue.insert(9, 12);
    queue.insert(68, 1);
    queue.insert(80, 4);
    queue.insert(112, 0);

    areEqual(queue, "[D0:[], D1:[[(5,2), (68,1), (112,0), {3}], [(6,3), (80,4), {5}], [(7,5), (8,11), (9,12), {1000}]], M: 3, B: 1000, size: 8]");
    areEqual(queue.get_D1_values_string(), "[{3}, {5}, {1000}]");

    queue.insert(5, 10);
    areEqual(queue, "[D0:[], D1:[[(5,2), (68,1), (112,0), {3}], [(6,3), (80,4), {5}], [(7,5), (8,11), (9,12), {1000}]], M: 3, B: 1000, size: 8]");
    areEqual(queue.get_D1_values_string(), "[{3}, {5}, {1000}]");

    queue.insert(6, 20);
    areEqual(queue, "[D0:[], D1:[[(5,2), (68,1), (112,0), {3}], [(6,3), (80,4), {5}], [(7,5), (8,11), (9,12), {1000}]], M: 3, B: 1000, size: 8]");
    areEqual(queue.get_D1_values_string(), "[{3}, {5}, {1000}]");

    queue.insert(7, -2);
    areEqual(queue, "[D0:[], D1:[[(112,0), (7,-2), {1}], [(5,2), (68,1), {3}], [(6,3), (80,4), {5}], [(8,11), (9,12), {1000}]], M: 3, B: 1000, size: 8]");
    areEqual(queue.get_D1_values_string(), "[{1}, {3}, {5}, {1000}]");

    queue.insert(80, 2);
    areEqual(queue, "[D0:[], D1:[[(112,0), (7,-2), {1}], [(5,2), (68,1), (80,2), {3}], [(6,3), {5}], [(8,11), (9,12), {1000}]], M: 3, B: 1000, size: 8]");
    areEqual(queue.get_D1_values_string(), "[{1}, {3}, {5}, {1000}]");

    queue.insert(8, 4);
    areEqual(queue, "[D0:[], D1:[[(112,0), (7,-2), {1}], [(5,2), (68,1), (80,2), {3}], [(6,3), (8,4), {5}], [(9,12), {1000}]], M: 3, B: 1000, size: 8]");
    areEqual(queue.get_D1_values_string(), "[{1}, {3}, {5}, {1000}]");

    queue.insert(9, -1);
    areEqual(queue, "[D0:[], D1:[[(112,0), (7,-2), (9,-1), {1}], [(5,2), (68,1), (80,2), {3}], [(6,3), (8,4), {5}], [{1000}]], M: 3, B: 1000, size: 8]");
    areEqual(queue.get_D1_values_string(), "[{1}, {3}, {5}, {1000}]");

    queue.insert(8, 3);
    areEqual(queue, "[D0:[], D1:[[(112,0), (7,-2), (9,-1), {1}], [(5,2), (68,1), (80,2), {3}], [(6,3), (8,3), {5}], [{1000}]], M: 3, B: 1000, size: 8]");
    areEqual(queue.get_D1_values_string(), "[{1}, {3}, {5}, {1000}]");

    queue.insert(8, 20);
    areEqual(queue, "[D0:[], D1:[[(112,0), (7,-2), (9,-1), {1}], [(5,2), (68,1), (80,2), {3}], [(6,3), (8,3), {5}], [{1000}]], M: 3, B: 1000, size: 8]");
    areEqual(queue.get_D1_values_string(), "[{1}, {3}, {5}, {1000}]");

    try
    {
        queue.insert(8, 1001);
        throw std::runtime_error("Value bound error: value greater than B was accepted.");
    }
    catch (const std::logic_error &e)
    {
        areEqual(e.what(), "Cannot insert: inserted object has value over queue's value");
    }

    areEqual(queue, "[D0:[], D1:[[(112,0), (7,-2), (9,-1), {1}], [(5,2), (68,1), (80,2), {3}], [(6,3), (8,3), {5}], [{1000}]], M: 3, B: 1000, size: 8]");
    areEqual(queue.get_D1_values_string(), "[{1}, {3}, {5}, {1000}]");
}

void test_insert_batch_simply()
{
    int MAX_N = 200;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);

    queue.insert(std::vector<std::pair<int, Ints>>{{5, 25}, {6, 28}, {7, 54}});
    areEqual(queue, "[D0:[[(5,25), (6,28), (7,54), {25}]], D1:[[{1000}]], M: 3, B: 1000, size: 3]");

    queue.insert(std::vector<std::pair<int, Ints>>{{8, 11}, {9, 12}, {68, 1}, {80, 4}, {112, 0}});
    areEqual(queue, "[D0:[[(68,1), (112,0), {0}], [(8,11), (9,12), (80,4), {4}], [(5,25), (6,28), (7,54), {25}]], D1:[[{1000}]], M: 3, B: 1000, size: 8]");
}

void test_big_insert_batch()
{
    int MAX_N = 200;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);
    std::vector<std::pair<int, Ints>> data;

    for (int i = 1; i <= 20; i++)
        data.push_back({i, Ints(i * 10)});

    queue.insert(data);

    areEqual(queue, "[D0:[[(1,10), (2,20), {10}], [(3,30), (4,40), (5,50), {30}], [(6,60), (7,70), {60}], [(8,80), (9,90), (10,100), {80}], [(11,110), (12,120), {110}], [(13,130), (14,140), (15,150), {130}], [(16,160), (17,170), {160}], [(18,180), (19,190), (20,200), {180}]], D1:[[{1000}]], M: 3, B: 1000, size: 20]");
}

void test_is_key_with_insert_batch()
{
    int MAX_N = 200;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);
    std::vector<std::pair<int, Ints>> data;
    for (int i = 1; i <= 20; i++)
        data.push_back({i, Ints(i * 10)});
    queue.insert(data);
    for (int i = 1; i <= 20; i++)
        if (!queue.isKey(i))
            throw std::runtime_error("Key error: this key was not accepted after batch insertion.");
}

void test_erase_with_insert_batch()
{
    int MAX_N = 200;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);
    std::vector<std::pair<int, Ints>> data;
    for (int i = 1; i <= 20; i++)
        data.push_back({i, Ints(i * 10)});
    queue.insert(data);

    for (int i = 1; i <= 20; i += 2)
        queue.erase(i);

    areEqual(queue, "[D0:[[(2,20), {10}], [(4,40), {30}], [(6,60), {60}], [(8,80), (10,100), {80}], [(12,120), {110}], [(14,140), {130}], [(16,160), {160}], [(18,180), (20,200), {180}]], D1:[[{1000}]], M: 3, B: 1000, size: 10]");

    for (int i = 1; i <= 20; i += 2)
        if (queue.isKey(i))
            throw std::runtime_error("Key error: this key was accepted after erasure.");

    queue.erase(12);
    queue.erase(8);
    queue.erase(10);

    areEqual(queue, "[D0:[[(2,20), {10}], [(4,40), {30}], [(6,60), {60}], [(14,140), {130}], [(16,160), {160}], [(18,180), (20,200), {180}]], D1:[[{1000}]], M: 3, B: 1000, size: 7]");
}

void test_insert_batch_with_existing_keys()
{
    int MAX_N = 200;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);
    std::vector<std::pair<int, Ints>> data;

    for (int i = 1; i <= 20; i++)
        data.push_back({i, Ints(i * 10 + 30)});

    queue.insert(data);

    areEqual(queue, "[D0:[[(1,40), (2,50), {40}], [(3,60), (4,70), (5,80), {60}], [(6,90), (7,100), {90}], [(8,110), (9,120), (10,130), {110}], [(11,140), (12,150), {140}], [(13,160), (14,170), (15,180), {160}], [(16,190), (17,200), {190}], [(18,210), (19,220), (20,230), {210}]], D1:[[{1000}]], M: 3, B: 1000, size: 20]");

    queue.insert(std::vector<std::pair<int, Ints>>{{25, 15}, {2, 21}, {6, 16}, {7, 28}, {65, 36}});

    areEqual(queue, "[D0:[[(25,15), (6,16), {15}], [(2,21), (7,28), (65,36), {21}], [(1,40), {40}], [(3,60), (4,70), (5,80), {60}], [(8,110), (9,120), (10,130), {110}], [(11,140), (12,150), {140}], [(13,160), (14,170), (15,180), {160}], [(16,190), (17,200), {190}], [(18,210), (19,220), (20,230), {210}]], D1:[[{1000}]], M: 3, B: 1000, size: 22]");
}

void test_insert_batch_the_same_keys()
{
    int MAX_N = 200;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);
    std::vector<std::pair<int, Ints>> data;

    data.push_back({4, 5});
    data.push_back({4, 3});
    data.push_back({4, 8});
    data.push_back({4, 2});

    queue.insert(data);

    areEqual(queue, "[D0:[[(4,2), {2}]], D1:[[{1000}]], M: 3, B: 1000, size: 1]");
}

void test_insert_mixed_with_existing_keys()
{
    int MAX_N = 200;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);
    std::vector<std::pair<int, Ints>> data;

    for (int i = 1; i <= 20; i++)
        data.push_back({i, Ints(i * 10 + 30)});

    queue.insert(data);
    areEqual(queue, "[D0:[[(1,40), (2,50), {40}], [(3,60), (4,70), (5,80), {60}], [(6,90), (7,100), {90}], [(8,110), (9,120), (10,130), {110}], [(11,140), (12,150), {140}], [(13,160), (14,170), (15,180), {160}], [(16,190), (17,200), {190}], [(18,210), (19,220), (20,230), {210}]], D1:[[{1000}]], M: 3, B: 1000, size: 20]");

    queue.insert(25, 15);
    queue.insert(2, 21);
    queue.insert(6, 16);
    queue.insert(7, 28);
    queue.insert(65, 36);
    areEqual(queue, "[D0:[[(1,40), {40}], [(3,60), (4,70), (5,80), {60}], [(8,110), (9,120), (10,130), {110}], [(11,140), (12,150), {140}], [(13,160), (14,170), (15,180), {160}], [(16,190), (17,200), {190}], [(18,210), (19,220), (20,230), {210}]], D1:[[(25,15), (6,16), {21}], [(2,21), (7,28), (65,36), {1000}]], M: 3, B: 1000, size: 22]");

    queue.insert(std::vector<std::pair<int, Ints>>());
    areEqual(queue, "[D0:[[(1,40), {40}], [(3,60), (4,70), (5,80), {60}], [(8,110), (9,120), (10,130), {110}], [(11,140), (12,150), {140}], [(13,160), (14,170), (15,180), {160}], [(16,190), (17,200), {190}], [(18,210), (19,220), (20,230), {210}]], D1:[[(25,15), (6,16), {21}], [(2,21), (7,28), (65,36), {1000}]], M: 3, B: 1000, size: 22]");

    queue.insert(std::vector<std::pair<int, Ints>>{{25, 20}, {2, 10}, {6, 18}, {7, 12}, {65, 13}});
    areEqual(queue, "[D0:[[(2,10), (7,12), (65,13), {10}], [(1,40), {40}], [(3,60), (4,70), (5,80), {60}], [(8,110), (9,120), (10,130), {110}], [(11,140), (12,150), {140}], [(13,160), (14,170), (15,180), {160}], [(16,190), (17,200), {190}], [(18,210), (19,220), (20,230), {210}]], D1:[[(25,15), (6,16), {21}], [{1000}]], M: 3, B: 1000, size: 22]");
}

void test_insert_mixed_with_existing_keys_and_erase()
{
    int MAX_N = 200;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);
    std::vector<std::pair<int, Ints>> data;

    for (int i = 1; i <= 20; i++)
        data.push_back({i, Ints(i * 10 + 30)});

    queue.insert(data);
    areEqual(queue, "[D0:[[(1,40), (2,50), {40}], [(3,60), (4,70), (5,80), {60}], [(6,90), (7,100), {90}], [(8,110), (9,120), (10,130), {110}], [(11,140), (12,150), {140}], [(13,160), (14,170), (15,180), {160}], [(16,190), (17,200), {190}], [(18,210), (19,220), (20,230), {210}]], D1:[[{1000}]], M: 3, B: 1000, size: 20]");

    queue.insert(25, 15);
    queue.insert(2, 21);
    queue.insert(6, 16);
    queue.insert(7, 28);
    queue.insert(65, 36);
    areEqual(queue, "[D0:[[(1,40), {40}], [(3,60), (4,70), (5,80), {60}], [(8,110), (9,120), (10,130), {110}], [(11,140), (12,150), {140}], [(13,160), (14,170), (15,180), {160}], [(16,190), (17,200), {190}], [(18,210), (19,220), (20,230), {210}]], D1:[[(25,15), (6,16), {21}], [(2,21), (7,28), (65,36), {1000}]], M: 3, B: 1000, size: 22]");

    queue.insert(std::vector<std::pair<int, Ints>>());
    areEqual(queue, "[D0:[[(1,40), {40}], [(3,60), (4,70), (5,80), {60}], [(8,110), (9,120), (10,130), {110}], [(11,140), (12,150), {140}], [(13,160), (14,170), (15,180), {160}], [(16,190), (17,200), {190}], [(18,210), (19,220), (20,230), {210}]], D1:[[(25,15), (6,16), {21}], [(2,21), (7,28), (65,36), {1000}]], M: 3, B: 1000, size: 22]");

    queue.insert(std::vector<std::pair<int, Ints>>{{25, 20}, {2, 10}, {6, 18}, {7, 12}, {65, 13}});
    areEqual(queue, "[D0:[[(2,10), (7,12), (65,13), {10}], [(1,40), {40}], [(3,60), (4,70), (5,80), {60}], [(8,110), (9,120), (10,130), {110}], [(11,140), (12,150), {140}], [(13,160), (14,170), (15,180), {160}], [(16,190), (17,200), {190}], [(18,210), (19,220), (20,230), {210}]], D1:[[(25,15), (6,16), {21}], [{1000}]], M: 3, B: 1000, size: 22]");

    queue.erase(1);
    areEqual(queue, "[D0:[[(2,10), (7,12), (65,13), {10}], [(3,60), (4,70), (5,80), {60}], [(8,110), (9,120), (10,130), {110}], [(11,140), (12,150), {140}], [(13,160), (14,170), (15,180), {160}], [(16,190), (17,200), {190}], [(18,210), (19,220), (20,230), {210}]], D1:[[(25,15), (6,16), {21}], [{1000}]], M: 3, B: 1000, size: 21]");

    queue.erase(13);
    areEqual(queue, "[D0:[[(2,10), (7,12), (65,13), {10}], [(3,60), (4,70), (5,80), {60}], [(8,110), (9,120), (10,130), {110}], [(11,140), (12,150), {140}], [(14,170), (15,180), {160}], [(16,190), (17,200), {190}], [(18,210), (19,220), (20,230), {210}]], D1:[[(25,15), (6,16), {21}], [{1000}]], M: 3, B: 1000, size: 20]");

    queue.erase(14);
    areEqual(queue, "[D0:[[(2,10), (7,12), (65,13), {10}], [(3,60), (4,70), (5,80), {60}], [(8,110), (9,120), (10,130), {110}], [(11,140), (12,150), {140}], [(15,180), {160}], [(16,190), (17,200), {190}], [(18,210), (19,220), (20,230), {210}]], D1:[[(25,15), (6,16), {21}], [{1000}]], M: 3, B: 1000, size: 19]");

    queue.erase(15);
    areEqual(queue, "[D0:[[(2,10), (7,12), (65,13), {10}], [(3,60), (4,70), (5,80), {60}], [(8,110), (9,120), (10,130), {110}], [(11,140), (12,150), {140}], [(16,190), (17,200), {190}], [(18,210), (19,220), (20,230), {210}]], D1:[[(25,15), (6,16), {21}], [{1000}]], M: 3, B: 1000, size: 18]");
}

void test_pull_empty()
{
    int MAX_N = 200;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);
    areEqual(queue.pull(), std::pair<std::vector<int>, Ints>(std::vector<int>(), Ints(1000)));
}

void test_pull()
{
    int MAX_N = 200;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);
    std::vector<std::pair<int, Ints>> data;
    for (int i = 1; i <= 20; i++)
        data.push_back({i, Ints(i * 10 + 30)});
    queue.insert(data);
    queue.insert(25, 9);
    queue.insert(2, 21);
    queue.insert(6, 16);
    queue.insert(7, 28);
    queue.insert(65, 36);
    queue.insert(std::vector<std::pair<int, Ints>>());
    queue.insert(std::vector<std::pair<int, Ints>>{{25, 20}, {2, 10}, {6, 18}, {7, 12}, {65, 13}});
    areEqual(queue, "[D0:[[(2,10), (7,12), (65,13), {10}], [(1,40), {40}], [(3,60), (4,70), (5,80), {60}], [(8,110), (9,120), (10,130), {110}], [(11,140), (12,150), {140}], [(13,160), (14,170), (15,180), {160}], [(16,190), (17,200), {190}], [(18,210), (19,220), (20,230), {210}]], D1:[[(25,9), (6,16), {21}], [{1000}]], M: 3, B: 1000, size: 22]");

    auto answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{25, 2, 7}, Ints(13)));
    areEqual(queue, "[D0:[[(65,13), {10}], [(1,40), {40}], [(3,60), (4,70), (5,80), {60}], [(8,110), (9,120), (10,130), {110}], [(11,140), (12,150), {140}], [(13,160), (14,170), (15,180), {160}], [(16,190), (17,200), {190}], [(18,210), (19,220), (20,230), {210}]], D1:[[(6,16), {21}], [{1000}]], M: 3, B: 1000, size: 19]");

    answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{65, 6, 1}, Ints(60)));
    areEqual(queue, "[D0:[[(3,60), (4,70), (5,80), {60}], [(8,110), (9,120), (10,130), {110}], [(11,140), (12,150), {140}], [(13,160), (14,170), (15,180), {160}], [(16,190), (17,200), {190}], [(18,210), (19,220), (20,230), {210}]], D1:[[{1000}]], M: 3, B: 1000, size: 16]");

    answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{3, 4, 5}, Ints(110)));
    areEqual(queue, "[D0:[[(8,110), (9,120), (10,130), {110}], [(11,140), (12,150), {140}], [(13,160), (14,170), (15,180), {160}], [(16,190), (17,200), {190}], [(18,210), (19,220), (20,230), {210}]], D1:[[{1000}]], M: 3, B: 1000, size: 13]");

    answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{8, 9, 10}, Ints(140)));
    areEqual(queue, "[D0:[[(11,140), (12,150), {140}], [(13,160), (14,170), (15,180), {160}], [(16,190), (17,200), {190}], [(18,210), (19,220), (20,230), {210}]], D1:[[{1000}]], M: 3, B: 1000, size: 10]");

    answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{11, 12, 13}, Ints(170)));
    areEqual(queue, "[D0:[[(14,170), (15,180), {160}], [(16,190), (17,200), {190}], [(18,210), (19,220), (20,230), {210}]], D1:[[{1000}]], M: 3, B: 1000, size: 7]");

    answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{14, 15, 16}, Ints(200)));
    areEqual(queue, "[D0:[[(17,200), {190}], [(18,210), (19,220), (20,230), {210}]], D1:[[{1000}]], M: 3, B: 1000, size: 4]");

    answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{17, 18, 19}, Ints(230)));
    areEqual(queue, "[D0:[[(20,230), {210}]], D1:[[{1000}]], M: 3, B: 1000, size: 1]");

    answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{20}, Ints(1000)));
    areEqual(queue, "[D0:[], D1:[[{1000}]], M: 3, B: 1000, size: 0]");
}

void test_pull_only_d0_indivisible()
{
    int MAX_N = 200;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);

    queue.insert(std::vector<std::pair<int, Ints>>{{6, 60}, {7, 70}, {5, 50}});
    queue.insert(std::vector<std::pair<int, Ints>>{{4, 40}, {3, 30}});
    queue.insert(std::vector<std::pair<int, Ints>>{{2, 20}, {1, 10}});

    areEqual(queue, "[D0:[[(2,20), (1,10), {10}], [(4,40), (3,30), {30}], [(6,60), (7,70), (5,50), {50}]], D1:[[{1000}]], M: 3, B: 1000, size: 7]");

    auto answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{1, 2, 3}, Ints(40)));
    areEqual(queue, "[D0:[[(4,40), {30}], [(6,60), (7,70), (5,50), {50}]], D1:[[{1000}]], M: 3, B: 1000, size: 4]");

    answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{4, 5, 6}, Ints(70)));
    areEqual(queue, "[D0:[[(7,70), {50}]], D1:[[{1000}]], M: 3, B: 1000, size: 1]");

    answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{7}, Ints(1000)));
    areEqual(queue, "[D0:[], D1:[[{1000}]], M: 3, B: 1000, size: 0]");
}

void test_pull_only_d0_divisible()
{
    int MAX_N = 200;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);
    queue.insert(std::vector<std::pair<int, Ints>>{{5, 50}, {4, 40}, {6, 60}});
    queue.insert(std::vector<std::pair<int, Ints>>{{3, 30}, {1, 10}, {2, 20}});

    areEqual(queue, "[D0:[[(3,30), (1,10), (2,20), {10}], [(5,50), (4,40), (6,60), {40}]], D1:[[{1000}]], M: 3, B: 1000, size: 6]");

    auto answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{1, 2, 3}, Ints(40)));
    areEqual(queue, "[D0:[[(5,50), (4,40), (6,60), {40}]], D1:[[{1000}]], M: 3, B: 1000, size: 3]");

    answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{4, 5, 6}, Ints(1000)));
    areEqual(queue, "[D0:[], D1:[[{1000}]], M: 3, B: 1000, size: 0]");
}

void test_pull_only_d1_indivisible()
{
    int MAX_N = 200;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);

    queue.insert(3, 30);
    queue.insert(1, 10);
    queue.insert(2, 20);

    queue.insert(5, 50);

    queue.insert(4, 40);

    areEqual(queue, "[D0:[], D1:[[(1,10), (2,20), {30}], [(3,30), (5,50), (4,40), {1000}]], M: 3, B: 1000, size: 5]");

    auto answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{1, 2, 3}, Ints(40)));
    areEqual(queue, "[D0:[], D1:[[(5,50), (4,40), {1000}]], M: 3, B: 1000, size: 2]");

    answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{4, 5}, Ints(1000)));
    areEqual(queue, "[D0:[], D1:[[{1000}]], M: 3, B: 1000, size: 0]");
}

void test_pull_only_d1_divisible()
{
    int MAX_N = 200;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);
    queue.insert(3, 30);
    queue.insert(1, 10);
    queue.insert(2, 20);
    queue.insert(5, 50);
    queue.insert(4, 40);
    queue.insert(6, 60);

    areEqual(queue, "[D0:[], D1:[[(1,10), (2,20), {30}], [(3,30), (4,40), {50}], [(5,50), (6,60), {1000}]], M: 3, B: 1000, size: 6]");

    auto answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{1, 2, 3}, Ints(40)));
    areEqual(queue, "[D0:[], D1:[[(4,40), {50}], [(5,50), (6,60), {1000}]], M: 3, B: 1000, size: 3]");

    answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{4, 5, 6}, Ints(1000)));
    areEqual(queue, "[D0:[], D1:[[{1000}]], M: 3, B: 1000, size: 0]");
}

void test_pull_smaller_in_d0()
{
    int MAX_N = 200;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);
    queue.insert(std::vector<std::pair<int, Ints>>{{3, 30}, {1, 10}, {2, 20}});
    queue.insert(5, 50);
    queue.insert(4, 40);

    areEqual(queue, "[D0:[[(3,30), (1,10), (2,20), {10}]], D1:[[(5,50), (4,40), {1000}]], M: 3, B: 1000, size: 5]");

    auto answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{1, 2, 3}, Ints(40)));
    areEqual(queue, "[D0:[], D1:[[(5,50), (4,40), {1000}]], M: 3, B: 1000, size: 2]");

    answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{4, 5}, Ints(1000)));
    areEqual(queue, "[D0:[], D1:[[{1000}]], M: 3, B: 1000, size: 0]");
}

void test_pull_smaller_in_d1()
{
    int MAX_N = 200;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);
    queue.insert(2, 20);
    queue.insert(3, 30);
    queue.insert(1, 10);

    queue.insert(std::vector<std::pair<int, Ints>>{{5, 50}, {4, 40}});

    areEqual(queue, "[D0:[[(5,50), (4,40), {40}]], D1:[[(2,20), (3,30), (1,10), {1000}]], M: 3, B: 1000, size: 5]");

    auto answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{1, 2, 3}, Ints(40)));
    areEqual(queue, "[D0:[[(5,50), (4,40), {40}]], D1:[[{1000}]], M: 3, B: 1000, size: 2]");

    answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{4, 5}, Ints(1000)));
    areEqual(queue, "[D0:[], D1:[[{1000}]], M: 3, B: 1000, size: 0]");
}

void test_pull_mixed_d0_and_d1()
{
    int MAX_N = 200;
    std::vector<pairr<pairr<LinkedListNode<pairr<Ints, int>> *, LinkedListNode<pairr<Ints, LinkedList<pairr<Ints, int>>>> *>, bool>> nodes(MAX_N + 1, {{nullptr, nullptr}, false});
    BatchPriorityQueue<Ints> queue(3, 1000, MAX_N, nodes);

    queue.insert(3, 30);
    queue.insert(2, 20);
    queue.insert(6, 60);
    queue.insert(5, 50);

    queue.insert(std::vector<std::pair<int, Ints>>{{8, 80}, {7, 70}});
    queue.insert(std::vector<std::pair<int, Ints>>{{4, 40}, {1, 10}});

    areEqual(queue, "[D0:[[(4,40), (1,10), {10}], [(8,80), (7,70), {70}]], D1:[[(3,30), (2,20), {50}], [(6,60), (5,50), {1000}]], M: 3, B: 1000, size: 8]");

    auto answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{1, 2, 3}, Ints(40)));
    areEqual(queue, "[D0:[[(4,40), {10}], [(8,80), (7,70), {70}]], D1:[[(6,60), (5,50), {1000}]], M: 3, B: 1000, size: 5]");

    answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{4, 5, 6}, Ints(70)));
    areEqual(queue, "[D0:[[(8,80), (7,70), {70}]], D1:[[{1000}]], M: 3, B: 1000, size: 2]");

    answer = queue.pull();
    areEqual(answer, std::pair<std::vector<int>, Ints>(std::vector<int>{7, 8}, Ints(1000)));
    areEqual(queue, "[D0:[], D1:[[{1000}]], M: 3, B: 1000, size: 0]");
}

int main()
{
    std::cout << "--- Starting Batch Priority Queue Unit Tests ---" << std::endl;
    runTest(test_empty_queue, "test_empty_queue");
    runTest(test_insert_different_keys_and_split, "test_insert_different_keys_and_split");
    runTest(test_erase_without_splitting, "test_erase_without_splitting");
    runTest(test_erase, "test_erase");
    runTest(test_key_bound, "test_key_bound");
    runTest(test_is_key, "test_is_key");
    runTest(test_insert_value_bound, "test_insert_value_bound");
    runTest(test_insert_key_bound, "test_insert_key_bound");
    runTest(test_insert_key_bound_on_batch_insert, "test_insert_key_bound_on_batch_insert");
    runTest(test_insert_value_bound_on_batch_insert, "test_insert_value_bound_on_batch_insert");
    runTest(test_insert_batch_simply, "test_insert_batch_simply");
    runTest(test_is_key_with_insert_batch, "test_is_key_with_insert_batch");
    runTest(test_erase_with_insert_batch, "test_erase_with_insert_batch");
    runTest(test_insert_with_existing_key, "test_insert_with_existing_key");
    runTest(test_big_insert_batch, "test_big_insert_batch");
    runTest(test_insert_batch_with_existing_keys, "test_insert_batch_with_existing_keys");
    runTest(test_insert_batch_the_same_keys, "test_insert_batch_the_same_keys");
    runTest(test_insert_mixed_with_existing_keys, "test_insert_mixed_with_existing_keys");
    runTest(test_insert_mixed_with_existing_keys_and_erase, "test_insert_mixed_with_existing_keys_and_erase");
    runTest(test_pull_empty, "test_pull_empty");
    runTest(test_pull, "test_pull");
    runTest(test_pull_only_d0_indivisible, "test_pull_only_d0_indivisible");
    runTest(test_pull_only_d0_divisible, "test_pull_only_d0_divisible");
    runTest(test_pull_only_d1_indivisible, "test_pull_only_d1_indivisible");
    runTest(test_pull_only_d1_divisible, "test_pull_only_d1_divisible");
    runTest(test_pull_smaller_in_d0, "test_pull_smaller_in_d0");
    runTest(test_pull_smaller_in_d1, "test_pull_smaller_in_d1");
    runTest(test_pull_mixed_d0_and_d1, "test_pull_mixed_d0_and_d1");
    std::cout << "--- Batch Priority Queue Tests Finished ---" << std::endl
              << std::endl
              << std::endl;
    return 0;
}