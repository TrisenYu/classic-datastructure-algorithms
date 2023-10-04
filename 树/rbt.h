/**
 * 插入数据不重复的红黑树。
 * 致谢：`https://github.com/sakeven/RbTree/blob/master/rbtree.go`
 * 以及: `https://github.com/whnw/rbtree/blob/master/rbtree.cpp`、`https://github.com/shetell/DataStructure/blob/main/BST.hpp`
 * 最后：`https://www.cs.usfca.edu/~galles/visualization/RedBlack.html`
 */

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
    std::cout << (left ? '/' : '\\');
    std::cout << "-----";
    std::cout << node->_val << ' '
              << (__testColor(node) ? "BLACK" : "RED") << '\n';
    if (node->_rc)
        printTree(node->_rc, false, indent + (left ? "|     " : "      "));
}

template <typename T>
RBnode<T> *RBT<T>::search(T val)
{
    RBnode<T> *node = this->root, *lst = nullptr;
    while (node != nullptr)
    {
        lst = node;
        if (node->_val == val)
            break; // 已经存在。
        else if (node->_val < val)
            node = node->_rc;
        else
            node = node->_lc;
    }
    return lst;
}
// 严格找到后继。
template <typename T>
RBnode<T> *RBT<T>::next(RBnode<T> *node)
{
    if (node->_rc != nullptr)
        return this->findMin(node->_rc);
    RBnode<T> *fa = node;
    while (fa != nullptr && fa->_rc == node)
        node = fa, fa = node->_fa;
    return fa;
}

template <typename T>
void RBT<T>::insertFix(RBnode<T> *node)
{
    /**
     * 循环中的两个最大的判断分别的示意如下。
     * (1) # 以父亲位于祖父左侧的情况为例。右侧的情况对称。
     *        Bgra               Rgra
     *        / \                /  \
     *     Rfa   Runcle  =>     Bfa Buncle   # 然后把祖父当成儿子来重复到根为止。
     *      |                    |
     *    Rcur                 Rcur
     *
     * (2) # 以父亲位于祖父左侧的情况为例。右侧的情况对称。
     *      Bgra                    Rgra
     *      /  \             1      /  \
     *    Rfa [Buncle, Bnil] =>   Bcur  [Buncle, Bnil]
     *     \                      /
     *      Rcur                Rfa
     *
     *                             Bcur
     *                       2    /    \
     *                       => Rfa    Rgra
     *                                   \
     *                                  [Buncle, Bnil]
     *
     * # (2)中的另外一种情况免除第一步左旋调整。使父亲变祖父即可。
     */
    RBnode<T> *fa = node->_fa, *uncle = nullptr, *gra = nullptr;
    while (node != nullptr && node != this->root && node->_fa->_color == RED)
    {
        fa = node->_fa, gra = fa->_fa;

        bool sign = gra->_lc == fa;
        uncle = sign ? gra->_rc : gra->_lc;

        if (uncle != nullptr && uncle->_color == RED)
        {
            uncle->_color = (fa->_color = BLACK);
            gra->_color = RED;
            node = gra;
        }
        else
        {
            if (sign && node == fa->_rc)
            {
                this->_Lrotate(fa);
                fa = gra->_lc;
                node = fa->_lc;
            }
            else if (!sign && node == fa->_lc)
            {
                this->_Rrotate(fa);
                fa = gra->_rc;
                node = fa->_rc;
            }
            sign ? this->_Rrotate(gra) : this->_Lrotate(gra);
            fa->_color = BLACK, gra->_color = RED;
        }
    }
    this->root->_color = BLACK;
}

template <typename T>
void RBT<T>::insert(T val)
{
    RBnode<T> *node = this->root, *lst = this->root;

    while (node != nullptr)
    {
        lst = node;
        if (node->_val == val)
            return;
        else if (node->_val > val)
            node = node->_lc;
        else
            node = node->_rc;
    }

    node = new RBnode<T>;
    node->_val = val;
    node->_lc = node->_rc = nullptr;
    this->size++;

    if (this->root == nullptr)
    {
        node->_color = BLACK;
        node->_fa = nullptr;
        this->root = node;
        return;
    }
    RBnode<T> *fa = lst;

    if (fa->_val > node->_val)
        fa->_lc = node;
    else
        fa->_rc = node;

    node->_fa = fa;
    if (fa->_color == RED)
        this->insertFix(node);

    return;
}

template <typename T>
void RBT<T>::removeFix(RBnode<T> *node, RBnode<T> *fa)
{
    RBnode<T> *bro = nullptr;
    while (node != this->root && this->testColor(node) == BLACK)
    {
        if (node != nullptr)
            fa = node->_fa;

        bool sign = fa->_lc == node;
        bro = sign ? fa->_rc : fa->_lc;
        /**
         * 以 Bpos 在左边的情况为例。
         *        fa                   Bbro
         *       /  \                  /  \
         *    Bpos   Rbro    =>       Rfa  Br
         *   <Bnil>                  /  \
         *                        Bpos  [Bnil, Bbro_lc]
         */
        if (this->testColor(bro) == RED)
        {
            bro->_color = BLACK;
            fa->_color = RED;
            sign ? this->_Lrotate(fa) : this->_Rrotate(fa);
            bro = sign ? fa->_rc : fa->_lc;
        }
        if (this->testColor(bro->_lc) == BLACK &&
            this->testColor(bro->_rc) == BLACK)
            bro->_color = RED, node = fa;
        else // 至少有一个是不黑的。
        {
            /**
             * 以在左边的情况为例。
             *        Rfa
             *       /   \
             *    Bpos   Bbro
             *           / \
             *     sibling  son
             */
            RBnode<T> *son = sign ? bro->_rc : bro->_lc,
                      *sibling = sign ? bro->_lc : bro->_rc;
            if (this->testColor(son) == BLACK)
            {
                bro->_color = RED;
                if (sibling != nullptr)
                    sibling->_color = BLACK;
                sign ? this->_Rrotate(bro) : this->_Lrotate(bro);
                bro = sign ? fa->_rc : fa->_lc;
            }

            bro->_color = fa->_color;
            fa->_color = BLACK;

            son = sign ? bro->_rc : bro->_lc;
            son->_color = BLACK;
            sign ? this->_Lrotate(fa) : this->_Rrotate(fa);
            node = this->root;
        }
    }
    if (node != nullptr)
        node->_color = BLACK;
}

template <typename T>
void RBT<T>::remove(T val)
{
    RBnode<T> *node = this->search(val);
    if (node == nullptr)
        return;

    RBnode<T> *successor = nullptr,
              *nchild = nullptr,
              *fa = node->_fa;

    if (node->_lc != nullptr && node->_rc != nullptr)
        successor = this->next(node);
    else
        successor = node;

    if (successor->_lc != nullptr)
        nchild = successor->_lc;
    else
        nchild = successor->_rc;

    fa = successor->_fa;

    if (nchild != nullptr)
        nchild->_fa = fa;

    if (fa == nullptr)
        this->root = nchild;
    else if (fa->_lc == successor)
        fa->_lc = nchild;
    else
        fa->_rc = nchild;

    if (successor != node)
        node->_val = successor->_val;

    if (successor->_color == BLACK)
        this->removeFix(nchild, fa);

    this->size--;
    if (!this->size)
        this->root = nullptr;

    delete successor;
}