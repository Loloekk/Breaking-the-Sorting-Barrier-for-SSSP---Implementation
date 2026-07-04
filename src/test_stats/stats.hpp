#ifndef STATS_H
#define STATS_H

#include <random>

struct Stats
{
    int n = 0, m = 0;
    int longest_vertex_path = 0;
    long long longest_path = 0;
    int longest_vertex_path_by_weights = 0;
    long long min_weight = 0;
    long long max_weight = 0;
    int component_size = 0;
    int component_edge_count = 0;
};

Stats getStats(int s, int n, const std::vector<std::pair<std::pair<int, int>, long long>> &edges, const std::vector<long long> &dis);

#endif // STATS_H