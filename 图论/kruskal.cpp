#include <cstdio>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <queue>

const int N = 1E5 + 10;

int n, m, fa[N];
struct edge
{
    int u, v, w;
    bool operator<(const edge &a) const
    {
        return w < a.w;
    }
} arr[N << 1];

int find(int x)
{
    return x == fa[x] ? x : fa[x] = find(fa[x]);
}
int kruskal()
{
    for (int i = 1; i <= n; i++)
        fa[i] = i;
    std::sort(arr, arr + m);
    int res = 0, cnt = 0;
    for (int i = 0; i < m; i++)
    {
        int u = arr[i].u, v = arr[i].v, w = arr[i].w;
        // 并查集合并。
        int fau = find(u), fav = find(v);
        if (fau != fav)
        {
            fa[fau] = fav;
            cnt += 1,
                res += w;
        }
    }
    if (cnt != n - 1)
        return 0x3f3f3f3f;
    return res;
}

int main()
{
    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; i++)
    {
        int a, b, c;
        scanf("%d %d %d", &a, &b, &c);
        arr[i] = {a, b, c};
    }
    int res = kruskal();
    if (res == 0x3f3f3f3f)
        puts("impossible");
    else
        printf("%d", res);
    return 0;
}