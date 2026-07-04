#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

#include "solutions.hpp"
#include "stats.hpp"

using namespace std;

int main()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    int n, m, s, d;
    std::cin >> n >> m >> s >> d;

    vector<pair<pair<int, int>, long long>> edges;
    while (m--)
    {
        int a, b;
        long long c;
        std::cin >> a >> b >> c;
        edges.push_back({{a, b}, c});
        if (!d)
            edges.push_back({{b, a}, c});
    }

    TheorethicalSolution<false> solution;

    using namespace std::chrono;

    auto start_time = high_resolution_clock::now();
    solution.init(n, edges);
    auto end_time = high_resolution_clock::now();
    double theo_init_time = duration<double>(end_time - start_time).count();

    start_time = high_resolution_clock::now();
    solution.solve(s);
    end_time = high_resolution_clock::now();
    double theo_solve_time = duration<double>(end_time - start_time).count();

    start_time = high_resolution_clock::now();
    auto res = solution.get();
    end_time = high_resolution_clock::now();
    double theo_get_time = duration<double>(end_time - start_time).count();

    double theo_total_time = theo_init_time + theo_solve_time + theo_get_time;

    for (int i = 1; i <= n; i++)
    {
        std::cout << i << ' ' << res[i] << '\n';
    }

    Stats stats = getStats(s, n, edges, res);

    std::cerr << "--- Graph Statistics ---\n";
    std::cerr << "Nodes count (N)                    : " << stats.n << "\n";
    std::cerr << "Edges count (M)                    : " << stats.m << "\n";
    std::cerr << "Longest path length                : " << stats.longest_path << "\n";
    std::cerr << "Longest vertex path                : " << stats.longest_vertex_path << "\n";
    std::cerr << "Longest vertex path (by weights)   : " << stats.longest_vertex_path_by_weights << "\n";
    std::cerr << "Minimum edge weight                : " << stats.min_weight << "\n";
    std::cerr << "Maximum edge weight                : " << stats.max_weight << "\n";
    std::cerr << "Main component size                : " << stats.component_size << "\n";
    std::cerr << "Main component edge count          : " << stats.component_edge_count << "\n\n";

    std::cerr << "--- Execution Times ---\n";
    std::cerr << std::fixed << std::setprecision(2);
    std::cerr << "+-------------+---------+---------+---------+---------+\n";
    std::cerr << "| Solution    | Init(s) | Solve(s)| Get(s)  | Total(s)|\n";
    std::cerr << "+-------------+---------+---------+---------+---------+\n";
    std::cerr << "| Theoretical | "
              << std::setw(7) << theo_init_time << " | "
              << std::setw(7) << theo_solve_time << " | "
              << std::setw(7) << theo_get_time << " | "
              << std::setw(7) << theo_total_time << " |\n";
    std::cerr << "+-------------+---------+---------+---------+---------+\n";

    return 0;
}