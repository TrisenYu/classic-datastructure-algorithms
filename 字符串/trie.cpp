#include <cstdio>
const int N = 100010;

int tr[N][26], cnt[N], idx;
char str[N];

void insert(char *str)
{
    int state = 0;
    for (int i = 0; str[i]; i++)
    {
        int c = str[i] - 'a'; // 只含有小写字母的字典树。
        if (!tr[state][c])
            tr[state][c] = ++idx;
        state = tr[state][c];
    }

    cnt[state]++; // 使终止态自增从而方便计算匹配数量。
}

int query(char *str)
{
    int state = 0;
    for (int i = 0; str[i]; i++)
    {
        int c = str[i] - 'a';
        if (!tr[state][c])
            return 0;
        state = tr[state][c];
    }

    return cnt[state];
}

int main()
{
    int n;
    scanf("%d", &n);
    while (n--)
    {
        char op[2];
        scanf("%s%s", op, str);
        if (*op == 'I')
            insert(str);
        else
            printf("%d\n", query(str));
    }

    return 0;
}