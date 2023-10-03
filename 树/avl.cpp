// g++ avl.cpp -fPIC -shared -o avl.so # 编译成动态链接库
#include "./avl.h"
// 很遗憾，模板类的声明和定义不能分开。
// 编译的时候这个文件少不了。