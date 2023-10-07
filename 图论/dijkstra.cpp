#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <queue>
using pii = std::pair<int, int>;
const int N = 1E5 + 10;
int head[N], e[N << 1], to[N << 1], w[N << 1], tot;
inline void add(int u, int v, int val)
{
    e[tot] = v, to[tot] = head[u], w[tot] = val, head[u] = tot++;
}

bool vis[N];
int dis[N], n, m;

int dijkstra()
{
    memset(dis, 0x3f, sizeof dis);
    dis[1] = 0;
    // 堆优化 dij。
    std::priority_queue<pii, std::vector<pii>, std::greater<pii>> q;
    q.push({0, 1});

    while (q.size())
    {
        auto tmp = q.top();
        q.pop();

        int u = tmp.second, dist = tmp.first;
        if (vis[u])
            continue;
        vis[u] = 1;
        for (int i = head[u]; ~i; i = to[i])
        {
            int v = e[i];
            if (dis[v] > dis[u] + w[i])
            {
                dis[v] = dis[u] + w[i];
                q.push({dis[v], v});
            }
        }
    }
    if (dis[n] == 0x3f3f3f3f)
        return ~0;
    return dis[n];
}

int main()
{
    scanf("%d %d", &n, &m);
    memset(head, -1, sizeof head);
    while (m--)
    {
        int a, b, c;
        scanf("%d %d %d", &a, &b, &c);
        add(a, b, c);
    }
    printf("%d", dijkstra());
    return 0;
}