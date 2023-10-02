// 不重复的红黑树。
// 致谢：https://github.com/sakeven/RbTree/blob/master/rbtree.go#L143
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <queue>
#include <iostream>

enum Color
{
    RED,
    BLACK
};

template <class T>
class RBnode
{ // TODO: 后期考虑在此处引入 cnt 形成可重复的红黑树？
  // 要看删除好不好做了。
private:
    Color _color;
    RBnode *_fa, *_lc, *_rc;
    T _val;

public:
    void setColor(Color setter)
    {
        this->_color = setter;
    }
    void setLc(RBnode *node) { this->_lc = node; }
    void setRc(RBnode *node) { this->_rc = node; }
    void setFa(RBnode *node) { this->_fa = node; }
    RBnode *getLc()
    {
        if (this == nullptr)
            return this;
        return this->_lc;
    }
    RBnode *getRc()
    {
        if (this == nullptr)
            return this;
        return this->_rc;
    }
    RBnode *getFa()
    {
        if (this == nullptr)
            return this;
        return this->_fa;
    }
    Color getColor() { return this->_color; }
    T getVal() { return this->_val; }
    RBnode(const T &val) : _val(val),
                           _color(RED),
                           _fa(nullptr),
                           _lc(nullptr),
                           _rc(nullptr) {}
    ~RBnode()
    {
        if (this->_lc != nullptr)
            delete this->_lc;
        if (this->_rc != nullptr)
            delete this->_rc;
        this->_fa = this->_lc = this->_rc = nullptr;
        this->_color = RED;
    }
};

/**
 * @brief  删除整棵树的节点。
 * @param *node: 当前的红黑树以 node 为根。并开始递归删除。
 * @return void
 */
template <typename T>
void destroyRBT(RBnode<T> *node)
{
    if (node->getLc() != nullptr)
        destroyRBT(node->getLc());
    if (node->getRc() != nullptr)
        destroyRBT(node->getRc());
    delete node;
    node = nullptr;
}

template <typename T>
class RBT
{
public:
    RBnode<T> *root;         // 根节点
    unsigned long long size; // 整棵树的节点数。

    RBT() : root(nullptr), size(0) {}
    ~RBT()
    {
        destroyRBT(this->root);
        this->size = 0;
    }
    // 树的大小。
    unsigned long long count() { return this->size; }

    RBnode<T> *find(T val);
    RBnode<T> *next(RBnode<T> *node);
    RBnode<T> *insert(T val);
    void insertFix(RBnode<T> *pos);
    void removeFix(RBnode<T> *pos, RBnode<T> *fa);
    void remove(T val);

private:
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
        RBnode<T> *rc = node->getRc();
        RBnode<T> *lrc = rc->getLc();

        node->setRc(lrc);
        if (lrc != nullptr)
            lrc->setFa(node);
        RBnode<T> *fa = node->getFa();

        if (fa == nullptr)
            this->root = rc;
        else if (fa->getLc() == node)
            fa->setLc(rc);
        else
            fa->setRc(rc);

        rc.setLc(node);
        node.setFa(rc);
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
        RBnode<T> *lc = node->getLc();
        RBnode<T> *rlc = lc->getRc();

        node->setLc(rlc);
        if (rlc != nullptr)
            rlc->setFa(node);
        RBnode<T> *fa = node->getFa();

        if (fa == nullptr)
            this->root = lc;
        else if (fa->getLc() == node)
            fa->setLc(lc);
        else
            fa->setRc(lc);

        lc.setRc(node);
        node.setFa(lc);
    }
    RBnode<T> *findMax(RBnode<T> *tmp)
    {
        if (tmp == nullptr)
            return tmp;
        while (tmp->getRc() != nullptr)
            tmp = tmp->getRc();
        return tmp;
    }
    RBnode<T> *findMin(RBnode<T> *tmp)
    {
        if (tmp == nullptr)
            return tmp;
        while (tmp->getLc() != nullptr)
            tmp = tmp->getLc();
        return tmp;
    }
};
