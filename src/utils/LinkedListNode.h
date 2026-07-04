#ifndef LINKED_LIST_NODE_H
#define LINKED_LIST_NODE_H

#include <utility>
#include <stdexcept>

template <typename V, bool SafeMode>
class LinkedList;

template <typename V, bool SafeMode = true>
class LinkedListNode
{
private:
    V data;
    LinkedListNode<V, SafeMode> *prev_node;
    LinkedListNode<V, SafeMode> *next_node;

public:
    LinkedListNode(LinkedListNode<V, SafeMode> *prev, LinkedListNode<V, SafeMode> *next) : prev_node(prev), next_node(next) {}
    LinkedListNode(V value, LinkedListNode<V, SafeMode> *prev, LinkedListNode<V, SafeMode> *next) : data(std::move(value)), prev_node(prev), next_node(next) {}
    LinkedListNode(V value) : data(std::move(value)), prev_node(this), next_node(this) {}
    LinkedListNode() : prev_node(this), next_node(this) {}
    bool isHead() const
    {
        return prev_node == nullptr;
    }
    bool isTail() const
    {
        return next_node == nullptr;
    }
    V &get()
    {
        if constexpr (SafeMode)
        {
            if (isHead())
                throw std::logic_error("Access denied: Attempted to read data from the sentinel head node.");
            if (isTail())
                throw std::logic_error("Access denied: Attempted to read data from the sentinel tail node.");
        }
        return data;
    }
    const V &get() const
    {
        if constexpr (SafeMode)
        {
            if (isHead())
                throw std::logic_error("Access denied: Attempted to read data from the sentinel head node.");
            if (isTail())
                throw std::logic_error("Access denied: Attempted to read data from the sentinel tail node.");
        }
        return data;
    }
    LinkedListNode<V, SafeMode> *next()
    {
        if constexpr (SafeMode)
        {
            if (isTail())
                throw std::out_of_range("Pointer overflow: next node doesn't exist.");
        }
        return next_node;
    }

    LinkedListNode<V, SafeMode> *prev()
    {
        if constexpr (SafeMode)
        {
            if (isHead())
                throw std::out_of_range("Pointer overflow: previous node doesn't exist.");
            if (prev_node->isHead())
                throw std::out_of_range("Pointer overflow: previous node doesn't exist.");
        }
        return prev_node;
    }

    const LinkedListNode<V, SafeMode> *next() const
    {
        if constexpr (SafeMode)
        {
            if (isTail())
                throw std::out_of_range("Pointer overflow: next node doesn't exist.");
        }
        return next_node;
    }

    const LinkedListNode<V, SafeMode> *prev() const
    {
        if constexpr (SafeMode)
        {
            if (isHead())
                throw std::out_of_range("Pointer overflow: previous node doesn't exist.");
            if (prev_node->isHead())
                throw std::out_of_range("Pointer overflow: previous node doesn't exist.");
        }
        return prev_node;
    }

    friend class LinkedList<V, SafeMode>;
};

#endif // LINKED_LIST_NODE_H