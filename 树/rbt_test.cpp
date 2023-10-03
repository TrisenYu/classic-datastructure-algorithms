#include "./rbt.h"

int main()
{
	RBT<long long> tr;
	for (int i = -10; i < 20; i++)
		tr.insert((long long)i);
	printf("0x%X", tr.find(15LL));

	return 0;
}
