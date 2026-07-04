#ifndef DIJKSTRA_SOLUTION_HPP
#define DIJKSTRA_SOLUTION_HPP

#include <vector>
#include <queue>
#include "solutions.hpp"

using namespace std;

const long long INF = 1e18;

class DijkstraSolution
{
private:
    std::vector<std::vector<std::pair<int, long long>>> neighbors;
    std::vector<long long> dis;
    int n;

public:
    void init(int n, const std::vector<std::pair<std::pair<int, int>, long long>> &edges)
    {
        this->n = n;
        dis.resize(this->n + 1, INF);
        neighbors.resize(n + 1);
        for (auto [e, w] : edges)
        {
            auto [a, b] = e;
            neighbors[a].push_back({b, w});
        }
    }

    void solve(int s)
    {
        std::priority_queue<std::pair<long long, int>, std::vector<std::pair<long long, int>>, std::greater<std::pair<long long, int>>> Q;
        dis[s] = 0;
        Q.push({0, s});
        while (!Q.empty())
        {
            auto [d, u] = Q.top();
            Q.pop();
            if (dis[u] != d)
                continue;
            for (auto [v, w] : neighbors[u])
            {
                if (dis[u] + w < dis[v])
                {
                    dis[v] = dis[u] + w;
                    Q.push({dis[v], v});
                }
            }
        }
    }
    const std::vector<long long> &get()
    {
        return dis;
    }
    void free()
    {
        std::vector<std::vector<std::pair<int, long long>>> neighborsTmp;
        neighbors.swap(neighborsTmp);
        std::vector<long long> disTmp;
        dis.swap(disTmp);
    }
};

#endif /*DIJKSTRA_SOLUTION_HPP*/