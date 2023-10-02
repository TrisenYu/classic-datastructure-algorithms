#include <stdio.h>
#include <iostream>
#include <string.h>
const int N = 1E4 + 10;
/** 在 txt 中寻找到第一个 str 的位置。
 * 暴力匹配做法：从 txt 的开头匹配到最后能匹配的位置
 * 只要中途不对就直接退出第二层循环。不然就一定可以走完循环然后返回 txt 中的下标即可。
 * @param str: 目的字符串。
 * @param txt: 文本字符串。
 * @todo: 做法在重复字符串非常多的时候就非常的低效。需要推翻。
 */
int brute_match(char *str, char *txt)
{
    int n = strlen(str), m = strlen(txt);
    for (int i = 0; i < m; i++)
    {
        int j = 0;
        for (; j < n && i + j < m; j++)
            if (str[j] != txt[i + j])
                break;
        if (j == n)
            return i;
    }
    return -1;
}

int nexter[N][256]; // 记当前状态为 i，所遇到的字符是 j 时的下一个状态。
// 实际上就是利用 str 建立具有边权的有穷状态自动机
// 通过 txt 中下一个字符来确定该从哪里开始匹配。
int kmp(char *str, char *txt)
{
    int n = strlen(str), m = strlen(txt);

    int fail_state = 0;
    nexter[0][str[0]] = 1;
    for (int state = 1; state < n; state++)
    {
        for (int c = 0; c < 256; c++)
            nexter[state][c] = nexter[fail_state][c];

        nexter[state][str[state]] = state + 1;
        fail_state = nexter[fail_state][str[state]];
    }
    for (int i = 0; i < n; i++, puts(""))
        for (int c = 'a'; c < 'z' + 1; c++)
            printf("%d ", nexter[i][c]);
    int pos_state = 0;
    for (int i = 0; i < m; i++)
    {
        pos_state = nexter[pos_state][txt[i]];
        if (pos_state == n)
            return i - n + 1;
    }
    return -1;
}

// 来优化二维数组成一维数组。
// 状态自动机的建立方法在某些场景下会被卡空间。
// 但是不难观察，失配指针是根据 str 中的最大前缀串和 txt 中的最大后缀串而确定的。
char partten[N], text[N];
int nxt[N];
/** 生成过程：
 *                                       i-1 i
 *              L                    R    | /
 * str:    |_________|__________|_________|?.........
 * str:    +---------|1-------------------+?------...
 *         |<M  A  T ||    C  H  E  D    >|
 *         |        /  \
 *         0  n[i]-1  n[i]
 *             ni-1    ni
 *              |       |
 *             j-1      j               # 记录的同时还兼负“问责”的一个
 *
 * 也就是，下一位能匹配当前匹配时，将下一位纳入模式串，并更新当前长度下的最长双缀。
 * if(str[i] == str[j]) j ++, i ++;     # 也意味着此时 str(0) == str(1)
 *
 * 如果不等，就移动到失败指针所指向的位置。并准备反复判断。
 *
 *                                       i-1 i
 *              L                    R    | /
 * str:   |_________|__________|______(___|?)........
 * str:                       (+--|2)--|--|1-------------------+?------...
 *                             |  ||      ||
 *                             0  / \    /  \
 *                               |   |  j-1  j
 *                             nj-1  nj    x 已经失败了，所以跳转
 *
 *                                       i-1 i
 *              L                    R    | /
 * str:   |_________|__________|______(___|?)........
 * str:                               (+--|2)--|--|1-------------------+?------...
 *                                     |  ||      ||
 *                                     0  / \    /  \
 *                                       |   |  j-1  j
 *                                     nj-1  nj
 *  如不等，判断最长双缀内的最长双缀的后一位是否能匹配：str(2) == str(i)
 *  直到最后变成空串为止。此处显示继续失败时的做法。
 *                                       i-1 i
 *              L                    R    | /
 * str:   |_________|__________|______(___|?)........
 * str:                                  +|3|2--|--|1-------------------+?------...
 *
 *
 *
 */
int test(char *str, char *txt)
{
    int n = strlen(str), m = strlen(txt);

    for (int i = 1, j = 0; i < n; i++)
    {
        while (j && str[i] != str[j])
            j = nxt[j];
        if (str[i] == str[j])
            j++;

        nxt[i + 1] = j;
    }

    for (int i = 0, j = 0; i < m; i++)
    {
        while (j && str[j] != txt[i])
            j = nxt[j];
        if (str[j] == txt[i])
            j++;

        if (j == n)
            return i - n + 1;
    }
    return ~0;
}
int fail[N];
int simplifiedKMP(char *str, char *txt)
{
    int n = strlen(txt), m = strlen(str), i = 1, j = 0;
    while (i < m)
    {
        while (j && str[j] ^ str[i])
            j = fail[j];
        fail[++i] = (j += (str[j] == txt[i]));
    }
    i = j = 0;
    while (i < n)
    {
        while (j && str[j] ^ txt[i])
            j = fail[j];
        j += (str[j] == txt[i]);
        if (j == m)
            return i - m + 1; // 视情况来操作此处
        i++;
    }
    return ~0;
}
int main()
{
    std::cin >> partten;
    std::cin >> text;
    // printf("%d\n", brute_match(partten, text));
    printf("%d\n", kmp(partten, text));
    printf("%d\n%d", test(partten, text),
           simplifiedKMP(partten, text));
    return 0;
}