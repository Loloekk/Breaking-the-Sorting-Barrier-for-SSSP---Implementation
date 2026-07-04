#ifndef INDEXED_PRIORITY_QUEUE_HPP
#define INDEXED_PRIORITY_QUEUE_HPP

#include <vector>
#include <utility>
#include <stdexcept>
template <typename V, bool SafeMode = true>
class IndexedPriorityQueue
{
    std::vector<int> &pointers;
    std::vector<std::pair<int, V>> heap;

public:
    IndexedPriorityQueue(std::vector<int> &pointers) : pointers(pointers) {}

    ~IndexedPriorityQueue()
    {
        for (auto [key, value] : heap)
            pointers[key] = -1;
    }

    void updateUpper(int pointer)
    {
        while (pointer != 0 && heap[pointer].second < heap[(pointer - 1) / 2].second)
        {
            int key1 = heap[(pointer - 1) / 2].first;
            int key2 = heap[pointer].first;
            std::swap(heap[(pointer - 1) / 2], heap[pointer]);
            std::swap(pointers[key1], pointers[key2]);
            pointer = (pointer - 1) / 2;
        }
    }

    void updateLower(int pointer)
    {
        while (true)
        {
            int pointer2 = pointer;
            if (pointer * 2 + 1 < (int)heap.size() && heap[pointer * 2 + 1].second < heap[pointer2].second)
            {
                pointer2 = pointer * 2 + 1;
            }
            if (pointer * 2 + 2 < (int)heap.size() && heap[pointer * 2 + 2].second < heap[pointer2].second)
            {
                pointer2 = pointer * 2 + 2;
            }
            if (pointer == pointer2)
                break;
            int key1 = heap[pointer].first;
            int key2 = heap[pointer2].first;
            std::swap(heap[pointer], heap[pointer2]);
            std::swap(pointers[key1], pointers[key2]);
            pointer = pointer2;
        }
    }

    void set_value(int key, const V &value)
    {
        if constexpr (SafeMode)
        {
            if (key >= (int)pointers.size())
                throw std::out_of_range("Cannot set_value: key is too high..");
        }
        if (pointers[key] == -1)
        {
            pointers[key] = heap.size();
            heap.push_back({key, value});
        }
        else
        {
            heap[pointers[key]].second = value;
        }
        updateLower(pointers[key]);
        updateUpper(pointers[key]);
    }
    const std::pair<int, V> &top() const
    {
        if constexpr (SafeMode)
        {
            if (heap.empty())
                throw std::out_of_range("Cannot access top: Queue is empty.");
        }
        return heap[0];
    }
    void pop()
    {
        if constexpr (SafeMode)
        {
            if (heap.empty())
                throw std::out_of_range("Cannot access top: Queue is empty.");
        }
        pointers[heap[0].first] = -1;
        if (heap.size() == 1)
        {
            heap.pop_back();
            return;
        }
        heap[0] = heap[heap.size() - 1];
        int key = heap[0].first;
        pointers[key] = 0;
        heap.pop_back();
        updateLower(0);
    }
    bool empty() const
    {
        return heap.empty();
    }
};

#endif /*INDEXED_PRIORITY_QUEUE_HPP*/