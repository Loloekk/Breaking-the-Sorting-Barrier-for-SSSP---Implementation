#include <vector>
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>

#include "generators.hpp"
#include "solutions.hpp"
#include "stats.hpp"
#include "TestExecutor.h"

long long MAX_WEIGHT = 1e9;

int main()
{
    int N = 0;
    int seed = 0;
    int fails = 0;
    std::cout << "--- Starting Speed Tests ---" << std::endl;
    for (int i = 0; i < 5; i++)
    {
        int N_nodes = 1000;
        auto edges = binomialRandomGraphGenerator(N_nodes, 7, 10, 1, MAX_WEIGHT, true, seed++);

        if (!runSolutions(N_nodes, edges, "Dense Asimmetric Binomial Random Graph ", N))
        {
            fails++;
            //break;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        int N_nodes = 10000;
        auto edges = binomialRandomGraphGenerator(N_nodes, 1, 100, 1, MAX_WEIGHT, true, seed++);

        if (!runSolutions(N_nodes, edges, "Sparse Asimmetric Binomial Random Graph", N))
        {
            fails++;
            //break;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        int N_nodes = 200000;
        auto edges = highRandomGraphGenerator(N_nodes, 20, 5, 1, MAX_WEIGHT, true, seed++);

        if (!runSolutions(N_nodes, edges, "High Asimmetric Random Graph", N))
        {
            fails++;
            //break;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        int N_nodes = 1000000;
        auto edges = randomTreeGenerator(N_nodes, 10, 1, MAX_WEIGHT, seed++);

        if (!runSolutions(N_nodes, edges, "High Random Tree", N))
        {
            fails++;
            //break;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        int N_nodes = 100000000;
        auto edges = randomTreeGenerator(N_nodes, N_nodes, 1, MAX_WEIGHT, seed++);

        if (!runSolutions(N_nodes, edges, "Low Random Tree", N))
        {
            fails++;
            //break;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        int N_nodes = 1000000;
        auto edges = randomTreeGenerator(N_nodes, N_nodes, 1, MAX_WEIGHT, seed++);

        if (!runSolutions(N_nodes, edges, "Low Random Tree", N))
        {
            fails++;
            //break;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        int N_nodes = 200000;
        auto edges = randomGraphGenerator(N_nodes, 5 * N_nodes, 1, MAX_WEIGHT, true, seed++);

        if (!runSolutions(N_nodes, edges, "Sparse Asimmetric Random Graph", N))
        {
            fails++;
            //break;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        int N_nodes = 1000;
        auto edges = randomGraphGenerator(N_nodes, N_nodes * (N_nodes - 1), 1, MAX_WEIGHT, true, seed++);

        if (!runSolutions(N_nodes, edges, "Dense Asimmetric Random Graph", N))
        {
            fails++;
            //break;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        int N_nodes = 1000;
        auto edges = binomialRandomGraphGenerator(N_nodes, 7, 10, 1, MAX_WEIGHT, false, seed++);

        if (!runSolutions(N_nodes, edges, "Dense Simmetric Binomial Random Graph ", N))
        {
            fails++;
            //break;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        int N_nodes = 10000;
        auto edges = binomialRandomGraphGenerator(N_nodes, 1, 200, 1, MAX_WEIGHT, false, seed++);

        if (!runSolutions(N_nodes, edges, "Sparse Simmetric Binomial Random Graph", N))
        {
            fails++;
            //break;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        int N_nodes = 100000;
        auto edges = highRandomGraphGenerator(N_nodes, 20, 5, 1, MAX_WEIGHT, false, seed++);

        if (!runSolutions(N_nodes, edges, "High Simmetric Random Graph", N))
        {
            fails++;
            //break;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        int N_nodes = 100000;
        auto edges = randomGraphGenerator(N_nodes, 5 * N_nodes, 1, MAX_WEIGHT, false, seed++);

        if (!runSolutions(N_nodes, edges, "Sparse Simmetric Random Graph", N))
        {
            fails++;
            //break;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        int N_nodes = 1000;
        auto edges = randomGraphGenerator(N_nodes, N_nodes * (N_nodes - 1) / 2, 1, MAX_WEIGHT, false, seed++);

        if (!runSolutions(N_nodes, edges, "Dense Simmetric Random Graph", N))
        {
            fails++;
            //break;
        }
    }

    std::cout << "--- Speed Tests Finished ---" << std::endl
              << std::endl
              << std::endl;

    return 0;
}