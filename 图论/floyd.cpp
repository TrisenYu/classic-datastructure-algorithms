#include <cstdio>
#include <cmath>
#include <cstring>
#include <iostream>

const int N = 2048;

int dis[N][N];
int st, ed;

void floyd()
{
    for (int i = st; i <= ed; i++)
        for (int j = st; j <= ed; j++)
            for (int k = st; k <= ed; k++)
                dis[i][j] = std::min(dis[i][j], dis[i][k] + dis[k][j]);
}

void printer()
{
    for (int i = st; i <= ed; i++, puts(""))
        for (int j = st; j <= ed; j++)
            printf("%d ", dis[i][j]);
}
// TODO: tester.
int main()
{
    st = 1;
    scanf("%d", &ed);
    for (int i = st; i <= ed; i++)
        for (int j = st; j <= ed; j++)
            scanf("%d", &dis[i][j]);

    floyd();
    printer();
    return 0;
}