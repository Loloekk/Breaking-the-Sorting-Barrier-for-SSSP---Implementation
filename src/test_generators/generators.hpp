#ifndef GENERATORS_H
#define GENERATORS_H

#include <random>

long long getRandom(long long minValue, long long maxValue, std::mt19937_64 &generator);

std::vector<std::pair<std::pair<int, int>, long long>> binomialRandomGraphGenerator(int n, long long numerator, long long denominator, long long minWeight, long long maxWeight, bool isAsimmetric, long long seed);
std::vector<std::pair<std::pair<int, int>, long long>> randomTreeGenerator(int n, int H, long long minWeight, long long maxWeight, long long seed);
std::vector<std::pair<std::pair<int, int>, long long>> highRandomGraphGenerator(int n, int H, int k, long long minWeight, long long maxWeight, bool isAsimmetric, long long seed);
std::vector<std::pair<std::pair<int, int>, long long>> randomGraphGenerator(int n, int m, long long minWeight, long long maxWeight, bool isAsimmetric, long long seed);
#endif // GENERATORS_H