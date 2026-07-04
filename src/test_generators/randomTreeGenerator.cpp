#include <vector>
#include <random>
#include <algorithm>
#include <utility>
#include "generators.hpp"

std::vector<std::pair<std::pair<int, int>, long long>> randomTreeGenerator(int n, int H, long long minWeight, long long maxWeight, long long seed)
{
    std::vector<std::pair<std::pair<int, int>, long long>> edges;
    std::mt19937_64 gen(seed);
    std::vector<int> labels(1, 1);
    for (int i = 2; i <= n; i++)
    {
        labels.push_back(i);
        int p = getRandom(std::max(1, i - H), i - 1, gen);
        long long w = getRandom(minWeight, maxWeight, gen);
        edges.push_back({{i, p}, w});
        edges.push_back({{p, i}, w});
    }
    std::shuffle(edges.begin(), edges.end(), gen);
    std::shuffle(labels.begin(), labels.end(), gen);
    for (int i = 0; i < (int)edges.size(); i++)
    {
        edges[i].first.first = labels[edges[i].first.first - 1];
        edges[i].first.second = labels[edges[i].first.second - 1];
    }
    return edges;
}