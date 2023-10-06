/**
 * 插入数据不重复的红黑树。
 * 致谢：`https://github.com/sakeven/RbTree/blob/master/rbtree.go`
 * 以及: `https://github.com/whnw/rbtree/blob/master/rbtree.cpp`、`https://github.com/shetell/DataStructure/blob/main/BST.hpp`
 * 最后：`https://www.cs.usfca.edu/~galles/visualization/RedBlack.html`
 */
#ifndef __RBT_H__
#define __RBT_H__
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <queue>
#include <string>

enum Color
{
    RED,
    BLACK
};

template <class T>
class RBnode
{
public:
    Color _color;
    RBnode *_fa, *_lc, *_rc;
    T _val;
    RBnode() : _color(RED),
               _fa(nullptr),
               _lc(nullptr),
               _rc(nullptr) {}
    ~RBnode()
    {
        this->_fa = this->_lc = this->_rc = nullptr;
        this->_color = RED;
        this->_val;
    }
};
template <typename T>
void DebugPrintNode(RBnode<T> *node)
{
    printf("addr -> 0x%X, ", node);
    if (node != nullptr)
        printf("color = %d, (fa, lc, rc) = (0x%X, 0x%X, 0x%X), val = 0x%X",
               node->_color, node->_fa, node->_lc, node->_rc, node->_val);
    puts("");
}
/**
 * @brief  删除整棵树的节点。
 * @param *node: 当前的红黑树以 node 为根。并开始递归删除。
 * @return void
 */
template <typename T>
void destroyRBT(RBnode<T> *node)
{
    if (node == nullptr)
        return;
    if (node->_lc != nullptr)
        destroyRBT(node->_lc);
    if (node->_rc != nullptr)
        destroyRBT(node->_rc);
    delete node;
    return;
}

/**
 * 规则：
 * 1. 根和空叶子都是黑色。
 * 2. 节点非黑即红。
 * 3. 红节点没有红孩子。
 * 4. 从任意节点出发到达任意空叶子中遇到的黑节点个数**均一致**。
 */
template <typename T>
class RBT
{
public:
    RBnode<T> *root;         /// 根节点，使用时小心空值！！
    unsigned long long size; /// 整棵树的节点数。

    RBT()
    {
        this->size = 0;
        this->root = nullptr;
    }
    ~RBT()
    {
        this->size = 0;
        destroyRBT(this->root);
        this->root = nullptr;
    }

    RBnode<T> *search(T val);
    RBnode<T> *next(RBnode<T> *node);
    void insert(T val);
    void remove(T val);
    void preOrder(RBnode<T> *node)
    {
        if (node == nullptr)
            return;
        std::cout << node->_val << ' ';
        preOrder(node->_lc);
        preOrder(node->_rc);
    }
    void inOrder(RBnode<T> *node)
    {
        if (node == nullptr)
            return;
        inOrder(node->_lc);
        std::cout << node->_val << ' ';
        inOrder(node->_rc);
    }
    void postOrder(RBnode<T> *node)
    {
        if (node == nullptr)
            return;
        postOrder(node->_lc);
        postOrder(node->_rc);
        std::cout << node->_val << ' ';
    }
    void layerOrder(RBnode<T> *node)
    {
        std::queue<RBnode<T> *> q;
        q.push(node);
        while (!q.empty())
        {
            RBnode<T> *u = q.front();
            q.pop();
            if (u->_lc != nullptr)
                q.push(u->_lc);
            if (u->_rc != nullptr)
                q.push(u->_rc);
            std::cout << u->_val << ' ';
        }
        return;
    }

private:
    Color testColor(RBnode<T> *node)
    {
        if (node == nullptr)
            return BLACK;
        return node->_color;
    }
    void insertFix(RBnode<T> *node);
    void removeFix(RBnode<T> *node, RBnode<T> *fa);
    // 左右旋无需关心颜色。
    /**
     *     fa            fa
     *     |             |
     *    node   =>     [rc]
     *    / \           /  \
     *      [rc]      node  rrc
     *      /  \      / \
     *    lrc  rrc       lrc
     */
    void _Lrotate(RBnode<T> *node)
    {
        RBnode<T> *rc = node->_rc, *fa = node->_fa,
                  *lrc = rc->_lc;
        if (lrc != nullptr)
            lrc->_fa = node;
        node->_rc = lrc;

        if (fa == nullptr)
            this->root = rc;
        else if (fa->_lc == node)
            fa->_lc = rc;
        else
            fa->_rc = rc;

        rc->_fa = fa;
        node->_fa = rc;
        rc->_lc = node;
    }
    /**
     *       fa           fa
     *       |            |
     *      node   =>   [lc]
     *      /           /  \
     *    [lc]        llc  node
     *    /  \             / \
     *  llc  rlc         rlc
     */
    void _Rrotate(RBnode<T> *node)
    {
        RBnode<T> *lc = node->_lc, *fa = node->_fa,
                  *rlc = lc->_rc;
        if (rlc != nullptr)
            rlc->_fa = node;
        node->_lc = rlc;

        if (fa == nullptr)
            this->root = lc;
        else if (fa->_lc == node)
            fa->_lc = lc;
        else
            fa->_rc = lc;

        lc->_fa = fa;
        node->_fa = lc;
        lc->_rc = node;
    }
    RBnode<T> *findMax(RBnode<T> *node)
    {
        RBnode<T> *lst = node;
        while (node != nullptr)
        {
            lst = node;
            node = node->_rc;
        }
        return lst;
    }
    RBnode<T> *findMin(RBnode<T> *node)
    {
        RBnode<T> *lst = node;
        while (node != nullptr)
        {
            lst = node;
            node = node->_lc;
        }
        return lst;
    }
};
template <typename T>
Color __testColor(RBnode<T> *node)
{
    if (node == nullptr)
        return BLACK;
    return node->_color;
}
template <typename T>
void printTree(RBnode<T> *node, bool left, std::string const &indent)
{
    if (node->_lc)
        printTree(node->_lc, true, indent + (left ? "      " : "|     "));

    std::cout << indent;
    std::cout << '+'; // left ? "/" : "\\"
    std::cout << "-----";
    std::cout << '(' << node->_val << ' '
              << (__testColor(node) ? "BLACK" : "RED") << ")\n";
    if (node->_rc)
        printTree(node->_rc, false, indent + (left ? "|     " : "      "));
}
#include "./rbt.cpp"
#endif