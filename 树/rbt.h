// 不重复的红黑树。
// 致谢：`https://github.com/sakeven/RbTree/blob/master/rbtree.go`
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <iostream>

enum Color
{
    RED,
    BLACK
};

template <class T>
class RBnode
{
    // TODO: 后期考虑在此处引入 cnt 形成可重复的红黑树？
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
    void setVal(const T &val) { this->_val = val; }
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
    Color getColor() { return this == nullptr ? BLACK : this->_color; }
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

/*
template <typename T>
void DebugprintMsg(RBnode<T> *node)
{
    printf("(node.fa, node.lc, node.rc) = (0x%X, 0x%X, 0x%X)", node->getFa(), node->getLc(), node->getRc());
    printf(", (facol, nodecol) = (%d, %d)", node->getFa()->getColor(), node->getColor());
    if (node != nullptr)
        printf(", val = %lld", node->getVal());
    puts("");
}
*/

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

    void preOrder(RBnode<T> *tmp)
    {
        if (tmp == nullptr)
            return;
        std::cout << tmp->getVal() << ' ';
        preOrder(tmp->getLc());
        preOrder(tmp->getRc());
    }
    void inOrder(RBnode<T> *tmp)
    {
        if (tmp == nullptr)
            return;
        inOrder(tmp->getLc());
        std::cout << tmp->getVal() << ' ';
        inOrder(tmp->getRc());
    }
    void postOrder(RBnode<T> *tmp)
    {
        if (tmp == nullptr)
            return;
        postOrder(tmp->getLc());
        postOrder(tmp->getRc());
        std::cout << tmp->getVal() << ' ';
    }

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
    void _Lrotate(RBnode<T> **node)
    {

        RBnode<T> *rc = (*node)->getRc();
        RBnode<T> *lrc = rc->getLc();
        (*node)->setRc(lrc);
        if (lrc != nullptr)
            lrc->setFa((*node));

        RBnode<T> *fa = (*node)->getFa();

        if (fa == nullptr)
            this->root = rc;
        else if (fa->getLc() == (*node))
            fa->setLc(rc);
        else
            fa->setRc(rc);

        rc->setFa(fa);
        rc->setLc((*node));

        (*node)->setFa(rc);
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
    void _Rrotate(RBnode<T> **node)
    {
        RBnode<T> *lc = (*node)->getLc();
        RBnode<T> *rlc = lc->getRc();

        (*node)->setLc(rlc);
        if (rlc != nullptr)
            rlc->setFa(*node);
        RBnode<T> *fa = (*node)->getFa();

        if (fa == nullptr)
            this->root = lc;
        else if (fa->getLc() == (*node))
            fa->setLc(lc);
        else
            fa->setRc(lc);

        lc->setFa(fa);
        lc->setRc(*node);
        (*node)->setFa(lc);
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

template <typename T>
RBnode<T> *RBT<T>::find(T val)
{
    RBnode<T> *tmp = root;
    while (tmp != nullptr)
    {
        T test = tmp->getVal();
        if (test == val)
            break;
        else if (test < val)
            tmp = tmp->getRc();
        else
            tmp = tmp->getLc();
    }
    return tmp;
}
/**
 * 中序遍历（左中右）即为升序。
 * 所以找左边最大或者右边最小。如果没有孩子的，返回自己。
 *         <         >
 *          \       /
 *             ...
 *      中 程 不 可 存 在 拐 点
 */
template <typename T>
RBnode<T> *RBT<T>::next(RBnode<T> *node)
{
    if (node->getRc() != nullptr)
        return this->findMin(node->getRc());
    // 接下来找左边的最大
    if (node->getLc() != nullptr)
        return this->findMax(node->getLc());
    // 实在不行自己就是。
    return node;
}

template <typename T>
void RBT<T>::insertFix(RBnode<T> *cur)
{
    RBnode<T> *uncle = nullptr, *fa = nullptr, *gra = nullptr;

    while (cur->getFa() != nullptr &&
           cur->getFa()->getColor() == RED)
    {
        // 需要判断父亲是祖父的左儿子还是右儿子。从而找到叔节点
        fa = cur->getFa(), gra = fa->getFa();
        if (fa == gra->getLc())
        {
            uncle = gra->getRc();
            /**
             * (1)
             *        Bgra               Rgra
             *        / \                /  \
             *     Rfa   Runcle  =>     Bfa Buncle   # 然后把祖父当成儿子来重复到根为止。
             *      |                    |
             *    Rcur                 Rcur
             */
            if (uncle != nullptr && uncle->getColor() == RED)
            {
                uncle->setColor(BLACK);
                fa->setColor(BLACK);
                gra->setColor(RED);
                cur = gra;
            }
            else // 叔叔可能不在或者是正常的
            {
                /**
                 * (2)
                 *      Bgra                    Rgra
                 *      /  \             1      /  \
                 *    Rfa [Buncle, Bnil] =>   Bcur  [Buncle, Bnil]
                 *     \                      /
                 *      Rcur                Rfa
                 *
                 *      Bcur
                 * 2    /   \
                 * => Rfa   Rgra
                 *            \
                 *        [Buncle, Bnil]
                 * (3)
                 * # 插入正确，免除第一步左旋调整。使父亲变祖父即可。
                 */

                if (cur == fa->getRc())
                {
                    cur = fa;
                    this->_Lrotate(&cur);
                }
                fa->setColor(BLACK);
                gra->setColor(RED);
                this->_Rrotate(&gra);
            }
        }
        else // 操作对称。
        {
            uncle = gra->getLc();
            /**
             * (1)
             *        Bgra               Rgra
             *        / \                /   \
             *  Runcle   Rfa  =>     Buncle   Bfa   # 然后把祖父当成儿子来重复到根为止。
             *            |                    |
             *            Rcur                 Rcur
             */
            if (uncle != nullptr && uncle->getColor() == RED)
            {
                uncle->setColor(BLACK);
                fa->setColor(BLACK);
                gra->setColor(RED);
                cur = gra;
            }
            else
            {

                if (cur == fa->getLc())
                {
                    cur = fa;
                    this->_Rrotate(&cur);
                }
                fa->setColor(BLACK);
                gra->setColor(RED);
                this->_Lrotate(&gra); // ???
            }
        }
    }
    this->root->setColor(BLACK);
}

template <typename T>
RBnode<T> *RBT<T>::insert(T val)
{
    RBnode<T> *node = this->root,
              *lst_node = nullptr;
    while (node != nullptr)
    {
        lst_node = node;
        // 已经存在，无需插入。
        if (node->getVal() == val)
            return node;
        else if (node->getVal() > val)
            node = node->getLc();
        else if (node->getVal() < val)
            node = node->getRc();
    }

    node = new RBnode<T>(val);
    node->setFa(lst_node);
    node->setLc(nullptr), node->setRc(nullptr);

    this->size++;

    if (lst_node == nullptr)
    {
        node->setFa(nullptr);
        node->setColor(BLACK);
        this->root = node;
        return node;
    }
    else if (node->getVal() < lst_node->getVal())
        lst_node->setLc(node);
    else
        lst_node->setRc(node);

    // 插入后往往需要修复
    this->insertFix(node);
    return node;
}

// 传进来时，pos 是黑色的。
// 这个函数负责恢复删除后整棵树的平衡。
// 简单来说，平衡之前 pos 一定会有一个兄弟，否则违反了红黑树本身的性质。

template <typename T>
void RBT<T>::removeFix(RBnode<T> *pos, RBnode<T> *fa)
{
    RBnode<T> *bro = nullptr;

    while (pos != this->root && pos->getColor() == BLACK)
    {
        if (pos != nullptr)
            fa = pos->getFa();

        bool sign = fa->getLc() == pos;

        bro = sign ? fa->getRc() : fa->getLc();
        /**
         * 以 Bpos 在左边的情况为例。
         *        fa                   Bbro
         *       /  \                  /  \
         *    Bpos   Rbro    =>       Rfa  Br
         *   <Bnil>                  /  \
         *                        Bpos  [Bnil, Bbro_lc]
         */
        if (bro->getColor() == RED)
        {
            fa->setColor(RED);
            bro->setColor(BLACK);
            sign ? this->_Lrotate(&fa) : this->_Rrotate(&fa);
            bro = sign ? fa->getRc() : fa->getLc();
        }

        // 兄弟的两个(空或非空的)儿子都是黑的。
        if (bro->getLc()->getColor() == bro->getRc()->getColor() &&
            bro->getLc()->getColor() == BLACK)
            bro->setColor(RED), pos = fa;

        else // 至少有一个不是黑的。
        {
            /**
             * 以在左边的情况为例。
             *        Rfa
             *       /   \
             *    Bpos   Bbro
             *           / \
             *     sibling  son
             */
            RBnode<T> *son = sign ? bro->getRc() : bro->getLc(),
                      *sibling = sign ? bro->getLc() : bro->getRc();

            Color now = son->getColor();

            if (now == BLACK)
            {
                if (sibling != nullptr)
                    sibling->setColor(BLACK);

                bro->setColor(RED);
                sign ? this->_Rrotate(&bro) : this->_Lrotate(&bro);
                bro = sign ? fa->getRc() : fa->getLc();
            }

            bro->setColor(fa->getColor());
            fa->setColor(BLACK);

            son = sign ? bro->getRc() : bro->getLc();

            if (son != nullptr)
                son->setColor(BLACK);

            sign ? this->_Lrotate(&fa) : this->_Rrotate(&fa);

            pos = this->root;
        }
    }
    if (pos != nullptr)
        pos->setColor(BLACK);
}
/**
 * 合法性质:
 *  1. 只可能删除没有子树的红叶子。
 *  2. 只可能删除只有一个子树的黑节点。
 *  3. 只可能删除黑色叶子。
 * 针对以上三种性质，应对策略分别是：
 *  1. 直接删除。
 *  2. 孩子继承。
 *  3. 判断兄弟。
 * @attention  删除相当于是插入节点的逆操作。
 * 由于可能提供的值并不能保证合法，所以需要先找到再说，否则免谈。
 * 找到以后的策略就是：替死鬼替死。
 * @param val: 泛型，由用户自定义。
 */
template <typename T>
void RBT<T>::remove(T val)
{
    RBnode<T> *node = this->find(val);
    if (node == nullptr)
        return;

    RBnode<T> *successor = this->next(node),
              *ward = nullptr;
    // 到这里找到了最多只有一个孩子的替死鬼<successor>。

    if (successor->getLc() != nullptr)
        ward = successor->getLc();
    else
        ward = successor->getRc();

    RBnode<T> *fa_nxt = successor->getFa();

    // 调整辈分
    if (ward != nullptr)
        ward->setFa(fa_nxt);

    if (fa_nxt == nullptr)
        this->root = ward;
    else if (fa_nxt->getLc() == successor)
        fa_nxt->setLc(ward);
    else
        fa_nxt->setRc(ward);

    if (successor != node)
        node->setVal(successor->getVal());

    // 原本删红就可以顺利结束，但现在奈何是黑的？
    // 可能会传空指针进去。但是只要做好判断就行。
    if (successor->getColor() == BLACK)
        this->removeFix(ward, fa_nxt);

    delete successor;
    successor = nullptr;
    this->size--;
}