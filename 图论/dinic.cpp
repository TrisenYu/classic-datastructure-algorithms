#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;
int S, T, n, m;
const int N = 1e4 + 10, M = 2e5 + 10, inf = 1e8;
long long head[N], e[M], to[M], f[M], tot;
int q[N], fl[N], cur[N]; // 队列 分层 当前弧

inline void add(int u, int v, long long c)
{
    e[tot] = v, f[tot] = c, to[tot] = head[u], head[u] = tot++;
    e[tot] = u, f[tot] = 0, to[tot] = head[v], head[v] = tot++;
}
bool bfs()
{
    int hh = 0, tt = 0;
    memset(fl, ~0, sizeof fl);
    q[0] = S, fl[S] = 0, cur[S] = head[S];
    while (hh <= tt)
    {
        int t = q[hh++];
        for (int i = head[t]; ~i; i = to[i])
        {
            int v = e[i];
            if (fl[v] == -1 && f[i] > 0)
            {
                fl[v] = fl[t] + 1;
                cur[v] = head[v];
                if (v == T)
                    return 1;
                q[++tt] = v;
            }
        }
    }
    return 0;
}
int find(int u, long long limit = inf)
{
    if (u == T)
        return limit;
    long long flow = 0;
    for (int i = cur[u]; ~i && flow < limit /* 显著的优化之一。 */; i = to[i])
    {
        cur[u] = i; // 更新到第i条边时, 前面已经用完
        int v = e[i];
        if (fl[v] == fl[u] + 1 and f[i] > 0)
        {
            int t = find(v, min(f[i], limit - flow));
            if (!t)
                fl[v] = -1;
            f[i] -= t, f[i ^ 1] += t, flow += t;
        }
    }
    return flow;
}
long long dinic()
{
    long long r = 0, flow;
    while (bfs())
        while (flow = find(S))
            r += flow;
    return r;
}

int main()
{
    scanf("%d %d %d %d", &n, &m, &S, &T);
    for (int i = 1; i <= n; ++i)
        head[i] = cur[i] = -1;
    while (m--)
    {
        long long c;
        int a, b;
        scanf("%d %d %lld", &a, &b, &c);
        add(a, b, c);
    }
    printf("%lld", dinic());
    return 0;
}