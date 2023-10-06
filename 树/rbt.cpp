#include "./rbt.h"

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
         * 以 Bpos 在左边的情况为例。如果兄弟是红的，那么父亲一定是黑的。
         *        Bfa                   Bbro
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
             *        Bfa
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
