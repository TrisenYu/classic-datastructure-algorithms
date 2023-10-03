#include "./rbt.h"

int main()
{
	RBT<long long> tr;

	for (int i = 0; i < 4; i++)
	{
		tr.insert((long long)i);
		tr.preOrder(tr.root);
		printf("\t[root = %lld, size = %llu]\n", tr.root->getVal(), tr.size);
	}
	puts("----------------------------PASS1----------------------------");
	for (int i = 65; i > 30; i--)
	{
		tr.insert((long long)i);
		tr.postOrder(tr.root);
		printf("\t[root = %lld, size = %llu]\n", tr.root->getVal(), tr.size);
	}
	puts("----------------------------PASS2----------------------------");
	tr.layerColor();
	puts("----------------------------TEST 2.5 ----------------------------");
	system("pause");
	tr.~RBT();
	return 0;
	for (int i = 65; i > 30; i--)
	{
		tr.insert((long long)i);
		tr.inOrder(tr.root);
		printf("\t[root = %lld, size =  %llu]\n", tr.root->getVal(), tr.size);
	}
	puts("----------------------------PASS3----------------------------");
	printf("0x%X\n", tr.find(15LL));

	printf("0x%X\n", tr.find(114514LL));
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

	for (int i = 0; i < 17; i++)
	{
		tr.remove((long long)i);
		tr.inOrder(tr.root);
		printf("\t[root = %lld, size =  %llu]\n", tr.root->getVal(), tr.size);
	}
	puts("----------------------------PASS5----------------------------");
}
