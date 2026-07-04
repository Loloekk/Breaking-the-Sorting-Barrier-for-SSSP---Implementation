#include <vector>
#include <random>
#include <algorithm>

#include "generators.hpp"

std::vector<std::pair<std::pair<int, int>, long long>> binomialRandomGraphGenerator(int n, long long numerator, long long denominator, long long minWeight, long long maxWeight, bool isAsimmetric, long long seed)
{
    std::vector<std::pair<std::pair<int, int>, long long>> edges;
    std::mt19937_64 gen(seed);
    std::vector<int> labels;
    for (int i = 1; i <= n; i++)
    {
        labels.push_back(i);
        for (int j = (isAsimmetric ? i + 1 : 1); j <= n; j++)
            if (i != j)
            {
                int a = i, b = j;
                if (isAsimmetric && getRandom(0, 1, gen))
                {
                    std::swap(a, b);
                }
                if (getRandom(1, denominator, gen) <= numerator)
                    edges.push_back({{a, b}, getRandom(minWeight, maxWeight, gen)});
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