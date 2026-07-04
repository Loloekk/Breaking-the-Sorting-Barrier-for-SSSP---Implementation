#include <vector>
#include <random>
#include <utility>
#include <algorithm>
#include "generators.hpp"

std::vector<std::pair<std::pair<int, int>, long long>> highRandomGraphGenerator(int n, int H, int k, long long minWeight, long long maxWeight, bool isAsimmetric, long long seed)
{
    std::vector<std::pair<std::pair<int, int>, long long>> edges;
    std::mt19937_64 gen(seed);
    std::vector<int> labels;
    for (int i = 1; i <= n; i++)
    {
        labels.push_back(i);
        for (int j = 0; j < k; j++)
        {
            int a = i;
            int b = i;
            while (b == i)
                b = getRandom(std::max(i - H, 1), std::min(i + H, n), gen);
            long long w = getRandom(minWeight, maxWeight, gen);
            if (getRandom(0, 1, gen))
                std::swap(a, b);
            edges.push_back({{a, b}, w});
            if (!isAsimmetric)
                edges.push_back({{b, a}, w});
        }
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