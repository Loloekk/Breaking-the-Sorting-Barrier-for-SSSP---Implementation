#ifndef TEST_EXECUTOR_H
#define TEST_EXECUTOR_H

#include <iostream>
#include <string>
#include <functional>
#include <stdexcept>
#include <chrono>
#include <iomanip>

#include "solutions.hpp"
#include "stats.hpp"

void runTest(std::function<void()> test_func, const std::string &test_name)
{
    try
    {
        test_func();
        std::cout << "[ OK ] " << test_name << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ FAIL ] " << test_name << " | Exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "[ FAIL ] " << test_name << " | Unknown error!" << std::endl;
    }
}

template <typename ExceptionType>
void runExceptionTest(std::function<void()> test_func,
                      const std::string &expected_msg)
{
    bool isOK = false;
    std::string message = "Expected exception was not thrown";
    try
    {
        test_func();
    }
    catch (const ExceptionType &e)
    {
        std::string actual_msg = e.what();
        if (actual_msg == expected_msg)
        {
            isOK = true;
        }
        else
        {
            message = (std::string) "Correct exception but wrong message!\n" +
                      "         Expected: " + expected_msg + "\n" +
                      "         Received: " + actual_msg;
        }
    }
    catch (const std::exception &e)
    {
        std::string actual_msg = e.what();
        message = (std::string) "Caught unexpected exception type!\n" +
                  "         Expected message: " + expected_msg + "\n" +
                  "         Received message: " + actual_msg;
    }
    catch (...)
    {
        message = "Caught completely unknown exception type!";
    }
    if (!isOK)
        throw std::runtime_error(message);
}

bool runSolutions(int n, const std::vector<std::pair<std::pair<int, int>, long long>> &edges, std::string type, int &N, bool verbose = true)
{
    N++;

    if (verbose)
    {
        std::cout << "========================================================\n";
        std::cout << "Type: " << type << "\n";
    }

    TheorethicalSolution<false> theorethical;
    TheorethicalSolution<true> theorethicalSafeMode;
    DijkstraSolution dijkstra;

    using namespace std::chrono;

    auto start_time = high_resolution_clock::now();
    theorethical.init(n, edges);
    auto end_time = high_resolution_clock::now();
    double theo_init_time = duration<double>(end_time - start_time).count();

    start_time = high_resolution_clock::now();
    theorethical.solve(1);
    end_time = high_resolution_clock::now();
    double theo_solve_time = duration<double>(end_time - start_time).count();

    start_time = high_resolution_clock::now();
    std::vector<long long> theorethicalOutput = theorethical.get();
    end_time = high_resolution_clock::now();
    double theo_get_time = duration<double>(end_time - start_time).count();

    double theo_total_time = theo_init_time + theo_solve_time + theo_get_time;

    start_time = high_resolution_clock::now();
    theorethicalSafeMode.init(n, edges);
    end_time = high_resolution_clock::now();
    double safe_init_time = duration<double>(end_time - start_time).count();

    start_time = high_resolution_clock::now();
    theorethicalSafeMode.solve(1);
    end_time = high_resolution_clock::now();
    double safe_solve_time = duration<double>(end_time - start_time).count();

    start_time = high_resolution_clock::now();
    std::vector<long long> theorethicalSafeOutput = theorethicalSafeMode.get();
    end_time = high_resolution_clock::now();
    double safe_get_time = duration<double>(end_time - start_time).count();

    double safe_total_time = safe_init_time + safe_solve_time + safe_get_time;

    start_time = high_resolution_clock::now();
    dijkstra.init(n, edges);
    end_time = high_resolution_clock::now();
    double dijk_init_time = duration<double>(end_time - start_time).count();

    start_time = high_resolution_clock::now();
    dijkstra.solve(1);
    end_time = high_resolution_clock::now();
    double dijk_solve_time = duration<double>(end_time - start_time).count();

    start_time = high_resolution_clock::now();
    std::vector<long long> dijkstraOutput = dijkstra.get();
    end_time = high_resolution_clock::now();
    double dijk_get_time = duration<double>(end_time - start_time).count();

    double dijk_total_time = dijk_init_time + dijk_solve_time + dijk_get_time;

    bool correct = true;
    std::string diff_msg = "";

    for (int i = 1; i < (int)dijkstraOutput.size(); i++)
    {
        if (theorethicalOutput[i] != dijkstraOutput[i])
        {
            diff_msg += "Difference (Theoretical) on index " + std::to_string(i) +
                        ": Dijkstra returned " + std::to_string(dijkstraOutput[i]) +
                        ", Theoretical returned " + std::to_string(theorethicalOutput[i]) + ".\n";
            correct = false;
        }
        if (theorethicalSafeOutput[i] != dijkstraOutput[i])
        {
            diff_msg += "Difference (Theo Safe) on index " + std::to_string(i) +
                        ": Dijkstra returned " + std::to_string(dijkstraOutput[i]) +
                        ", Theo Safe returned " + std::to_string(theorethicalSafeOutput[i]) + ".\n";
            correct = false;
        }
    }

    if (correct)
    {
        std::cout << "[ OK ] ";
    }
    else
    {
        std::cout << "[ FAIL ] ";
    }

    std::cout << "Test number: " << N << " " << type << "\n";

    if (verbose)
    {

        if (!correct)
        {
            std::cout << diff_msg;
        }
        Stats stats = getStats(1, n, edges, dijkstraOutput);
        std::cout << "--- Graph Statistics ---\n";
        std::cout << "Nodes count (N)                    : " << stats.n << "\n";
        std::cout << "Edges count (M)                    : " << stats.m << "\n";
        std::cout << "Longest path length                : " << stats.longest_path << "\n";
        std::cout << "Longest vertex path                : " << stats.longest_vertex_path << "\n";
        std::cout << "Longest vertex path (by weights)   : " << stats.longest_vertex_path_by_weights << "\n";
        std::cout << "Minimum edge weight                : " << stats.min_weight << "\n";
        std::cout << "Maximum edge weight                : " << stats.max_weight << "\n";
        std::cout << "Main component size                : " << stats.component_size << "\n";
        std::cout << "Main component edge count          : " << stats.component_edge_count << "\n\n";

        std::cout << "--- Execution Times ---\n";

        std::cout << std::fixed << std::setprecision(2);

        std::cout << "+-------------+---------+---------+---------+---------+\n";
        std::cout << "| Solution    | Init(s) | Solve(s)| Get(s)  | Total(s)|\n";
        std::cout << "+-------------+---------+---------+---------+---------+\n";

        std::cout << "| Theoretical | "
                  << std::setw(7) << theo_init_time << " | "
                  << std::setw(7) << theo_solve_time << " | "
                  << std::setw(7) << theo_get_time << " | "
                  << std::setw(7) << theo_total_time << " |\n";

        std::cout << "| Theo Safe   | "
                  << std::setw(7) << safe_init_time << " | "
                  << std::setw(7) << safe_solve_time << " | "
                  << std::setw(7) << safe_get_time << " | "
                  << std::setw(7) << safe_total_time << " |\n";

        std::cout << "| Dijkstra    | "
                  << std::setw(7) << dijk_init_time << " | "
                  << std::setw(7) << dijk_solve_time << " | "
                  << std::setw(7) << dijk_get_time << " | "
                  << std::setw(7) << dijk_total_time << " |\n";

        std::cout << "+-------------+---------+---------+---------+---------+\n\n";
    }

    return correct;
}

#endif // TEST_EXECUTOR_H