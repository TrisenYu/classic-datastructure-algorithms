/* 使用 g++ avl_test.cpp -L. avl.so -o test 生成可执行程序。 */
#include "./avl.h"

//< ============================== 测试函数声明区域 ============================== >//

template <typename T>
void OrderTest(AVLnode<T> *tr)
{
    printf("[pre] ");
    OrderpaddingEnter(preOrder, tr);
    printf("[in] ");
    OrderpaddingEnter(inOrder, tr);
    printf("[post] ");
    OrderpaddingEnter(postOrder, tr);
    printf("[layer] ");
    OrderpaddingEnter(layerOrder, tr);
}

void deleteTest_ll(AVLnode<long long> **tr)
{
    puts("\nBEGIN DELETING...");
    *tr = AVLdelete(*tr, 12LL);
    OrderTest(*tr);

    *tr = AVLdelete(*tr, 5LL);
    OrderTest(*tr);

    *tr = AVLdelete(*tr, -1LL);
    OrderTest(*tr);

    *tr = AVLdelete(*tr, 3LL);
    OrderTest(*tr);

    *tr = AVLdelete(*tr, 4LL);
    OrderTest(*tr);

    *tr = AVLdelete(*tr, 1ll << 31);
    OrderTest(*tr);

    *tr = AVLdelete(*tr, -(1ll << 31));
    OrderTest(*tr);
    puts("END DELETING...");
}
void searchRepeatedlyTest_ll(AVLnode<long long> *tr)
{
    AVLnode<long long> *tmp = nullptr;
    printf("\nparam: addr[0x%X]", tr);
    puts("\nBEGIN SEARCHING...");

    tmp = AVLsearchRepeatedly(tr, -1LL);
    if (tmp)
        printf("tmp-addr: 0x%X, val = %d\n", tmp, tmp->val);
    tmp = AVLsearchRepeatedly(tr, 11LL);
    if (tmp)
        printf("tmp-addr: 0x%X, val = %d\n", tmp, tmp->val);
    tmp = AVLsearchRepeatedly(tr, 10LL);
    if (tmp)
        printf("tmp-addr: 0x%X, val = %d\n", tmp, tmp->val);
    tmp = AVLsearchRepeatedly(tr, 9LL);
    if (tmp)
        printf("tmp-addr: 0x%X, val = %d\n", tmp, tmp->val);
    tmp = AVLsearchRepeatedly(tr, 8LL);
    if (tmp)
        printf("tmp-addr: 0x%X, val = %d\n", tmp, tmp->val);
    tmp = AVLsearchRepeatedly(tr, -1LL);
    if (tmp)
        printf("tmp-addr: 0x%X, val = %d\n", tmp, tmp->val);
    puts("END SEARCHING......");
}
void searchRecursively_ll(AVLnode<long long> *tr)
{
    AVLnode<long long> *tmp = nullptr;
    tmp = AVLsearchRecursively(tr, 15LL);
    if (tmp)
        printf("tmp-addr: 0x%X, val = %d\n", tmp, tmp->val);
    tmp = AVLsearchRecursively(tr, -100LL);
    if (tmp)
        printf("tmp-addr: 0x%X, val = %d\n", tmp, tmp->val);
    tmp = AVLsearchRecursively(tr, -10LL);
    if (tmp)
        printf("tmp-addr: 0x%X, val = %d\n", tmp, tmp->val);
    tmp = AVLsearchRecursively(tr, -20LL);
    if (tmp)
        printf("tmp-addr: 0x%X, val = %d\n", tmp, tmp->val);
    tmp = AVLsearchRecursively(tr, 2LL);
    if (tmp)
        printf("tmp-addr: 0x%X, val = %d\n", tmp, tmp->val);
}

void deleteTest_double(AVLnode<double> **tr)
{
    puts("\nBEGIN DELETING...");
    *tr = AVLdelete(*tr, 12.0);
    OrderTest(*tr);

    *tr = AVLdelete(*tr, -12.0);
    OrderTest(*tr);

    *tr = AVLdelete(*tr, -1e7 + 10);
    OrderTest(*tr);

    *tr = AVLdelete(*tr, 0.03);
    OrderTest(*tr);

    *tr = AVLdelete(*tr, -0.09);
    OrderTest(*tr);

    *tr = AVLdelete(*tr, 1145141919810.0);
    OrderTest(*tr);

    *tr = AVLdelete(*tr, 1145141919810.0);
    OrderTest(*tr);
    puts("END DELETING...");
}
void searchRepeatedlyTest_double(AVLnode<double> *tr)
{
    AVLnode<double> *tmp = nullptr;
    printf("\nparam: addr[0x%X]", tr);
    puts("\nBEGIN SEARCHING...");

    tmp = AVLsearchRepeatedly(tr, -1.1200351);
    if (tmp)
        printf("tmp-addr: 0x%X, val = %d\n", tmp, tmp->val);
    tmp = AVLsearchRepeatedly(tr, 3.14159265358979323);
    if (tmp)
        printf("tmp-addr: 0x%X, val = %d\n", tmp, tmp->val);
    tmp = AVLsearchRepeatedly(tr, 100086.10001);
    if (tmp)
        printf("tmp-addr: 0x%X, val = %d\n", tmp, tmp->val);
    tmp = AVLsearchRepeatedly(tr, 1314.520);
    if (tmp)
        printf("tmp-addr: 0x%X, val = %d\n", tmp, tmp->val);
    tmp = AVLsearchRepeatedly(tr, -9563131.0231);
    if (tmp)
        printf("tmp-addr: 0x%X, val = %d\n", tmp, tmp->val);
    tmp = AVLsearchRepeatedly(tr, 0.0000000036891);
    if (tmp)
        printf("tmp-addr: 0x%X, val = %d\n", tmp, tmp->val);
    puts("END SEARCHING......");
}

//< ============================= 测试函数声明区域结束 ============================= >//

int main()
{
    AVLnode<long long> *tr = nullptr;
    for (long long i = 1; i <= 10; i++)
        tr = AVLinsert(tr, i);

    OrderTest(tr);
    deleteTest_ll(&tr);
    searchRepeatedlyTest_ll(tr);

    for (long long i = -10; i <= -1; i++)
        tr = AVLinsert(tr, i);

    OrderTest(tr);
    searchRepeatedlyTest_ll(tr);
    deleteTest_ll(&tr);

    for (long long i = 200; i > 50; i--)
        tr = AVLinsert(tr, i);
    OrderTest(tr);
    deleteTest_ll(&tr);
    searchRepeatedlyTest_ll(tr);

    searchRecursively_ll(tr);
    destroyAVLtr(tr);
    tr = nullptr;

    AVLnode<double> *ptr = nullptr;
    double l = -1, r = 2;
    for (; l < r; l += 0.001)
        ptr = AVLinsert(ptr, l);
    deleteTest_double(&ptr);
    searchRepeatedlyTest_double(ptr);
    deleteTest_double(&ptr);

    destroyAVLtr(ptr);
    ptr = nullptr;
    return 0;
}
