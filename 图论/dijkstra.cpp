#include <cstdio>
#include <cstring>
#include <cmath>
#include <iostream>
#include <queue>

const int N = 1e5 + 10;
int head[N], tot, e[N], to[N], w[N];
inline void add(int u, int v, int val)
{
    e[++tot] = v, to[tot] = head[u], w[tot] = val, head[u] = tot;
}

int st, ed;
int dis[N];
bool vis[N];

void dij()
{
    std::queue<int> q;
    q.push(st);
    memset(dis, ~0, sizeof dis);
    dis[st] = 0;
    while (q.size())
    {
        int u = q.front();
        q.pop();
        for (int i = head[u]; ~i; i = to[i])
        {
            int v = e[i];
            if (dis[u] + w[i] < dis[v])
            {
                dis[v] = dis[u] + w[i];
            }
        }
    }
}

void spfa()
{
    // TODO.
}