#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <stdexcept>

template <typename T>
inline void order(T &a, T &b)
{
    if (b < a)
        std::swap(a, b);
}

template <typename T>
inline void medianOfFive(T *p)
{
    order(p[0], p[1]);
    order(p[3], p[4]);

    if (p[3] < p[0])
    {
        std::swap(p[0], p[3]);
        std::swap(p[1], p[4]);
    }

    order(p[1], p[2]);

    if (p[1] < p[3])
    {
        std::swap(p[1], p[3]);
        std::swap(p[2], p[4]);
    }

    order(p[2], p[3]);
}

template <typename T>
T kthValueRecursion(T *left, T *right, int k)
{
    int n = right - left + 1;

    if (n <= 5)
    {
        std::sort(left, right + 1);
        return *(left + k - 1);
    }

    int num_medians = n / 5;
    for (int i = 0; i < (int)num_medians; i++)
    {
        T *group_start = left + i * 5;
        std::sort(group_start, group_start + 5);
        std::swap(*(group_start + 2), *(left + i));
    }

    if (n % 5 != 0)
    {
        T *group_start = left + num_medians * 5;
        int rem = n % 5;
        std::sort(group_start, right + 1);
        std::swap(*(group_start + (rem - 1) / 2), *(left + num_medians));
        num_medians++;
    }

    T pivot = kthValueRecursion(left, left + num_medians - 1, (num_medians / 2) + 1);

    T *pivot_ptr = left;
    while (pivot_ptr <= right && *pivot_ptr != pivot)
    {
        pivot_ptr++;
    }
    std::swap(*pivot_ptr, *right);

    T *i = left;
    for (T *j = left; j < right; j++)
    {
        if (*j < pivot)
        {
            std::swap(*i, *j);
            i++;
        }
    }

    std::swap(*i, *right);
    T *pivot_pos = i;

    int rank = pivot_pos - left + 1;

    if (k == rank)
    {
        return *pivot_pos;
    }
    else if (k < rank)
    {
        return kthValueRecursion(left, pivot_pos - 1, k);
    }
    else
    {
        return kthValueRecursion(pivot_pos + 1, right, k - rank);
    }
}

template <typename T, bool SafeMode = true>
T kthValue(std::vector<T> &data, int k)
{
    if constexpr (SafeMode)
    {
        if (k <= 0)
            throw std::logic_error("Pointer overflow: cant return 0 element");
        if ((int)data.size() < k)
            throw std::logic_error("Pointer overflow: there is not enought elements.");
    }
    return kthValueRecursion(data.data(), data.data() + data.size() - 1, k);
}

#endif // UTILS_H