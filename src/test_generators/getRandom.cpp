#include <random>

#include "generators.hpp"

long long getRandom(long long minValue, long long maxValue, std::mt19937_64 &generator)
{
    long long random = generator();
    if (random < 0)
        random = -random;
    return (random % (maxValue - minValue + 1) + minValue);
}