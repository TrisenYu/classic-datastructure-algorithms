#include <cstdio>
#include <cstring>
#include <cmath>
#include <iostream>

using namespace std;
const int M = 20010, N = 5010;
int n, m;
int head[N], deg[N], to[M], e[M], tot;
inline void add(int u, int v)
{
    e[tot] = v, to[tot] = head[u], head[u] = tot++;
}

int low[N], dfn[N], timestamp,
    same_col[N], col_cnt[N], idx,
    stk[N], top;
bool brige[M];

void tar(int u, int from)
{
    dfn[u] = low[u] = ++timestamp;
    stk[++top] = u;

    for (int i = head[u]; ~i; i = to[i])
    {
        int v = e[i];
        if (!dfn[v])
        {
            tar(v, i);
            low[u] = min(low[u], low[v]);

            if (dfn[u] < low[v]) // 桥边
                brige[i] = brige[i ^ 1] = 1;
        }
        else if (i ^ (from ^ 1))
            low[u] = min(low[u], dfn[v]);
    }

    if (low[u] == dfn[u])
    {
        int coly;
        idx++;
        do
        {
            coly = stk[top--];
            same_col[coly] = idx;
        } while (coly != u);
    }
}

int main()
{

    memset(head, ~0, sizeof head);
    scanf("%d %d", &n, &m);
    while (m--)
    {
        int a, b;
        scanf("%d %d", &a, &b);
        add(a, b), add(b, a);
    }
    tar(1, ~0);

    for (int i = 0; i < tot; i++)
        if (brige[i])
            ++deg[same_col[e[i]]];

    int res = 0;
    for (int i = 1; i <= idx; i++)
        if (deg[i] == 1)
            ++res;
    printf("%d", (res + 1) / 2);

    return 0;
}