#ifndef BATCH_PRIORITY_QUEUE_H
#define BATCH_PRIORITY_QUEUE_H

#include <vector>
#include <map>

#include "LinkedList.h"
#include "LinkedListNode.h"
#include "utils.h"

template <typename T>
concept HasToStringMethod = requires(const T &t) {
    { t.to_string() } -> std::convertible_to<std::string>;
};

template <typename T>
std::string to_string_helper(const T &val)
{
    if constexpr (HasToStringMethod<T>)
    {
        return val.to_string();
    }
    else
    {
        using std::to_string;
        return to_string(val);
    }
}

template <typename T1, typename T2>
struct pairr : public std::pair<T1, T2>
{
    using std::pair<T1, T2>::pair;

    std::string to_string() const
    {
        return "(" + to_string_helper(this->first) + ", " + to_string_helper(this->second) + ")";
    }
};

template <typename V, bool SafeMode = true>
class BatchPriorityQueue
{
public:
    using InnerList = LinkedList<pairr<V, int>, SafeMode>;
    using OuterList = LinkedList<pairr<V, InnerList>, SafeMode>;
    using InnerNode = LinkedListNode<pairr<V, int>, SafeMode>;
    using OuterNode = LinkedListNode<pairr<V, InnerList>, SafeMode>;

    OuterList D0, D1;
    std::map<V, OuterNode *> value_to_list;
    std::vector<pairr<pairr<InnerNode *, OuterNode *>, bool>> &nodes;

    int M;
    V B;
    int MAX_N;
    int size_ = 0;

    BatchPriorityQueue(
        int M,
        V B,
        int MAX_N,
        std::vector<pairr<pairr<InnerNode *, OuterNode *>, bool>> &nodes)
        : nodes(nodes), M(M), B(B), MAX_N(MAX_N)
    {
        if constexpr (SafeMode)
        {
            if (M < 1)
                throw std::logic_error("Container error: minimal batch size must be greater than 0");
        }
        D1.pushBack({B, InnerList()});
        value_to_list[B] = D1.begin();
    }

    ~BatchPriorityQueue()
    {
        auto clear_pointers = [&](OuterList &list_to_clear)
        {
            auto *current = list_to_clear.begin();
            while (current != list_to_clear.end())
            {
                const auto &list = current->get().second;
                auto *current2 = list.begin();
                while (current2 != list.end())
                {
                    int key = current2->get().second;
                    nodes[key].first.first = nullptr;
                    nodes[key].first.second = nullptr;
                    nodes[key].second = false;
                    current2 = current2->next();
                }
                current = current->next();
            }
        };

        clear_pointers(D0);
        clear_pointers(D1);
    }

    void splitValuedList(OuterNode *valued_list_node, bool list_id)
    {
        int list_size = valued_list_node->get().second.size();
        if (list_size <= M)
            return;

        OuterList &list = list_id ? D1 : D0;

        std::vector<V> elements;
        auto *current = valued_list_node->get().second.begin();
        while (current != valued_list_node->get().second.end())
        {
            elements.push_back(current->get().first);
            current = current->next();
        }

        V split_value = kthValue<V, SafeMode>(elements, list_size / 2 + 1);

        auto *node = new OuterNode();

        splitLessWithNodes(split_value, &(valued_list_node->get().second), node);

        if (list_id)
        {
            node->get().first = split_value;
            value_to_list[split_value] = node;
        }
        else
        {
            node->get().first = valued_list_node->get().first;
            valued_list_node->get().first = split_value;
        }

        list.insertBefore(node, valued_list_node);
        splitValuedList(valued_list_node, list_id);
        splitValuedList(node, list_id);
    }

    void splitLessWithNodes(const V &value, InnerList *original, OuterNode *less_node)
    {
        auto *current = original->begin();
        auto *less = &(less_node->get().second);
        while (current != original->end())
        {
            auto *next = current->next();
            if (current->get().first < value)
            {
                original->cutOff(current);
                less->pushBack(current);
                nodes[current->get().second].first.second = less_node;
            }
            current = next;
        }
    }

    bool isKey(int key)
    {
        if constexpr (SafeMode)
        {
            if (key < 0 || key > (int)MAX_N)
                throw std::logic_error("Pointer overflow: key is out of range.");
        }
        return nodes[key].first.first != nullptr;
    }

    void erase(int key)
    {
        isKey(key);
        if (nodes[key].first.first == nullptr)
            return;

        size_--;

        auto [pointers, list_id] = nodes[key];
        auto [node, valued_list_node] = pointers;

        InnerList &valued_list = valued_list_node->get().second;

        valued_list.erase(node);

        if (valued_list.empty() && valued_list_node->get().first != B)
        {
            if (!list_id)
            {
                D0.erase(valued_list_node);
            }
            else
            {
                V list_value = valued_list_node->get().first;
                D1.erase(valued_list_node);
                value_to_list.erase(list_value);
            }
        }
        nodes[key].first.first = nullptr;
        nodes[key].first.second = nullptr;
        nodes[key].second = false;
    }

    void insert(int key, V value)
    {
        if constexpr (SafeMode)
        {
            if (value > B)
                throw std::logic_error("Cannot insert: inserted object has value over queue's value");
            if (key < 0 || key > (int)MAX_N)
                throw std::logic_error("Pointer overflow: key is out of range.");
        }
        if (isKey(key))
        {
            if (nodes[key].first.first->get().first <= value)
                return;
            erase(key);
        }

        size_++;
        auto it = value_to_list.upper_bound(value);

        if constexpr (SafeMode)
        {
            if (it == value_to_list.end())
                throw std::logic_error("Cannot insert: inserted object has value over queue's value");
        }
        auto *valued_linked_list_node = it->second;

        valued_linked_list_node->get().second.pushBack({value, key});

        nodes[key].first.first = valued_linked_list_node->get().second.backNode();
        nodes[key].first.second = valued_linked_list_node;
        nodes[key].second = true;
        splitValuedList(valued_linked_list_node, true);
    }

    void insert(const std::vector<std::pair<int, V>> &data)
    {
        if (data.size() == 0)
            return;

        auto *valued_list_node = new OuterNode();
        InnerList &valued_list = valued_list_node->get().second;

        V min_value;
        bool find_first = false;
        int size_before = size_;

        for (const auto &p : data)
        {
            int key = p.first;
            V value = p.second;

            if constexpr (SafeMode)
            {
                if (value > B)
                {
                    delete valued_list_node;
                    size_ = size_before;
                    throw std::logic_error("Cannot insert: inserted object has value over queue's value");
                }
                if (key < 0 || key > (int)MAX_N)
                {
                    delete valued_list_node;
                    size_ = size_before;
                    throw std::logic_error("Pointer overflow: key is out of range.");
                }
            }
            if (isKey(key))
            {
                if (!(nodes[key].first.first->get().first < value))
                {
                    if (nodes[key].first.second == valued_list_node) // od tad
                    {
                        valued_list.erase(nodes[key].first.first);
                        size_--;
                    } // Do tad
                    else
                    {
                        erase(key);
                    }
                }
                else
                {
                    continue;
                }
            }
            if (!find_first)
            {
                find_first = true;
                min_value = value;
            }
            size_++;
            if (value < min_value)
                min_value = value;

            pairr<V, int> internal_pair = {value, key};
            auto *node = valued_list.pushBack(internal_pair);
            nodes[key].first.first = node;
            nodes[key].first.second = valued_list_node;
            nodes[key].second = false;
        }

        if (!find_first)
        {
            delete valued_list_node;
            return;
        }

        valued_list_node->get().first = min_value;
        D0.insertBefore(valued_list_node, D0.begin());
        splitValuedList(valued_list_node, false);
    }

    std::pair<std::vector<int>, V> pull()
    {
        std::vector<int> result;
        V bound = B;

        if (size_ <= M)
        {
            auto *current = D1.begin();
            while (current != D1.end())
            {
                const InnerList &valued_list = current->get().second;
                const auto *node = valued_list.begin();
                while (node != valued_list.end())
                {
                    result.push_back(node->get().second);
                    node = node->next();
                }
                current = current->next();
            }
            current = D0.begin();
            while (current != D0.end())
            {
                const InnerList &valued_list = current->get().second;
                const auto *node = valued_list.begin();
                while (node != valued_list.end())
                {
                    result.push_back(node->get().second);
                    node = node->next();
                }
                current = current->next();
            }
        }
        else
        {
            std::vector<pairr<V, int>> pre_result;

            if (D1.size() == 0)
            {
                auto *current = D0.begin();
                while (current != D0.end() && (int)pre_result.size() < M)
                {
                    const InnerList &valued_list = current->get().second;
                    const auto *node = valued_list.begin();
                    while (node != valued_list.end())
                    {
                        pre_result.push_back(node->get());
                        node = node->next();
                    }
                    current = current->next();
                }
                if ((int)pre_result.size() == M)
                {
                    for (const auto &x : pre_result)
                        result.push_back(x.second);
                    if (current == D0.end())
                        bound = B;
                    else
                        bound = current->get().first;
                }
                else
                {
                    bound = kthValue(pre_result, M + 1).first;
                    for (const auto &x : pre_result)
                    {
                        if (x.first < bound)
                        {
                            result.push_back(x.second);
                        }
                    }
                }
            }
            else if (D0.size() == 0)
            {
                auto *current = D1.begin();
                while (current != D1.end() && (int)pre_result.size() < M)
                {
                    const InnerList &valued_list = current->get().second;
                    const auto *node = valued_list.begin();
                    while (node != valued_list.end())
                    {
                        pre_result.push_back(node->get());
                        node = node->next();
                    }
                    bound = current->get().first;
                    current = current->next();
                }

                if ((int)pre_result.size() == M)
                {
                    for (const auto &x : pre_result)
                        result.push_back(x.second);
                }
                else
                {
                    bound = kthValue(pre_result, M + 1).first;
                    for (const auto &x : pre_result)
                    {
                        if (x.first < bound)
                        {
                            result.push_back(x.second);
                        }
                    }
                }
            }
            else
            {
                int D0_count_elements = 0, D1_count_elements = 0;
                auto *current = D0.begin();
                while (current != D0.end() && D0_count_elements <= M)
                {
                    const InnerList &valued_list = current->get().second;
                    const auto *node = valued_list.begin();
                    while (node != valued_list.end())
                    {
                        pre_result.push_back(node->get());
                        node = node->next();
                        D0_count_elements++;
                    }
                    current = current->next();
                }
                current = D1.begin();
                while (current != D1.end() && D1_count_elements <= M)
                {
                    const InnerList &valued_list = current->get().second;
                    const auto *node = valued_list.begin();
                    while (node != valued_list.end())
                    {
                        pre_result.push_back(node->get());
                        node = node->next();
                        D1_count_elements++;
                    }
                    current = current->next();
                }
                bound = kthValue(pre_result, M + 1).first;
                for (const auto &x : pre_result)
                {
                    if (x.first < bound)
                    {
                        result.push_back(x.second);
                    }
                }
            }
        }
        for (const auto &x : result)
            erase(x);
        return {result, size_ == 0 ? B : bound};
    }

    bool empty()
    {
        return size_ == 0;
    }

    std::string to_stringD(const OuterList &D) const
        requires SafeMode
    {
        std::string output = "[";
        auto current = D.begin();
        while (current != D.end())
        {
            auto next = current->next();
            const auto &list = current->get().second;
            const auto &val = current->get().first;

            output += "[";
            auto current2 = list.begin();
            while (current2 != list.end())
            {
                output += "(" + std::to_string(current2->get().second) +
                          "," + current2->get().first.to_string() + "), ";
                current2 = current2->next();
            }
            output += "{" + val.to_string() + "}";
            output += "]";
            if (next != D.end())
                output += ", ";
            current = next;
        }

        output += "]";
        return output;
    }

    std::string to_string() const
        requires SafeMode
    {
        std::string output = "[D0:";
        output += to_stringD(D0);
        output += ", D1:";
        output += to_stringD(D1);
        output += ", M: " + std::to_string(M) + ", B: " + B.to_string() + ", size: " + std::to_string(size_) + "]";
        return output;
    }

    std::string get_D1_values_string() const
        requires SafeMode
    {
        std::string output = "[";
        int i = 0;
        for (const auto &[value, node] : value_to_list)
        {
            if (i != 0)
                output += ", ";
            output += "{" + value.to_string() + "}";
            i++;
        }
        output += "]";
        return output;
    }
};

#endif // BATCH_PRIORITY_QUEUE_H