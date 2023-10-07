#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <queue>
const int N = 512;

int g[N][N];
bool vis[N];
int dis[N], n, m;

int prime()
{
    memset(dis, 0x3f, sizeof dis);
    int res = 0;
    // 找一个离当前点最近的且第一次遇到的点然后比较距离。
    for (int i = 0; i < n; i++)
    {
        int t = ~0;
        for (int j = 1; j <= n; j++)
            if (!vis[j] && (!~t || dis[t] > dis[j]))
                t = j;
        if (i && dis[t] == 0x3f3f3f3f)
            return dis[t];

        if (i)
            res += dis[t];
        vis[t] = 1;

        for (int j = 1; j <= n; j++)
            dis[j] = std::min(dis[j], g[t][j]);
    }
    return res;
}

int main()
{
    scanf("%d %d", &n, &m);
    memset(g, 0x3f, sizeof g);
    while (m--)
    {
        int a, b, c;
        scanf("%d %d %d", &a, &b, &c);
        // 照理说应该是这样的。但是题目没说给过来的数据没有剔除冗余边。
        // g[a][b] = g[b][a] = c;
        g[a][b] = g[b][a] = std::min(c, g[a][b]);
    }
    int res = prime();
    if (res == 0x3f3f3f3f)
        puts("impossible");
    else
        printf("%d", res);
    return 0;
}