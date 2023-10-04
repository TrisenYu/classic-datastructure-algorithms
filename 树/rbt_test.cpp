#include "./rbt.h"
void TEST1(RBT<long long> &tr);
void TEST2(RBT<double> &tr);

int main()
{
	RBT<long long> tr;
	TEST1(tr);
	tr.~RBT();
	system("pause");
	/*
	RBT<double> dtr;
	TEST2(dtr);
	dtr.~RBT();
	system("pause");
	*/
	return 0;
}
void TEST1(RBT<long long> &tr)
{
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
		// tr.inOrder(tr.root);
		printf("\t[root = %lld, size = %llu]\n", tr.root->_val, tr.size);
	}
	printTree(tr.root->_lc, true, "");
	printf("%lld BLACK\n", tr.root->_val);
	printTree(tr.root->_rc, false, "");

	puts("----------------------------PASS2----------------------------");
	for (int i = -5; i <= 30; i++)
	{
		tr.insert((long long)i);
		// tr.postOrder(tr.root);
		// puts("");
	}
	printf("%lld\n", tr.root->_val);
	for (int i = 65; i > 30; i--)
	{
		tr.insert((long long)i);
		// tr.inOrder(tr.root);
		printf("\t[root = %lld, size =  %llu]\n", tr.root->_val, tr.size);
	}
	puts("----------------------------PASS3----------------------------");
	printf("0x%X\n", tr.search(15LL));

	printf("0x%X\n", tr.search(114514LL));
	for (long long i = -116; i <= -1; i++)
	{
		tr.insert(i);
		// tr.inOrder(tr.root);
		// puts("\n");
		// tr.preOrder(tr.root);
		// puts("\n-----------");
		// tr.layerOrder(tr.root);
	}
	puts("----------------------------PASS4----------------------------");

	printTree(tr.root->_lc, true, "");
	printf("%lld BLACK\n", tr.root->_val);
	printTree(tr.root->_rc, false, "");

	puts("----------------------------PASS4.5----------------------------");

	printf("[%lld, size =  %llu]\n", tr.root->_val, tr.size);

	for (int i = 0; i < 17; i++)
		tr.remove((long long)i);
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
}

void TEST2(RBT<double> &tr)
{
	for (double i = 0; i < 2; i += 0.001)
	{
		tr.insert(i);
		puts("");
	}
	puts("----------------------------PASS1----------------------------");
	for (double i = 65; i > 50; i -= 0.01)
	{
		tr.insert(i);
		printf("\t[root = %lf, size = %llu]\n", tr.root->_val, tr.size);
	}
	puts("----------------------------PASS2----------------------------");
	for (double i = -50; i <= 300; i += 1)
		tr.insert(i);
	printf("%lld\n", tr.root->_val);
	printf("0x%X\n", tr.search(15.0));
	printf("0x%X\n", tr.search(114514.0));
	printf("0x%X\n", tr.search(-114514.0));

	printf("[%lld, size =  %llu]\n", tr.root->_val, tr.size);

	for (double i = 65; i > 30; i -= 0.01)
		tr.remove(i);
	printf("\t[root = %lf, size = %llu]\n", tr.root->_val, tr.size);
	puts("----------------------------PASS3----------------------------");

	for (double i = -50; i <= 300; i += 1)
	{
		tr.remove(i);
		if (tr.root != nullptr)
			printf("[root = %lf]\t", tr.root->_val);
		printf("[size = %llu]\n", tr.size);
	}
	for (double i = -50; i <= 300; i += 1)
	{
		tr.remove(i);
		if (tr.root != nullptr)
			printf("[root = %lf]\t", tr.root->_val);
		printf("[size = %llu]\n", tr.size);
	}
	if (tr.root != nullptr)
		printf("\t[root = %lf, size = %llu]\n", tr.root->_val, tr.size);
	printf("[size = %llu]\n", tr.size);
	puts("----------------------------PASS4----------------------------");
}