#include "./rbt.h"

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
 * 而继任者要么是右边的最小值，要么是上一个『中』或者下一个『中』。
 */
template <typename T>
RBnode<T> *RBT<T>::next(RBnode<T> *node)
{
    if (node->getRc() != nullptr)
        return this->findMin(node->getRc());
    RBnode<T> fa = node->getFa();
    while (fa != nullptr && fa->getRc() == node)
        node = fa, fa = node->getFa();
    return fa;
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
             *     Rfa   Runcle  =>     Bfa Buncle, 然后把祖父当成儿子来重复到根为止。
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
                 * 2    Bcur
                 * =>   /   \
                 *     Rfa  Rgra
                 *           \
                 *         [Buncle, Bnil]
                 * (3)
                 * # 插入正确，免除第一步左旋调整。使父亲变祖父即可。
                 */
                if (cur == fa->getRc())
                {
                    cur = fa;
                    this->_Lrotate(cur);
                }
                cur->setColor(BLACK);
                gra->setColor(RED);
                this->_Rrotate(gra);
            }
        }
        else // 操作对称。
        {
            uncle = gra->getLc();
            /**
             * (1)
             *        Bgra               Rgra
             *        / \                /   \
             *  Runcle   Rfa  =>     Buncle   Bfa, 然后把祖父当成儿子来重复到根为止。
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
                    this->_Rrotate(cur);
                }
                cur->setColor(BLACK);
                gra->setColor(RED);
                this->_Lrotate(gra);
            }
        }
    }
    this->root->setColor(BLACK);
}

template <typename T>
RBnode<T> *RBT<T>::insert(T val)
{
    RBnode<T> *node = this->root, *lst_node = nullptr;

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

    RBnode<T> *cur = new RBnode<T>(T);
    cur->setFa(lst_node);
    cur->setLc(nullptr);
    cur->setRc(nullptr);

    this->size++;

    if (lst_node == nullptr)
    {
        cur->setFa(nullptr);
        cur->setColor(BLACK);
        this->root = cur;
        return cur;
    }
    else if (cur->getVal < lst_node->getVal)
        lst_node->setLc(cur);
    else
        lst_node->setRc(cur);

    // 插入后往往需要修复
    this->insertFix(cur);
}

template <typename T>
void RBT<T>::removeFix(RBnode<T> *pos, RBnode<T> *fa)
{
    RBnode<T> *bro = nullptr;
    // 总感觉照着写不如弄清楚原理然后重构（？
    while (pos != this->root && pos->getColor() == BLACK)
    {
        if (pos != nullptr)
            fa = pos->getFa();

        bool sign = fa->getLc() == pos;

        bro = sign ? fa->getRc() : fa->getLc();
        if (bro->getColor() == RED)
        {
            fa->setColor(RED);
            bro->setColor(BLACK);
            sign ? this->_Lrotate(fa) : this->_Rrotate(fa);
            bro = sign ? fa->getRc() : fa->getLc();
        }

        if (bro->getLc()->getColor() == bro->getRc()->getColor() &&
            bro->getLc()->getColor() == BLACK)
            bro->setColor(RED), pos = fa;

        else
        {
            RBnode<T> *child_ptr = sign ? bro->getRc() : bro->getLc();
            Color now = sign
                            ? child_ptr->getColor()
                            : child_ptr->getColor();

            if (now == BLACK)
            {
                if (child_ptr != nullptr)
                    child_ptr->setColor(BLACK);

                bro->setColor(RED);
                sign ? this->_Rrotate(bro) : this->_Lrotate(bro);
                bro = sign ? fa->getRc : fa->getLc;
            }
            bro->setColor(fa->getColor());
            fa->setColor(BLACK);
            child_ptr = sign ? bro->getLc() : bro->getRc();
            if (child_ptr != nullptr)
                child_ptr->setColor(BLACK);
            sign ? this->_Lrotate(fa) : this->_Rrotate(fa);

            pos = this->root;
        }
    }

    if (pos != nullptr)
        pos->setColor(BLACK);
}
/**
 * 删除相当于是插入节点的逆操作。
 * 由于可能提供的值并不能保证合法，所以需要先找到再说，否则免谈。
 * @param val: 泛型，由用户自定义。
 *
 */
template <typename T>
void RBT<T>::remove(T val)
{
    RBnode<T> *node = this->find(val);
    if (node == nullptr)
        return;

    RBnode<T> *successor, *nchild;
    // 到这里找到了替死鬼<successor>。
    if (node->getLc() != nullptr && node->getRc() != nullptr)
        successor = this->next(node);
    else
        successor = node;

    if (successor->getLc() != nullptr)
        nchild = successor->getLc();
    else
        nchild = successor->getRc();

    RBnode<T> *fa_nxt = successor->getFa();

    if (nchild != nullptr)
        nchild->setFa(fa_nxt);

    if (fa_nxt == nullptr)
        this->root = nchild;
    else if (fa_nxt->getLc() == successor)
        fa_nxt->setLc(nchild);
    else
        fa_nxt->setRc(nchild);

    if (successor != node)
        node->setVal(successor->getVal());

    // 双红的情况。
    if (successor->getColor() == BLACK)
        this->removeFix(nchild, fa_nxt);

    delete successor;
    successor = nullptr;
    this->size--;
}