#include "./rbt.h"

int main()
{
	RBT<long long> tr;

	for (int i = 0; i < 4; i++)
	{
		tr.insert((long long)i);
		tr.preOrder(tr.root);
		puts("");
	}
	puts("----------------------------PASS1----------------------------");
	for (int i = 65; i > 30; i--)
	{
		tr.insert((long long)i);
		tr.inOrder(tr.root);
		printf("\t[root = %lld, size = %llu]\n", tr.root->_val, tr.size);
	}
	puts("----------------------------PASS2----------------------------");
	for (int i = -5; i <= 30; i++)
	{
		tr.insert((long long)i);
		tr.postOrder(tr.root);
		puts("");
	}
	printf("%lld\n", tr.root->_val);
	puts("----------------------------TEST 2.5 ----------------------------");

	for (int i = 65; i > 30; i--)
	{
		tr.insert((long long)i);
		tr.inOrder(tr.root);
		printf("\t[root = %lld, size =  %llu]\n", tr.root->_val, tr.size);
	}
	puts("----------------------------PASS3----------------------------");
	printf("0x%X\n", tr.search(15LL));

	printf("0x%X\n", tr.search(114514LL));

	for (long long i = -116; i <= -1; i++)
	{
		tr.insert(i);
		tr.inOrder(tr.root);
		puts("\n");
		tr.preOrder(tr.root);
		puts("\n-----------");
		// tr.layerOrder(tr.root);
	}
	puts("----------------------------PASS4----------------------------");
	printf("[%lld, size =  %llu]\n", tr.root->_val, tr.size);

	for (int i = 0; i < 17; i++)
	{
		tr.remove((long long)i);
		// tr.inOrder(tr.root);
	}
	printf("\t[root = %lld, size = %llu]\n", tr.root->_val, tr.size);
	puts("----------------------------PASS5----------------------------");

	for (int i = -100; i <= 100; i++)
	{
		tr.remove((long long)i);
		if (tr.root != nullptr)
			printf("[root = %lld]\t", tr.root->_val);
		printf("[size = %llu]\n", tr.size);
	}

	if (tr.root != nullptr)
		printf("\t[root = %lld, size = %llu]\n", tr.root->_val, tr.size);
	printf("[size = %llu]\n", tr.size);
	puts("----------------------------PASS6----------------------------");

	// 8MB 的内存插入测试。
	for (int i = 1 << 20; i < 1 << 21; i++)
		tr.insert((long long)i);

	printf("\t[root = %lld, size = %llu]\n", tr.root->_val, tr.size);
	puts("----------------------------PASS7----------------------------");

	system("pause");
	tr.~RBT();
	return 0;
}
