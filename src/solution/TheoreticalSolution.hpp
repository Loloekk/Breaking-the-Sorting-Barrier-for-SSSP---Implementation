
#ifndef THEORETICAL_SOLUTION_HPP
#define THEORETICAL_SOLUTION_HPP

#include <vector>
#include <stdexcept>
#include <cmath>
#include <string>

#include "solutions.hpp"
#include "PathDistance.hpp"
#include "IndexedPriorityQueue.hpp"
#include "BatchPriorityQueue.h"

template <bool SafeMode = true>
class TheorethicalSolution
{
private:
    using QueueValue = PathDistance<long long>;
    using InnerList = LinkedList<pairr<QueueValue, int>, SafeMode>;
    using OuterList = LinkedList<pairr<QueueValue, InnerList>, SafeMode>;
    using InnerNode = LinkedListNode<pairr<QueueValue, int>, SafeMode>;
    using OuterNode = LinkedListNode<pairr<QueueValue, InnerList>, SafeMode>;
    using BPQNodeRecord = pairr<pairr<InnerNode *, OuterNode *>, bool>;

    std::vector<std::vector<std::pair<int, long long>>> neighbors;
    std::vector<PathDistance<long long>> dis;
    int original_n;
    int n, k, t, initial_l;

    std::vector<bool> WVisited;
    std::vector<bool> WiVisited;
    std::vector<bool> SVisited;
    std::vector<int> first_child;
    std::vector<int> next_sibling;
    std::vector<std::vector<bool>> BMSSPUVisited;
    std::vector<std::vector<BPQNodeRecord>> BPQPointers;
    std::vector<int> IPQpointers;

    std::pair<PathDistance<long long>, std::vector<int>> baseCase(PathDistance<long long> B, const std::vector<int> &S)
    {
        IndexedPriorityQueue<PathDistance<long long>, SafeMode> H(IPQpointers);
        std::vector<int> U0;
        for (int u : S)
        {
            H.set_value(u, dis[u]);
        }
        while (!H.empty())
        {
            auto [u, d] = H.top();
            H.pop();
            for (auto e : neighbors[u])
            {
                auto [v, w] = e;
                PathDistance<long long> new_dist = dis[u] + e;

                if (new_dist <= dis[v] && new_dist < B)
                {
                    dis[v] = new_dist;
                    H.set_value(v, dis[v]);
                }
            }
            if (U0.size() == (unsigned int)k)
            {
                B = dis[u];
                break;
            }
            U0.push_back(u);
        }
        return {B, U0};
    }

    void FindPivotsClear(const std::vector<int> &W, const std::vector<int> &S, std::vector<bool> &WVisited, std::vector<bool> &SVisited, std::vector<int> &first_child, std::vector<int> &next_sibling)
    {
        for (int u : S)
            SVisited[u] = false;
        for (int u : W)
        {
            WVisited[u] = false;
            first_child[u] = -1;
            next_sibling[u] = -1;
        }
    }

    std::pair<std::vector<int>, std::vector<int>> findPivots(const PathDistance<long long> &B, const std::vector<int> &S)
    {
        std::vector<int> W, Wi, Wl;
        for (int u : S)
            if (!SVisited[u])
            {
                WVisited[u] = true;
                SVisited[u] = true;
                W.push_back(u);
                Wl.push_back(u);
            }
        for (int i = 1; i <= k; i++)
        {
            for (int u : Wl)
            {
                for (auto e : neighbors[u])
                {
                    auto [v, w] = e;
                    PathDistance<long long> new_dist = dis[u] + e;
                    if (new_dist <= dis[v])
                    {
                        dis[v] = new_dist;
                        if (dis[v] < B)
                        {
                            if (!WiVisited[v])
                            {
                                WiVisited[v] = true;
                                Wi.push_back(v);
                            }
                        }
                    }
                }
            }
            for (int u : Wi)
            {
                WiVisited[u] = false;
                if (!WVisited[u])
                {
                    WVisited[u] = true;
                    W.push_back(u);
                }
            }
            if (W.size() > k * S.size())
            {
                FindPivotsClear(W, S, WVisited, SVisited, first_child, next_sibling);
                return {S, W};
            }
            Wl.swap(Wi);
            Wi.clear();
        }
        for (int u : W)
        {
            // if(SVisited[u]) /////////////////////////////////////////////////////////There was a bug, where I spend 20 hours
            //     continue;
            int p = dis[u].last;

            if constexpr (SafeMode)
            {
                if (p == -1)
                    throw std::out_of_range("Parent's index error");
            }
            next_sibling[u] = first_child[p];
            first_child[p] = u;
        }

        auto dfs_size = [&](auto &self, int u) -> int
        {
            int size = 1;
            for (int v = first_child[u]; v != -1; v = next_sibling[v])
            {
                size += self(self, v);
            }
            return size;
        };
        std::vector<int> P;
        for (int u : S)
        {
            if (!WVisited[dis[u].last] && dfs_size(dfs_size, u) >= k)
            {
                P.push_back(u);
            }
        }
        FindPivotsClear(W, S, WVisited, SVisited, first_child, next_sibling);
        return {P, W};
    }

    std::pair<PathDistance<long long>, std::vector<int>> BMSSP(int l, PathDistance<long long> B, const std::vector<int> &S)
    {
        if (l == 0)
            return baseCase(B, S);
        std::vector<int> U;
        std::vector<bool> &UVisited = BMSSPUVisited[l - 1];
        auto [P, W] = findPivots(B, S);
        BatchPriorityQueue<PathDistance<long long>, SafeMode> D(1ULL << ((l - 1) * t), B, n, BPQPointers[l - 1]);
        PathDistance<long long> Bip = B;
        for (int u : P)
        {
            D.insert(u, dis[u]);
            if (dis[u] < Bip)
                Bip = dis[u];
        }
        int i = 0;
        std::vector<std::pair<int, PathDistance<long long>>> K;

        while (U.size() < k * (1ULL << (l * t)) && (!D.empty()))
        {
            i++;
            auto [Si, Bi] = D.pull();
            auto [Biptmp, Ui] = BMSSP(l - 1, Bi, Si);
            Bip = Biptmp;

            for (int u : Ui)
            {
                if (!UVisited[u])
                {

                    UVisited[u] = true;
                    U.push_back(u);
                }
            }

            K.clear();
            for (int u : Ui)
            {
                D.erase(u);
                for (auto e : neighbors[u])
                {
                    auto [v, w] = e;
                    PathDistance<long long> new_dist = dis[u] + e;
                    if (new_dist <= dis[v])
                    {
                        dis[v] = new_dist;
                        if (Bi <= new_dist && new_dist < B)
                            D.insert(v, new_dist);
                        else if (Bip <= new_dist && new_dist < Bi)
                            K.push_back({v, new_dist});
                    }
                }
            }
            for (int u : Si)
                if (Bip <= dis[u] && dis[u] < Bi)
                    K.push_back({u, dis[u]});

            D.insert(K);
        }

        PathDistance<long long> Bp = B;

        if (Bip < Bp && (!D.empty()))
            Bp = Bip;

        for (int u : W)
            if (dis[u] < Bp && (!UVisited[u]))
            {
                UVisited[u] = true;
                U.push_back(u);
            }

        for (int u : U)
            UVisited[u] = false;
        return {Bp, U};
    }

    int prepareVertices(int original_n, const std::vector<std::pair<std::pair<int, int>, long long>> &edges)
    {
        std::vector<std::vector<int>> ports(original_n + 1);
        std::vector<int> in_degree(original_n + 1, 0), out_degree(original_n + 1, 0);
        int N = original_n;
        neighbors.push_back({});
        for (int i = 1; i <= original_n; i++)
        {
            ports[i].push_back(i);
            neighbors.push_back({});
        }
        for (auto [e, w] : edges)
        {
            auto [a, b] = e;
            if (in_degree[a] == (int)ports[a].size())
            {
                ports[a].push_back(++N);
                neighbors.push_back({});
            }
            if (in_degree[b] == (int)ports[b].size())
            {
                ports[b].push_back(++N);
                neighbors.push_back({});
            }
            neighbors[ports[a][in_degree[a]]].push_back({ports[b][in_degree[b]], w});
            in_degree[a]++;
            in_degree[b]++;
        }
        for (int v = 1; v <= original_n; v++)
        {
            if (ports[v].size() == 1)
                continue;
            for (int i = 1; i < (int)ports[v].size(); i++)
                neighbors[ports[v][i - 1]].push_back({ports[v][i], 0});
            neighbors[ports[v][ports[v].size() - 1]].push_back({ports[v][0], 0});
        }
        return N;
    }

public:
    void init(int original_n, const std::vector<std::pair<std::pair<int, int>, long long>> &edges)
    {
        this->original_n = original_n;
        n = prepareVertices(original_n, edges);
        dis.resize(n + 1, {INF, 0, 0, 0});

        k = floor(pow(log2(n), 1.0 / 3.0));
        t = floor(pow(log2(n), 2.0 / 3.0));
        initial_l = ceil(log2(n) / t);

        WVisited.resize(n + 1, false);
        WiVisited.resize(n + 1, false);
        SVisited.resize(n + 1, false);
        first_child.resize(n + 1, -1);
        next_sibling.resize(n + 1, -1);

        BMSSPUVisited.resize(initial_l, std::vector<bool>(n + 1, false));
        BPQPointers.resize(initial_l, std::vector<BPQNodeRecord>(n + 1, {{nullptr, nullptr}, false}));

        IPQpointers.resize(n + 1, -1);
    }

    void solve(int s)
    {
        dis[s] = {0, 0, s, s};
        BMSSP(initial_l, {INF, 0, 0, 0}, {s});
    }

    std::vector<long long> get()
    {
        std::vector<long long> output(n + 1, 0);

        for (int i = 1; i <= n; i++)
        {
            output[i] = dis[i].dis;
        }
        return output;
    }
    void free()
    {
        std::vector<std::vector<std::pair<int, long long>>> neighborsTmp;
        neighbors.swap(neighborsTmp);
        std::vector<PathDistance<long long>> disTmp;
        dis.swap(disTmp);
        std::vector<bool> WVisitedTmp;
        WVisited.swap(WVisitedTmp);
        std::vector<bool> WiVisitedTmp;
        WiVisited.swap(WiVisitedTmp);
        std::vector<bool> SVisitedTmp;
        SVisited.swap(SVisitedTmp);
        std::vector<int> first_childTmp;
        first_child.swap(first_childTmp);
        std::vector<int> next_siblingTmp;
        next_sibling.swap(next_siblingTmp);
        std::vector<std::vector<bool>> BMSSPUVisitedTmp;
        BMSSPUVisited.swap(BMSSPUVisitedTmp);
        std::vector<std::vector<pairr<pairr<LinkedListNode<pairr<PathDistance<long long>, int>> *, LinkedListNode<pairr<PathDistance<long long>, LinkedList<pairr<PathDistance<long long>, int>>>> *>, bool>>> BPQPointersTmp;
        BPQPointers.swap(BPQPointersTmp);
        std::vector<int> IPQpointersTmp;
        IPQpointers.swap(IPQpointersTmp);
    }
};

#endif /*THEORETICAL_SOLUTION_HPP*/