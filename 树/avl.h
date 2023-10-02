#include <algorithm>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <cstring>
#include <queue>

enum direction
{
    REVERSE,
    FORWARD
};
template <class T>
class AVLnode
{
public:
    // 用于计算父节点的平衡因子
    // 当某个节点儿子带来的影响使之大过 1 或者 小过 -1，就一定需要调整
    long long height;
    T val;            // 权值，某些时候需要自定义输入输出方法。否则会使得程序崩溃。
    AVLnode *lc, *rc; // 左右孩
    AVLnode(T val,
            long long h = 0,
            AVLnode *l = nullptr,
            AVLnode *r = nullptr) : height(h),
                                    val(val),
                                    lc(l),
                                    rc(r) {}
};
template <typename T>
inline long long getHeight(AVLnode<T> *node) { return node != nullptr ? node->height : 0; }
template <typename T>
inline long long updateHeight(AVLnode<T> *node)
{
    return std::max(getHeight(node->lc), getHeight(node->rc)) + 1; // 算上自己。
}
template <typename T>
inline bool unbalanceJud(AVLnode<T> *lc, AVLnode<T> *rc, direction sign = FORWARD)
{ // sign = 1: 右边高度比左边高，否则左边比右边高
    // 小心空指针
    long long val = getHeight(rc) - getHeight(lc);
    bool res = sign ? (val == 2ll) : (-val == 2ll);
    return res;
}
// 把整棵树都扬了。
template <typename T>
void destroyAVLtr(AVLnode<T> *root)
{
    if (root->lc != nullptr)
        destroyAVLtr(root->lc);
    if (root->rc != nullptr)
        destroyAVLtr(root->rc);
    if (root != nullptr)
        delete root; // 把分配的内存送回 arena
    root = nullptr;  // 置空
}

template <typename T>
long long preOrder(AVLnode<T> *root)
{
    long long cnt = 0;
    AVLnode<T> *now = root;
    if (now == nullptr)
        return 0;
    std::cout << now->val << ' ';
    cnt += 1;
    cnt += preOrder(now->lc);
    cnt += preOrder(now->rc);
    return cnt;
}

template <typename T>
long long inOrder(AVLnode<T> *root)
{
    long long cnt = 0;
    AVLnode<T> *now = root;
    if (now == nullptr)
        return 0;
    cnt += inOrder(now->lc);
    std::cout << now->val << ' ';
    cnt += 1;
    cnt += inOrder(now->rc);
    return cnt;
}

template <typename T>
long long postOrder(AVLnode<T> *root)
{
    long long cnt = 0;
    AVLnode<T> *now = root;
    if (now == nullptr)
        return 0;
    cnt += postOrder(now->lc);
    cnt += postOrder(now->rc);
    std::cout << now->val << ' ';
    return cnt + 1;
}

template <typename T>
long long layerOrder(AVLnode<T> *root)
{
    long long cnt = 0;
    std::queue<AVLnode<T> *> q;
    if (root == nullptr)
        return cnt;
    q.push(root);
    cnt += 1;
    while (!q.empty())
    {
        AVLnode<T> *u = q.front();
        q.pop();
        if (u->lc != nullptr)
            q.push(u->lc), cnt += 1;
        if (u->rc != nullptr)
            q.push(u->rc), cnt += 1;
        std::cout << u->val << ' ';
    }
    return cnt;
}

/**
 * 函数名即其地址。
 * 传入三种遍历方式中的一个以额外增添回车键。
 */
template <typename T>
void OrderpaddingEnter(long long (*func_ptr)(AVLnode<T> *),
                       AVLnode<T> *root)
{
    (*func_ptr)(root);
    puts("");
}
template <typename T>
AVLnode<T> *FindMax(AVLnode<T> *node)
{
    if (node == nullptr)
        return node;
    while (node->rc != nullptr)
        node = node->rc;
    return node;
}

template <typename T>
AVLnode<T> *FindMin(AVLnode<T> *node)
{
    if (node == nullptr)
        return node;
    while (node->lc != nullptr)
        node = node->lc;
    return node;
}

/**
 * 左旋父亲为自身左儿子。
 *          |
 *     node(height = 2)
 *                \
 *                 rc
 *                /  \
 *        <rrc, nil>  rrc
 */

/**
 * @return  返回的地址来更新 node 父亲的指向。
 */
template <typename T>
AVLnode<T> *LRotation(AVLnode<T> **node)
{
    AVLnode<T> *rc = (*node)->rc;
    (*node)->rc = rc->lc;
    rc->lc = (*node);

    (*node)->height = updateHeight((*node));
    rc->height = updateHeight(rc);

    return rc;
}

/**
 * 右旋父亲为自身右儿子。
 *          |
 *     node(height = 2)
 *         /
 *        lc
 *       /  \
 *   llc(?)  <rlc, nil>
 */
/**
 * @return  返回的地址来更新 node 父亲的指向。
 */
template <typename T>
AVLnode<T> *RRotation(AVLnode<T> **node)
{
    AVLnode<T> *lc = (*node)->lc;
    (*node)->lc = lc->rc,
    lc->rc = *node;
    (*node)->height = updateHeight(*node);
    lc->height = updateHeight(lc);
    return lc;
}

/**
 *          |                 |
 *        node             [node]             |
 *       /                   /               lrc
 *     [lc]      ==>       lrc       ==>    /   \
 *       \                /               lc    node
 *        lrc            lc
 */

/**
 * @return  返回的地址来更新 node 父亲的指向。
 */
template <typename T>
AVLnode<T> *LRdoubleRotate(AVLnode<T> *node)
{
    node->lc = LRotation(&node->lc);
    return RRotation(&node);
}

/**
 *      |                 |
 *     node             [node]               |
 *       \                   \              rlc
 *      [rc]    ==>          rlc    ==>    /   \
 *       /                     \         node   rc
 *      rlc                      rc
 */

/**
 * @return  返回的地址来更新 node 父亲的指向。
 */
template <typename T>
AVLnode<T> *RLdoubleRotate(AVLnode<T> *node)
{
    node->rc = RRotation(&node->rc);
    return LRotation(&node);
}

template <typename T>
AVLnode<T> *AVLinsert(AVLnode<T> *node, T val)
{
    if (node == nullptr)
        node = new AVLnode<T>(val);

    else if (val < node->val)
    {
        // 递归插入过程
        node->lc = AVLinsert(node->lc, val);
        // 出来以后，左边会变高
        if (unbalanceJud(node->lc, node->rc, REVERSE)) // 左倾失衡
        {
            if (node->lc->val > val) // 左型共线
                node = RRotation(&node);

            else if (node->lc->val < val) // < 那就先把自己变孙子，然后把孙子变祖父
                node = LRdoubleRotate(node);
        }
    }
    else if (val > node->val)
    {
        node->rc = AVLinsert(node->rc, val);
        if (unbalanceJud(node->lc, node->rc)) // 右倾失衡
        {
            if (node->rc->val < val) // 右型共线
                node = LRotation(&node);

            else if (node->rc->val > val) // >
                node = RLdoubleRotate(node);
        }
    }

    // 记录自身高度。
    node->height = updateHeight(node);
    return node;
}

/**
 * 删除等价于插入的逆。
 * 删除右子树中的某个节点，“相当于”左子树加了一个节点。
 * 具体如何加，看是否会使得整棵树失衡。
 */
template <typename T>
AVLnode<T> *AVLdelete(AVLnode<T> *node, T val)
{
    if (node == nullptr)
        return node;

    if (node->val == val)
    {
        // 考虑使用右子树最小节点或者左子树最大节点来替代这即将被删除的节点位置。
        // 从而可严格保持中序遍历的正确。
        // 但在此之前，需判断是否为叶子 / 有单个孩子
        if (!(node->lc != nullptr && node->rc != nullptr))
        {
            AVLnode<T> *tmp = node;
            if (node->lc != nullptr)
                node = node->lc;
            else if (node->rc != nullptr)
                node = node->rc;
            delete tmp;
            return tmp = nullptr;
        }

        // 都有孩子的话，看哪个孩子高
        if (getHeight(node->rc) > getHeight(node->lc))
        {
            // 需要取出右树上的最小值作为当前节点的替代。
            AVLnode<T> *rmin = FindMin(node->rc);
            node->val = rmin->val;                     // 覆盖这个值
            node->rc = AVLdelete(node->rc, node->val); // 开始覆盖到叶子然后删除。
        }
        else
        {
            AVLnode<T> *lmax = FindMax(node->lc);      // 取左树最大值。
            node->val = lmax->val;                     // 覆盖
            node->rc = AVLdelete(node->rc, node->val); // 搞到叶子然后删除。
        }
    }
    else if (node->val > val) // 左边找
    {
        node->lc = AVLdelete(node->lc, val);
        if (!unbalanceJud(node->lc, node->rc))
            return node;
        // 删完以后右边高
        // 查看右树的两个孩子哪一个在作妖
        if (getHeight(node->rc->rc) > getHeight(node->rc->lc))
            // 右型共线
            node = LRotation(&node);
        else
            node = RLdoubleRotate(node);
    }
    else // 右边找
    {
        node->rc = AVLdelete(node->rc, val);
        if (!unbalanceJud(node->lc, node->rc, REVERSE))
            return node;
        // 删完以后左边高
        // 查看左树的两个孩子哪一个在作妖
        if (getHeight(node->lc->rc) > getHeight(node->lc->lc))
            // 左型共线
            node = RRotation(&node);
        else
            node = LRdoubleRotate(node);
    }

    return node;
}

/**
 * @return  递归查找。返回空或者具体的节点地址。
 * log(n) 级别的额外栈空间
 */
template <typename T>
AVLnode<T> *AVLsearchRecursively(AVLnode<T> *node, T val)
{
    if (node == nullptr || val == node->val)
        return node;

    else if (val > node->val)
        return AVLsearchRecursively(node->rc, val);
    else
        return AVLsearchRecursively(node->lc, val);
}

/**
 * @return  非递归查找。返回空或者具体的节点地址。
 */
template <typename T>
AVLnode<T> *AVLsearchRepeatedly(AVLnode<T> *node, T val)
{
    AVLnode<T> *tmp = node;
    while (tmp != nullptr)
    {
        if (tmp->val == val)
            break;
        if (tmp->val > val)
            tmp = tmp->lc;
        else if (tmp->val < val)
            tmp = tmp->rc;
    }
    return tmp;
}
//< ============================= 功能函数区域结束 ============================= >//
