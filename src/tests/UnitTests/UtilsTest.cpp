#include <iostream>
#include <string>
#include <functional>
#include <utility>
#include <algorithm>

#include "utils.h"
#include "TestExecutor.h"

void test_bounded_k_value()
{
    std::vector<int> data = {1, 2, 3, 4, 5};
    try
    {
        kthValue(data, 0);
        throw std::runtime_error("kthValue did not throw an exception for k=0");
    }
    catch (const std::logic_error &e)
    {
        if (std::string(e.what()) != "Pointer overflow: cant return 0 element")
            throw std::runtime_error("kthValue threw an unexpected exception for k=0");
    }
    try
    {
        kthValue(data, 6);
        throw std::runtime_error("kthValue did not throw an exception for k greater than data size");
    }
    catch (const std::logic_error &e)
    {
        if (std::string(e.what()) != "Pointer overflow: there is not enought elements.")
            throw std::runtime_error("kthValue threw an unexpected exception for k greater than data size");
    }
}

void test_k_value()
{
    std::vector<int> data = {5, 3, 1, 4, 2};
    if (kthValue(data, 1) != 1)
        throw std::runtime_error("kthValue returned incorrect value for k=1");
    if (kthValue(data, 2) != 2)
        throw std::runtime_error("kthValue returned incorrect value for k=2");
    if (kthValue(data, 3) != 3)
        throw std::runtime_error("kthValue returned incorrect value for k=3");
    if (kthValue(data, 4) != 4)
        throw std::runtime_error("kthValue returned incorrect value for k=4");
    if (kthValue(data, 5) != 5)
        throw std::runtime_error("kthValue returned incorrect value for k=5");
}

void test_k_value2()
{
    std::vector<int> data;
    std::vector<int> sorted;
    for (int i = 1; i <= 500; i++)
    {
        int val = i * i % 200;
        data.push_back(val);
        sorted.push_back(val);
    }
    std::sort(sorted.begin(), sorted.end());
    for (int i = 0; i < 500; i++)
    {
        if (sorted[i] != kthValue(data, i + 1))
            throw std::runtime_error("kthValue returned incorrect value for k=" + std::to_string(i + 1));
    }
}

int main()
{
    std::cout << "--- Starting Utils Tests ---" << std::endl;
    runTest(test_bounded_k_value, "test_bounded_k_value");
    runTest(test_k_value, "test_k_value");
    runTest(test_k_value2, "test_k_value2");
    std::cout << "--- Utils Tests Finished ---" << std::endl
              << std::endl
              << std::endl;
    return 0;
}