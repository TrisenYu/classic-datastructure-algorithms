#include "./rbt.h"

int main()
{
	RBT<long long> tr;

	for (int i = -10; i < 20; i++)
	{
		tr.insert((long long)i);
		tr.preOrder(tr.root);
		printf("\t[root = %lld]\n", tr.root->getVal());
	}
	puts("----------------------------PASS1----------------------------");
	for (int i = 50; i > 30; i--)
	{
		tr.insert((long long)i);
		tr.postOrder(tr.root);
		printf("\t[root = %lld]\n", tr.root->getVal());
	}
	puts("----------------------------PASS2----------------------------");
	for (int i = 65; i > 30; i--)
	{
		tr.insert((long long)i);
		tr.inOrder(tr.root);
		printf("\t[root = %lld]\n", tr.root->getVal());
	}
	puts("----------------------------PASS3----------------------------");
	printf("0x%X\n", tr.find(15LL));

	printf("0x%X", tr.find(114514LL));

	for (int i = 0; i < 17; i++)
		tr.remove((long long)i),
			tr.inOrder(tr.root),
			printf("\t[root = %lld]\n", tr.root->getVal());
	puts("----------------------------PASS4----------------------------");

	tr.~RBT();
	return 0;
}
