#include <iostream>
#include <string>
#include <functional>
#include <utility>
#include "LinkedList.h"
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
    bool operator<(const Ints &other)
    {
        return value_ < other.value_;
    }
    operator int() const
    {
        return value_;
    }
};

template <typename V>
void areEqual(const LinkedList<V> &list, const std::vector<V> &vector)
{
    if (list.getVector() != vector)
        throw std::runtime_error("List is uncorrect");
    if ((int)list.size() != (int)vector.size())
        throw std::runtime_error("List is uncorrect");
    if (list.empty() != vector.empty())
        throw std::runtime_error("List is uncorrect");
}

LinkedList<Ints> getDefaultList()
{
    LinkedList<Ints> list;
    list.pushFront(5);
    areEqual(list, {5});
    list.pushBack(6);
    areEqual(list, {5, 6});
    list.pushFront(4);
    areEqual(list, {4, 5, 6});
    list.pushBack(7);
    areEqual(list, {4, 5, 6, 7});
    list.pushFront(3);
    areEqual(list, {3, 4, 5, 6, 7});
    list.pushBack(8);
    areEqual(list, {3, 4, 5, 6, 7, 8});
    list.pushFront(2);
    areEqual(list, {2, 3, 4, 5, 6, 7, 8});
    list.pushBack(9);
    areEqual(list, {2, 3, 4, 5, 6, 7, 8, 9});
    list.pushFront(1);
    areEqual(list, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    list.pushBack(10);
    areEqual(list, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    return list;
}

std::vector<Ints> getDefaultVector()
{
    return {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
}

std::vector<LinkedListNode<Ints> *> getPointers(LinkedList<Ints> &list)
{
    std::vector<LinkedListNode<Ints> *> pointers(list.size());
    int i = 0;
    LinkedListNode<Ints> *current = list.begin();
    while (current != list.end())
    {
        pointers[i] = current;
        i++;
        current = current->next();
    }
    return pointers;
}

void test_empty_list()
{
    LinkedList<Ints> list;
    areEqual(list, {});
}

void test_move_constructor()
{
    LinkedList<Ints> list = getDefaultList();
    LinkedList<Ints> list2 = std::move(list);
    areEqual(list2, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    areEqual(list, {});
}

void test_push_back_and_get_vector()
{
    LinkedList<Ints> list;
    list.pushBack(1);
    list.pushBack(6);
    list.pushBack(2);
    list.pushBack(3);
    list.pushBack(4);
    list.pushBack(3);
    areEqual(list, {1, 6, 2, 3, 4, 3});
}

void test_push_back_and_get_values_vector()
{
    LinkedList<Ints> list;
    list.pushBack(1);
    list.pushBack(6);
    list.pushBack(2);
    list.pushBack(3);
    list.pushBack(4);
    list.pushBack(3);
    std::vector<Ints> correct_answear = {1, 6, 2, 3, 4, 3};
    std::vector<Ints> answear = list.getValuesVector();
    areEqual(list, {1, 6, 2, 3, 4, 3});
    if (correct_answear.size() != answear.size())
        throw std::runtime_error("List is uncorrect");
    if (correct_answear != answear)
        throw std::runtime_error("List is uncorrect");
}

void test_push_back_and_get_values_vector_1element()
{
    LinkedList<Ints> list;
    list.pushBack(1);
    std::vector<Ints> correct_answear = {1};
    std::vector<Ints> answear = list.getValuesVector();
    areEqual(list, {1});
    if (correct_answear.size() != answear.size())
        throw std::runtime_error("List is uncorrect");
    if (correct_answear != answear)
        throw std::runtime_error("List is uncorrect");
}

void test_push_back_and_get_values_vector_empty()
{
    LinkedList<Ints> list;
    std::vector<Ints> correct_answear = {};
    std::vector<Ints> answear = list.getValuesVector();
    areEqual(list, {});
    if (correct_answear.size() != answear.size())
        throw std::runtime_error("List is uncorrect");
    if (correct_answear != answear)
        throw std::runtime_error("List is uncorrect");
}

void test_vector_constructor()
{
    LinkedList<Ints> list(getDefaultVector());
    areEqual(list, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
}

void test_push_front()
{
    LinkedList<Ints> list;
    list.pushFront(1);
    list.pushFront(6);
    list.pushFront(2);
    list.pushFront(3);
    list.pushFront(4);
    list.pushFront(3);
    areEqual(list, {3, 4, 3, 2, 6, 1});
}

void test_push_1()
{
    LinkedList<Ints> list;
    list.pushFront(1);
    list.pushFront(6);
    list.pushFront(2);
    list.pushFront(3);
    list.pushFront(4);
    list.pushFront(3);
    list.pushBack(1);
    list.pushBack(6);
    list.pushBack(2);
    list.pushBack(3);
    list.pushBack(4);
    list.pushBack(3);
    areEqual(list, {3, 4, 3, 2, 6, 1, 1, 6, 2, 3, 4, 3});
}

void test_push_2()
{
    LinkedList<Ints> list;
    list.pushFront(1);
    list.pushBack(1);
    list.pushFront(6);
    list.pushBack(6);
    list.pushFront(2);
    list.pushBack(2);
    list.pushFront(3);
    list.pushBack(3);
    list.pushFront(4);
    list.pushBack(4);
    list.pushFront(3);
    list.pushBack(3);
    areEqual(list, {3, 4, 3, 2, 6, 1, 1, 6, 2, 3, 4, 3});
}

void test_front_back()
{
    LinkedList<Ints> list;
    list.pushFront(1);
    if (list.front() != 1)
        throw std::runtime_error("List is uncorrect");
    list.pushBack(1);
    if (list.back() != 1)
        throw std::runtime_error("List is uncorrect");
    list.pushFront(6);
    if (list.front() != 6)
        throw std::runtime_error("List is uncorrect");
    list.pushBack(6);
    if (list.back() != 6)
        throw std::runtime_error("List is uncorrect");
    list.pushFront(2);
    if (list.front() != 2)
        throw std::runtime_error("List is uncorrect");
    list.pushBack(2);
    if (list.back() != 2)
        throw std::runtime_error("List is uncorrect");
    list.pushFront(3);
    if (list.front() != 3)
        throw std::runtime_error("List is uncorrect");
    list.pushBack(3);
    if (list.back() != 3)
        throw std::runtime_error("List is uncorrect");
    list.pushFront(4);
    if (list.front() != 4)
        throw std::runtime_error("List is uncorrect");
    list.pushBack(4);
    if (list.back() != 4)
        throw std::runtime_error("List is uncorrect");
    list.pushFront(3);
    if (list.front() != 3)
        throw std::runtime_error("List is uncorrect");
    list.pushBack(3);
    if (list.back() != 3)
        throw std::runtime_error("List is uncorrect");
    areEqual(list, {3, 4, 3, 2, 6, 1, 1, 6, 2, 3, 4, 3});
}

void test_front_node_back_node()
{
    LinkedList<Ints> list;
    list.pushFront(1);
    if (list.frontNode()->get() != 1)
        throw std::runtime_error("List is uncorrect");
    list.pushBack(1);
    if (list.backNode()->get() != 1)
        throw std::runtime_error("List is uncorrect");
    list.pushFront(6);
    if (list.frontNode()->get() != 6)
        throw std::runtime_error("List is uncorrect");
    list.pushBack(6);
    if (list.backNode()->get() != 6)
        throw std::runtime_error("List is uncorrect");
    list.pushFront(2);
    if (list.frontNode()->get() != 2)
        throw std::runtime_error("List is uncorrect");
    list.pushBack(2);
    if (list.backNode()->get() != 2)
        throw std::runtime_error("List is uncorrect");
    list.pushFront(3);
    if (list.frontNode()->get() != 3)
        throw std::runtime_error("List is uncorrect");
    list.pushBack(3);
    if (list.backNode()->get() != 3)
        throw std::runtime_error("List is uncorrect");
    list.pushFront(4);
    if (list.frontNode()->get() != 4)
        throw std::runtime_error("List is uncorrect");
    list.pushBack(4);
    if (list.backNode()->get() != 4)
        throw std::runtime_error("List is uncorrect");
    list.pushFront(3);
    if (list.frontNode()->get() != 3)
        throw std::runtime_error("List is uncorrect");
    list.pushBack(3);
    if (list.backNode()->get() != 3)
        throw std::runtime_error("List is uncorrect");
    areEqual(list, {3, 4, 3, 2, 6, 1, 1, 6, 2, 3, 4, 3});
}

void test_operator_equality()
{
    LinkedList<Ints> list;
    LinkedList<Ints> list2 = list;
    LinkedList<Ints> list3;
    areEqual(list2, {});
    list3 = list;
    areEqual(list3, {});

    list.pushFront(1);
    list.pushFront(6);
    list.pushFront(2);
    list.pushFront(3);
    list.pushFront(4);
    list.pushFront(3);
    list.pushBack(1);
    list.pushBack(6);
    list.pushBack(2);
    list.pushBack(3);
    list.pushBack(4);
    list.pushBack(3);
    LinkedList<Ints> list4 = list;
    list2 = list;
    areEqual(list, {3, 4, 3, 2, 6, 1, 1, 6, 2, 3, 4, 3});
    areEqual(list2, {3, 4, 3, 2, 6, 1, 1, 6, 2, 3, 4, 3});
    areEqual(list4, {3, 4, 3, 2, 6, 1, 1, 6, 2, 3, 4, 3});
}

void test_default_list()
{
    areEqual(getDefaultList(), {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
}

void test_pop_back()
{
    LinkedList<Ints> list = getDefaultList();
    std::vector<Ints> vector = getDefaultVector();
    while (!list.empty())
    {
        list.popBack();
        vector.pop_back();
        areEqual(list, vector);
    }
    areEqual(list, {});
}

void test_pop_back_exception_func()
{
    LinkedList<Ints> list = getDefaultList();
    while (true)
    {
        list.popBack();
    }
}
void test_pop_back_exception()
{
    runExceptionTest<std::out_of_range>(
        test_pop_back_exception_func,
        "Cannot pop_back: LinkedList is empty.");
}

void test_pop_front()
{
    LinkedList<Ints> list = getDefaultList();
    std::vector<Ints> vector = getDefaultVector();
    while (!list.empty())
    {
        list.popFront();
        vector.erase(vector.begin());
        areEqual(list, vector);
    }
    areEqual(list, {});
}

void test_pop_front_exception_func()
{
    LinkedList<Ints> list = getDefaultList();
    while (true)
    {
        list.popFront();
    }
}
void test_pop_front_exception()
{
    runExceptionTest<std::out_of_range>(
        test_pop_front_exception_func,
        "Cannot pop_front: LinkedList is empty.");
}

void test_begin_end()
{
    LinkedList<Ints> list = getDefaultList();
    std::vector<Ints> vector = getDefaultVector();
    LinkedListNode<Ints> *current = list.begin();
    int i = 0;
    while (current != list.end())
    {
        if (vector[i] != current->get())
            throw std::runtime_error("Error durigniteration");
        i++;
        current = current->next();
    }
}

void test_cut_off()
{
    LinkedList<Ints> list = getDefaultList();
    std::vector<LinkedListNode<Ints> *> pointers = getPointers(list);
    list.cutOff(pointers[5]);
    delete pointers[5];
    areEqual(list, {1, 2, 3, 4, 5, 7, 8, 9, 10});
    list.cutOff(pointers[0]);
    delete pointers[0];
    areEqual(list, {2, 3, 4, 5, 7, 8, 9, 10});
    list.cutOff(pointers[9]);
    delete pointers[9];
    areEqual(list, {2, 3, 4, 5, 7, 8, 9});
}

void test_cut_off_exception_func()
{
    LinkedList<Ints> list = getDefaultList();
    list.cutOff(list.end());
}
void test_cut_off_exception()
{
    runExceptionTest<std::out_of_range>(test_cut_off_exception_func, "Cannot cut_off: node is head or tail.");
}

void test_erase()
{
    LinkedList<Ints> list = getDefaultList();
    std::vector<LinkedListNode<Ints> *> pointers = getPointers(list);
    list.erase(pointers[5]);
    areEqual(list, {1, 2, 3, 4, 5, 7, 8, 9, 10});
    list.erase(pointers[0]);
    areEqual(list, {2, 3, 4, 5, 7, 8, 9, 10});
    list.erase(pointers[9]);
    areEqual(list, {2, 3, 4, 5, 7, 8, 9});
}

void test_merge()
{
    LinkedList<Ints> list = getDefaultList();
    LinkedList<Ints> list2(list);
    LinkedList<Ints> empty1;
    LinkedList<Ints> empty2;
    list.merge(&list2);
    areEqual(list, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    areEqual(list2, {});
    list.merge(&empty1);
    areEqual(list, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    areEqual(empty1, {});
    empty2.merge(&list);
    areEqual(empty2, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    areEqual(list, {});
}

void test_insert_before_node()
{
    LinkedList<Ints> list = getDefaultList();
    std::vector<LinkedListNode<Ints> *> pointers = getPointers(list);
    list.cutOff(pointers[7]);
    list.cutOff(pointers[8]);
    list.cutOff(pointers[9]);
    list.insertBefore(pointers[9], pointers[0]);
    list.insertBefore(pointers[8], pointers[5]);
    list.insertBefore(pointers[7], list.end());
    areEqual(list, {10, 1, 2, 3, 4, 5, 9, 6, 7, 8});
}

void test_insert_before_value()
{
    LinkedList<Ints> list = getDefaultList();
    std::vector<LinkedListNode<Ints> *> pointers = getPointers(list);
    LinkedListNode<Ints> *p1 = list.insertBefore(13, pointers[0]);
    LinkedListNode<Ints> *p2 = list.insertBefore(12, pointers[5]);
    LinkedListNode<Ints> *p3 = list.insertBefore(14, list.end());
    areEqual(list, {13, 1, 2, 3, 4, 5, 12, 6, 7, 8, 9, 10, 14});
    list.cutOff(p1);
    list.cutOff(p2);
    list.cutOff(p3);
    areEqual(list, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    delete p1;
    delete p2;
    delete p3;
}

void test_insert_before_value_empty_list()
{
    LinkedList<Ints> list;
    list.insertBefore(1, list.end());
    list.insertBefore(2, list.end());
    list.insertBefore(3, list.end());
    areEqual(list, {1, 2, 3});
}

void test_split_less()
{
    LinkedList<Ints> list = getDefaultList();
    LinkedList<Ints> list2 = list;
    list.merge(&list2);
    LinkedList<Ints> less;
    list.splitLess(5, &less);
    areEqual(list, {5, 6, 7, 8, 9, 10, 5, 6, 7, 8, 9, 10});
    areEqual(less, {1, 2, 3, 4, 1, 2, 3, 4});
}

void test_split_less_empty()
{
    LinkedList<Ints> list = getDefaultList();
    LinkedList<Ints> less;
    list.splitLess(0, &less);
    areEqual(list, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    areEqual(less, {});
}

void test_split_less_all()
{
    LinkedList<Ints> list = getDefaultList();
    LinkedList<Ints> less;
    list.splitLess(11, &less);
    areEqual(list, {});
    areEqual(less, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
}

void test_split_empty_list()
{
    LinkedList<Ints> list;
    LinkedList<Ints> less;
    list.splitLess(5, &less);
    areEqual(list, {});
    areEqual(less, {});
}

void test_split_less_all_equal()
{
    LinkedList<Ints> list;
    list.pushBack(5);
    list.pushBack(5);
    list.pushBack(5);
    LinkedList<Ints> less;
    list.splitLess(5, &less);
    areEqual(list, {5, 5, 5});
    areEqual(less, {});
}

void test_split_less_and_equal()
{
    LinkedList<Ints> list = getDefaultList();
    LinkedList<Ints> list2 = list;
    list.merge(&list2);
    LinkedList<Ints> less;
    list.splitLessOrEqual(5, &less);
    areEqual(list, {6, 7, 8, 9, 10, 6, 7, 8, 9, 10});
    areEqual(less, {1, 2, 3, 4, 5, 1, 2, 3, 4, 5});
}

void test_split_less_and_equal_empty_list()
{
    LinkedList<Ints> list;
    LinkedList<Ints> less;
    list.splitLessOrEqual(5, &less);
    areEqual(list, {});
    areEqual(less, {});
}

void test_split_less_and_equal_empty_less()
{
    LinkedList<Ints> list;
    list.pushBack(5);
    list.pushBack(5);
    list.pushBack(5);
    list.pushBack(6);
    list.pushBack(6);
    list.pushBack(6);
    list.pushBack(7);
    list.pushBack(7);
    list.pushBack(7);
    LinkedList<Ints> less;
    list.splitLessOrEqual(5, &less);
    areEqual(list, {6, 6, 6, 7, 7, 7});
    areEqual(less, {5, 5, 5});
}

void test_split_less_and_equal_empty_equal()
{
    LinkedList<Ints> list;
    list.pushBack(5);
    list.pushBack(5);
    list.pushBack(5);
    list.pushBack(6);
    list.pushBack(6);
    list.pushBack(6);
    list.pushBack(8);
    list.pushBack(8);
    list.pushBack(8);
    LinkedList<Ints> less;
    list.splitLessOrEqual(7, &less);
    areEqual(list, {8, 8, 8});
    areEqual(less, {5, 5, 5, 6, 6, 6});
}

void test_split_less_and_equal_empty_equal_and_less()
{
    LinkedList<Ints> list;
    list.pushBack(5);
    list.pushBack(5);
    list.pushBack(5);
    list.pushBack(6);
    list.pushBack(6);
    list.pushBack(6);
    list.pushBack(8);
    list.pushBack(8);
    list.pushBack(8);
    LinkedList<Ints> less;
    list.splitLessOrEqual(4, &less);
    areEqual(list, {5, 5, 5, 6, 6, 6, 8, 8, 8});
    areEqual(less, {});
}

void test_to_string_empty()
{
    LinkedList<Ints> list;
    if (list.to_string() != "[]")
        throw std::runtime_error("List is uncorrect");
}

void test_to_string()
{
    LinkedList<Ints> list = getDefaultList();
    LinkedList<Ints> list2 = list;
    list.merge(&list2);
    LinkedList<Ints> less;
    list.splitLessOrEqual(5, &less);
    areEqual(list, {6, 7, 8, 9, 10, 6, 7, 8, 9, 10});
    areEqual(less, {1, 2, 3, 4, 5, 1, 2, 3, 4, 5});
    if (list.to_string() != "[6, 7, 8, 9, 10, 6, 7, 8, 9, 10]")
        throw std::runtime_error("List is uncorrect");
    if (less.to_string() != "[1, 2, 3, 4, 5, 1, 2, 3, 4, 5]")
        throw std::runtime_error("List is uncorrect");
}

int main()
{
    std::cout << "--- Starting Linked List Unit Tests ---" << std::endl;

    runTest(test_empty_list, "test_empty_list");
    runTest(test_move_constructor, "test_move_constructor");
    runTest(test_push_back_and_get_vector, "test_push_back_and_get_vector");
    runTest(test_push_back_and_get_values_vector, "test_push_back_and_get_values_vector");
    runTest(test_push_back_and_get_values_vector_1element, "test_push_back_and_get_values_vector_1element");
    runTest(test_push_back_and_get_values_vector_empty, "test_push_back_and_get_values_vector_empty");
    runTest(test_vector_constructor, "test_vector_constructor");
    runTest(test_push_front, "test_push_front");
    runTest(test_push_1, "test_push_1");
    runTest(test_push_2, "test_push_2");
    runTest(test_front_back, "test_front_back");
    runTest(test_front_node_back_node, "test_front_node_back_node");
    runTest(test_operator_equality, "test_operator_equality");
    runTest(test_default_list, "test_default_list");
    runTest(test_pop_back, "test_pop_back");
    runTest(test_pop_back_exception, "test_pop_back_exception");
    runTest(test_pop_front, "test_pop_front");
    runTest(test_pop_front_exception, "test_pop_front_exception");
    runTest(test_begin_end, "test_begin_end");
    runTest(test_cut_off, "test_cut_off");
    runTest(test_cut_off_exception, "test_cut_off_exception");
    runTest(test_erase, "test_erase");
    runTest(test_merge, "test_merge");
    runTest(test_insert_before_node, "test_insert_before_node");
    runTest(test_insert_before_value, "test_insert_before_value");
    runTest(test_insert_before_value_empty_list, "test_insert_before_value_empty_list");
    runTest(test_split_less, "test_split_less");
    runTest(test_split_less_empty, "test_split_less_empty");
    runTest(test_split_less_all, "test_split_less_all");
    runTest(test_split_empty_list, "test_split_empty_list");
    runTest(test_split_less_all_equal, "test_split_less_all_equal");
    runTest(test_split_less_and_equal, "test_split_less_and_equal");
    runTest(test_split_less_and_equal_empty_list, "test_split_less_and_equal_empty_list");
    runTest(test_split_less_and_equal_empty_less, "test_split_less_and_equal_empty_less");
    runTest(test_split_less_and_equal_empty_equal, "test_split_less_and_equal_empty_equal");
    runTest(test_split_less_and_equal_empty_equal_and_less, "test_split_less_and_equal_empty_equal_and_less");
    runTest(test_to_string_empty, "test_to_string_empty");
    runTest(test_to_string, "test_to_string");
    std::cout << "--- Linked List Tests Finished ---" << std::endl
              << std::endl
              << std::endl;
    return 0;
}