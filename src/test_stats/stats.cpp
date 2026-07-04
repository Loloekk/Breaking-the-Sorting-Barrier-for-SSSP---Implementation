#include <vector>
#include <random>
#include <queue>
#include "stats.hpp"

const long long INF = 1e18;

void bfs(int s, const std::vector<std::vector<std::pair<int, long long>>> &g, std::vector<int> &vertex_dis)
{
    vertex_dis[s] = 0;
    std::queue<int> Q;
    Q.push(s);
    while (!Q.empty())
    {
        int u = Q.front();
        Q.pop();
        for (auto [v, w] : g[u])
        {
            if (vertex_dis[v] == -1)
            {
                vertex_dis[v] = vertex_dis[u] + 1;
                Q.push(v);
            }
        }
    }
}

void bfsByWeights(int s, const std::vector<std::vector<std::pair<int, long long>>> &g, const std::vector<long long> &dis, std::vector<int> &vertex_dis)
{
    vertex_dis[s] = 0;
    std::queue<int> Q;
    Q.push(s);
    while (!Q.empty())
    {
        int u = Q.front();
        Q.pop();
        for (auto [v, w] : g[u])
        {
            if (vertex_dis[v] == -1 && dis[v] == dis[u] + w)
            {
                vertex_dis[v] = vertex_dis[u] + 1;
                Q.push(v);
            }
        }
    }
}

Stats getStats(int s, int n, const std::vector<std::pair<std::pair<int, int>, long long>> &edges, const std::vector<long long> &dis)
{
    Stats stats;
    stats.n = n;
    stats.m = edges.size();
    if (stats.m == 0)
    {
        stats.component_size = 1;
        return stats;
    }
    stats.min_weight = edges[0].second;
    stats.max_weight = edges[0].second;
    std::vector<std::vector<std::pair<int, long long>>> g(n + 1);
    for (auto [e, w] : edges)
    {
        auto [a, b] = e;
        stats.min_weight = std::min(stats.min_weight, w);
        stats.max_weight = std::max(stats.max_weight, w);
        g[a].push_back({b, w});
    }
    std::vector<int> vertex_dis(n + 1, -1);
    bfs(s, g, vertex_dis);
    for (int i = 1; i <= n; i++)
    {
        if (vertex_dis[i] >= 0)
        {
            stats.component_size++;
            stats.component_edge_count += g[i].size();
        }
        if (dis[i] != INF)
            stats.longest_path = std::max(stats.longest_path, dis[i]);
        stats.longest_vertex_path = std::max(stats.longest_vertex_path, vertex_dis[i]);
    }
    std::fill(vertex_dis.begin(), vertex_dis.end(), -1);
    bfsByWeights(s, g, dis, vertex_dis);
    for (int i = 1; i <= n; i++)
    {
        stats.longest_vertex_path_by_weights = std::max(stats.longest_vertex_path_by_weights, vertex_dis[i]);
    }
    return stats;
}