#include <vector>
#include <random>
#include <utility>
#include <unordered_set>
#include <algorithm>
#include "generators.hpp"

std::vector<std::pair<std::pair<int, int>, long long>> bigMRandomGraphGenerator(int n, int m, long long minWeight, long long maxWeight, bool isAsimmetric, long long seed)
{
    std::vector<std::pair<std::pair<int, int>, long long>> edges;
    std::mt19937_64 gen(seed);
    std::vector<std::pair<int, int>> allEdges;
    for (int i = 1; i <= n; i++)
        for (int j = i + 1; j <= n; j++)
        {
            allEdges.push_back({i, j});
            if (isAsimmetric)
                allEdges.push_back({j, i});
        }
    for (int i = 0; i < m; i++)
    {
        int I = getRandom(0, allEdges.size() - 1, gen);
        auto [a, b] = allEdges[I];
        allEdges[I] = allEdges[allEdges.size() - 1];
        allEdges.pop_back();
        long long w = getRandom(minWeight, maxWeight, gen);
        edges.push_back({{a, b}, w});
        if (!isAsimmetric)
            edges.push_back({{b, a}, w});
    }
    return edges;
}

struct PairHash
{
    template <class T, class V>
    int operator()(const std::pair<T, V> &p) const
    {
        int h1 = std::hash<T>{}(p.first);
        int h2 = std::hash<V>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

std::vector<std::pair<std::pair<int, int>, long long>> smallMRandomGraphGenerator(int n, int m, long long minWeight, long long maxWeight, bool isAsimmetric, long long seed)
{
    std::vector<std::pair<std::pair<int, int>, long long>> edges;
    std::mt19937_64 gen(seed);
    std::unordered_set<std::pair<int, int>, PairHash> usedEdges;
    usedEdges.insert({0, 0});
    for (int i = 0; i < m; i++)
    {
        int a = 0, b = 0;
        while (usedEdges.count({a, b}))
        {
            a = getRandom(1, n, gen);
            b = a;
            while (a == b)
                b = getRandom(1, n, gen);
        }
        long long w = getRandom(minWeight, maxWeight, gen);
        edges.push_back({{a, b}, w});
        usedEdges.insert({a, b});
        if (!isAsimmetric)
        {
            edges.push_back({{b, a}, w});
            usedEdges.insert({b, a});
        }
    }
    return edges;
}

std::vector<std::pair<std::pair<int, int>, long long>> randomGraphGenerator(int n, int m, long long minWeight, long long maxWeight, bool isAsimmetric, long long seed)
{
    if (m == 0)
        return {};
    std::vector<std::pair<std::pair<int, int>, long long>> edges;
    if ((isAsimmetric && m > n * (n - 1) / 2))
        edges = bigMRandomGraphGenerator(n, std::min(m, n * (n - 1)), minWeight, maxWeight, isAsimmetric, seed);
    else if ((!isAsimmetric) && m > n * (n - 1) / 4)
        edges = bigMRandomGraphGenerator(n, std::min(m, n * (n - 1) / 2), minWeight, maxWeight, isAsimmetric, seed);
    else
        edges = smallMRandomGraphGenerator(n, m, minWeight, maxWeight, isAsimmetric, seed);
    std::mt19937_64 gen(seed);
    std::vector<int> labels;
    for (int i = 1; i <= n; i++)
        labels.push_back(i);
    std::shuffle(edges.begin(), edges.end(), gen);
    std::shuffle(labels.begin(), labels.end(), gen);
    for (int i = 0; i < (int)edges.size(); i++)
    {
        edges[i].first.first = labels[edges[i].first.first - 1];
        edges[i].first.second = labels[edges[i].first.second - 1];
    }
    return edges;
}