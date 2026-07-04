#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdexcept>
#include <utility>
#include <cassert>
#include <vector>
#include <iostream>

#include "LinkedListNode.h"

template <typename V, bool SafeMode = true>
class LinkedList
{
private:
    LinkedListNode<V, SafeMode> *head = nullptr;
    LinkedListNode<V, SafeMode> *tail = nullptr;
    int size_ = 0;

public:
    LinkedList()
    {
        head = new LinkedListNode<V, SafeMode>(nullptr, nullptr);
        tail = new LinkedListNode<V, SafeMode>(nullptr, nullptr);
        head->next_node = tail;
        tail->prev_node = head;
    }

    LinkedList(std::vector<V> vector) : LinkedList()
    {
        for (const V &value : vector)
            this->pushBack(value);
    }

    virtual ~LinkedList()
    {
        LinkedListNode<V, SafeMode> *current = this->begin();
        while (current != this->end())
        {
            LinkedListNode<V, SafeMode> *next = current->next_node;
            erase(current);
            current = next;
        }
        delete head;
        delete tail;
    }

    LinkedList(const LinkedList<V, SafeMode> &other) : LinkedList()
    {
        const LinkedListNode<V, SafeMode> *current = other.begin();
        while (current != other.end())
        {
            this->pushBack(current->get());
            current = current->next_node;
        }
    }
    LinkedList<V, SafeMode> &operator=(LinkedList<V, SafeMode> other)
    {
        std::swap(this->head, other.head);
        std::swap(this->tail, other.tail);
        std::swap(this->size_, other.size_);

        return *this;
    }

    LinkedList(LinkedList<V, SafeMode> &&list) : LinkedList()
    {
        std::swap(list.head, this->head);
        std::swap(list.tail, this->tail);
        std::swap(list.size_, this->size_);
    }

    LinkedListNode<V, SafeMode> *begin()
    {
        return head->next_node;
    }
    const LinkedListNode<V, SafeMode> *begin() const
    {
        return head->next_node;
    }
    LinkedListNode<V, SafeMode> *end()
    {
        return tail;
    }
    const LinkedListNode<V, SafeMode> *end() const
    {
        return tail;
    }
    void pushBack(LinkedListNode<V, SafeMode> *node)
    {
        node->prev_node = tail->prev_node;
        node->next_node = tail;
        tail->prev_node->next_node = node;
        tail->prev_node = node;
        size_++;
    }
    LinkedListNode<V, SafeMode> *pushBack(V value)
    {
        LinkedListNode<V, SafeMode> *node = new LinkedListNode<V, SafeMode>(std::move(value));
        this->pushBack(node);
        return node;
    }
    void pushFront(LinkedListNode<V, SafeMode> *node)
    {
        node->next_node = head->next_node;
        node->prev_node = head;
        head->next_node->prev_node = node;
        head->next_node = node;
        size_++;
    }
    LinkedListNode<V, SafeMode> *pushFront(V value)
    {
        LinkedListNode<V, SafeMode> *node = new LinkedListNode<V, SafeMode>(std::move(value));
        this->pushFront(node);
        return node;
    }
    int size() const
    {
        return size_;
    }
    bool empty() const
    {
        return size_ == 0;
    }
    void cutOff(LinkedListNode<V, SafeMode> *node)
    {
        if constexpr (SafeMode)
        {
            if (node->isHead() || node->isTail())
                throw std::out_of_range("Cannot cut_off: node is head or tail.");
        }
        node->prev_node->next_node = node->next_node;
        node->next_node->prev_node = node->prev_node;
        node->next_node = nullptr;
        node->prev_node = nullptr;
        size_--;
    }
    void erase(LinkedListNode<V, SafeMode> *node)
    {
        if constexpr (SafeMode)
        {
            if (node->isHead() || node->isTail())
                throw std::out_of_range("Cannot erase: node is head or tail.");
        }
        LinkedListNode<V, SafeMode> *prev = node->prev_node;
        LinkedListNode<V, SafeMode> *next = node->next_node;
        size_--;
        prev->next_node = next;
        next->prev_node = prev;
        delete node;
    }
    void popFront()
    {
        if constexpr (SafeMode)
        {
            if (size_ == 0)
                throw std::out_of_range("Cannot pop_front: LinkedList is empty.");
        }
        this->erase(this->begin());
    }
    void popBack()
    {
        if constexpr (SafeMode)
        {
            if (size_ == 0)
                throw std::out_of_range("Cannot pop_back: LinkedList is empty.");
        }
        this->erase(this->end()->prev_node);
    }
    V &front()
    {
        if constexpr (SafeMode)
        {
            if (size_ == 0)
                throw std::out_of_range("Cannot access front: LinkedList is empty.");
        }
        return this->begin()->get();
    }
    const V &front() const
    {
        if constexpr (SafeMode)
        {
            if (size_ == 0)
                throw std::out_of_range("Cannot access front: LinkedList is empty.");
        }
        return this->begin()->get();
    }
    V &back()
    {
        if constexpr (SafeMode)
        {
            if (size_ == 0)
                throw std::out_of_range("Cannot access back: LinkedList is empty.");
        }
        return this->end()->prev_node->get();
    }
    const V &back() const
    {
        if constexpr (SafeMode)
        {
            if (size_ == 0)
                throw std::out_of_range("Cannot access back: LinkedList is empty.");
        }
        return this->end()->prev_node->get();
    }
    LinkedListNode<V, SafeMode> *backNode()
    {
        if constexpr (SafeMode)
        {
            if (size_ == 0)
                throw std::out_of_range("Cannot access back: LinkedList is empty.");
        }
        return this->end()->prev_node;
    }
    LinkedListNode<V, SafeMode> *frontNode()
    {
        if constexpr (SafeMode)
        {
            if (size_ == 0)
                throw std::out_of_range("Cannot access front: LinkedList is empty.");
        }
        return this->begin();
    }
    const LinkedListNode<V, SafeMode> *backNode() const
    {
        if constexpr (SafeMode)
        {
            if (size_ == 0)
                throw std::out_of_range("Cannot access back: LinkedList is empty.");
        }
        return this->end()->prev_node;
    }
    const LinkedListNode<V, SafeMode> *frontNode() const
    {
        if constexpr (SafeMode)
        {
            if (size_ == 0)
                throw std::out_of_range("Cannot access front: LinkedList is empty.");
        }
        return this->begin();
    }
    const LinkedListNode<V, SafeMode> *find(const V &value) const
    {
        const LinkedListNode<V, SafeMode> *current = this->begin();
        while (current != this->end())
        {
            if (current->get() == value)
                return current;
            current = current->next_node;
        }
        return nullptr;
    }
    void merge(LinkedList<V, SafeMode> *list)
    {
        if (list->size() == 0 || list == this)
            return;
        size_ += list->size_;
        list->size_ = 0;
        tail->prev_node->next_node = list->head->next_node;
        list->head->next_node->prev_node = tail->prev_node;
        tail->prev_node = list->tail->prev_node;
        list->tail->prev_node->next_node = tail;
        list->head->next_node = list->tail;
        list->tail->prev_node = list->head;
    }
    void insertBefore(LinkedListNode<V, SafeMode> *value_node, LinkedListNode<V, SafeMode> *node)
    {

        node->prev_node->next_node = value_node;
        value_node->prev_node = node->prev_node;
        value_node->next_node = node;
        node->prev_node = value_node;
        size_++;
    }

    LinkedListNode<V, SafeMode> *insertBefore(V value, LinkedListNode<V, SafeMode> *node)
    {
        LinkedListNode<V, SafeMode> *value_node = new LinkedListNode<V, SafeMode>(std::move(value));
        this->insertBefore(value_node, node);
        return value_node;
    }

    void splitLess(const V &value, LinkedList<V> *less)
    {
        LinkedListNode<V, SafeMode> *current = this->begin();
        while (current != this->end())
        {
            LinkedListNode<V, SafeMode> *next = current->next_node;
            if (current->get() < value)
            {
                this->cutOff(current);
                less->pushBack(current);
            }
            current = next;
        }
    }
    void splitLessOrEqual(const V &value, LinkedList<V, SafeMode> *less_or_equal)
    {
        LinkedListNode<V, SafeMode> *current = this->begin();
        while (current != this->end())
        {
            LinkedListNode<V, SafeMode> *next = current->next_node;
            if (!(value < current->get()))
            {
                this->cutOff(current);
                less_or_equal->pushBack(current);
            }
            current = next;
        }
    }
    std::vector<V> getVector() const
        requires SafeMode
    {
        std::vector<V> output(size_);
        const LinkedListNode<V, SafeMode> *current = this->begin();
        int i = 0;
        while (current != this->end())
        {
            output[i] = current->get();
            current = current->next_node;
            i++;
        }
        return output;
    }

    std::vector<V> getValuesVector() const
        requires SafeMode
    {
        std::vector<V> output(this->size_);
        const LinkedListNode<V, SafeMode> *current = this->begin();
        int i = 0;
        while (current != this->end())
        {
            output[i] = current->get();
            current = current->next_node;
            i++;
        }
        return output;
    }

    std::string to_string() const
        requires SafeMode
    {
        std::string output = "[";
        const LinkedListNode<V, SafeMode> *current = this->begin();
        while (current != this->end())
        {
            const LinkedListNode<V, SafeMode> *next = current->next_node;
            output += current->get().to_string();
            if (next != this->end())
                output += ", ";
            current = next;
        }
        output += "]";
        return output;
    }
};

#endif // LINKED_LIST_H