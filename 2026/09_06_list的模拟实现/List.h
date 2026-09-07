#pragma once
// 实现list
// 一、创建节点
// 二、构造链表，实现尾插
// 三、实现迭代器
// 四、完善链表功能，实现insert等
// 五、拷贝构造，赋值
// 六、const迭代器。方法一：拷贝一份迭代器修改为const。方法二：加一个模版参数。
// 七、实现反向迭代器。方式一独立类。方式二适配器

#include <iostream>
#include <cassert>
#include <initializer_list>

namespace mylist
{
    // 一、创建节点
    // 1、list 的节点应当包含：指向前一个节点的指针、指向后一个节点的指针、当前节点的数据。
    // 其中数据的类型应当是不确定的，所以可以使用模板参数

    // 使用 struct 不受到访问限定符的限制，如 private，默认直接 public
    // 使用 struct 不是因为功能上做不到，而是因为“它只是个数据容器”，
    // 用 struct 既省下了 public: 的代码量，也明确告诉阅读代码的人：
    // “这个结构体的所有内容都是公开的，放心直接操作”。
    template <class T>
    struct list_node
    {
        list_node *_prev;
        list_node *_next;
        T _data;

        // 接受数据的构造函数，让 new ListNode(data) 能通过
        list_node(const T &data = T())
            : _prev(nullptr),
              _next(nullptr),
              _data(data)
        {
        }
    };

    // 三、实现迭代器
    // template <class T>
    template <class T, class Ref, class Ptr>
    struct list_iterator
    {
        // 节点将会被频繁访问，要重命名一下
        typedef list_node<T> Node;
        typedef list_iterator<T, Ref, Ptr> Self;
        // typedef list_iterator<T> Self;

        Node *_node; // 当前节点

        // 构造
        list_iterator(Node *node = nullptr)
            : _node(node)
        {
        }

        // 添加的模板构造函数，允许从其他Ref和Ptr的迭代器构造
        // 可以不加，但是只能在传参数时使用const_iterator，加上可以直接使用
        template <class RefOther, class PtrOther>
        list_iterator(const list_iterator<T, RefOther, PtrOther> &other)
            : _node(other._node)
        {
        }

        // 重载 *
        // 为什么？？

        // 假设没有重载 * ，那么想获取迭代器指向的数据时，都要这样：
        // list_iterator<int> it = list.begin();
        // int value = it._node->_data;   // 必须暴露迭代器的内部成员

        // 而重载 operator* 后
        // int value = *it;
        // 这层封装把“先解引用取节点，再取节点里的数据”这两个步骤合并成了一个 *it。
        // T &operator*()
        Ref operator*()
        {
            return _node->_data;
        }

        // 结构体访问
        Ptr operator->()
        {
            return &_node->_data;
        }

        // 重载前置 ++
        Self &operator++()
        {
            _node = _node->_next;
            return *this;
        }
        // 重载后置 ++
        // 放 int 是规定，int 后的参数名可以省略
        Self operator++(int)
        {
            Self tmp = *this;
            _node = _node->_next;
            return tmp;
        }
        // 重载前置 --
        Self &operator--()
        {
            _node = _node->_prev;
            return *this;
        }
        // 重载后置 --
        // 放 int 是规定，int 后的参数名可以省略
        Self operator--(int)
        {
            Self tmp = *this;
            _node = _node->_prev;
            return tmp;
        }

        // 比较节点的 != 和 == ，此时不需要修改，使用const
        // 末尾的 const 修饰的是当前对象（即 this 指针），
        // 它表示“这个函数不会修改当前迭代器对象本身”。它的核心意义是：让这个比较函数能够处理 const 迭代器对象。
        bool operator!=(const Self &it) const
        {
            return _node != it._node;
        }

        bool operator==(const Self &it) const
        {
            return _node == it._node;
        }
    };

    // template <class T>
    // struct list_const_iterator
    // {
    //     // 节点将会被频繁访问，要重命名一下
    //     typedef list_node<T> Node;
    //     typedef list_const_iterator<T> Self;

    //     Node *_node; // 当前节点

    //     // 构造
    //     list_const_iterator(Node *node = nullptr)
    //         : _node(node)
    //     {
    //     }

    //     // 转换构造：iterator → const_iterator
    //     // lt.begin()/lt.end() 返回 iterator，需要能隐式转成 const_iterator
    //     list_const_iterator(const list_iterator<T> &it)
    //         : _node(it._node)
    //     {
    //     }

    //     const T &operator*()
    //     {
    //         return _node->_data;
    //     }

    //     Self &operator++()
    //     {
    //         _node = _node->_next;
    //         return *this;
    //     }

    //     Self operator++(int)
    //     {
    //         Self tmp = *this;
    //         _node = _node->_next;
    //         return tmp;
    //     }

    //     Self &operator--()
    //     {
    //         _node = _node->_prev;
    //         return *this;
    //     }

    //     Self operator--(int)
    //     {
    //         Self tmp = *this;
    //         _node = _node->_prev;
    //         return tmp;
    //     }

    //     bool operator!=(const Self &it) const
    //     {
    //         return _node != it._node;
    //     }

    //     bool operator==(const Self &it) const
    //     {
    //         return _node == it._node;
    //     }
    // };

    // 七、实现反向迭代器
    // 方式一：直接复制正向迭代器代码，将 ++ 改为 -- 实现反向迭代器
    // template <class T, class Ref, class Ptr>
    // struct list_reverse_iterator
    // {
    //     typedef list_node<T> Node;
    //     typedef list_reverse_iterator<T, Ref, Ptr> Self;

    //     Node *_node; // 当前节点

    //     list_reverse_iterator(Node *node = nullptr)
    //         : _node(node)
    //     {
    //     }

    // 	template<class RefOther, class PtrOther>
    // 	list_reverse_iterator(const list_reverse_iterator<T, RefOther, PtrOther>& other)
    // 		: _node(other._node)
    // 	{}

    //     Ref operator*()
    //     {
    //         return _node->_data;
    //     }
    //     Ptr operator->()
    //     {
    //         return &_node->_data;
    //     }

    //     Self &operator++()
    //     {
    //         _node = _node->_prev;
    //         return *this;
    //     }
    //     Self operator++(int)
    //     {
    //         Self tmp = *this;
    //         _node = _node->_prev;
    //         return tmp;
    //     }
    //     Self &operator--()
    //     {
    //         _node = _node->_next;
    //         return *this;
    //     }
    //     Self operator--(int)
    //     {
    //         Self tmp = *this;
    //         _node = _node->_next;
    //         return tmp;
    //     }
    //     bool operator!=(const Self &it) const
    //     {
    //         return _node != it._node;
    //     }

    //     bool operator==(const Self &it) const
    //     {
    //         return _node == it._node;
    //     }
    // };

    // 方式二：使用模板参数，实现反向迭代器 --适配器
    template <class Iter, class Ref, class Ptr>
    struct list_reverse_iterator
    {
        Iter _iterator;
        typedef list_reverse_iterator<Iter, Ref, Ptr> Self;

        list_reverse_iterator(Iter it = Iter())
            : _iterator(it)
        {
        }

        // 转换构造：普通反向适配器 → const 反向适配器
        template <class IterOther, class RefOther, class PtrOther>
        list_reverse_iterator(const list_reverse_iterator<IterOther, RefOther, PtrOther> &other)
            : _iterator(other._iterator)
        {
        }

        Ref operator*()
        {
            Iter tmp = _iterator;     // 错位核心：先拷贝
            --tmp;                    // 后退一步
            return *tmp;              // 再解引用 → 前一个元素
        }
        Ptr operator->()
        {
            return &(operator*()); // 复用 *
        }
        // 方向全部反转转发
        Self &operator++()
        {
            --_iterator; // ++ → base 的 --
            return *this;
        }
        Self operator++(int)
        {
            Self tmp = *this;
            --_iterator; // ++ → base 的 --
            return tmp;
        }
        Self &operator--()
        {
            ++_iterator;     // -- → base 的 ++
            return *this;
        }
        Self operator--(int)
        {
            Self tmp = *this;
            ++_iterator;     // -- → base 的 ++ 
            return tmp;
        }
        bool operator!=(const Self &it) const
        {
            return _iterator != it._iterator;
        }

        bool operator==(const Self &it) const
        {
            return _iterator == it._iterator;
        }
    };

    // 二、构造链表，实现尾插
    template <class T>
    class list
    {
    private:
        typedef list_node<T> ListNode;

    private:
        ListNode *_head; // 只需头节点即可
        size_t _size;    // list 长度
    public:
        // 迭代器
        typedef list_iterator<T, T &, T *> iterator;
        typedef list_iterator<T, const T &, const T *> const_iterator;
        // typedef list_iterator<T> iterator;
        // typedef list_const_iterator<T> const_iterator;

        iterator begin()
        {
            // _head->_next 是第一个节点，_head 是哨兵位
            return iterator(_head->_next);
        }
        iterator end()
        {
            return iterator(_head);
        }
        const_iterator begin() const
        {
            return const_iterator(_head->_next);
        }
        const_iterator end() const
        {
            return const_iterator(_head);
        }

        // 反向迭代器
        // 方式一：独立类，实现反向迭代器
        // typedef list_reverse_iterator<T, T &, T *> reverse_iterator;
        // typedef list_reverse_iterator<T, const T &, const T *> const_reverse_iterator;

        // reverse_iterator rbegin()
        // {
        //     return reverse_iterator(_head->_prev);
        // }
        // reverse_iterator rend()
        // {
        //     return reverse_iterator(_head);
        // }
        // const_reverse_iterator rbegin() const
        // {
        //     return const_reverse_iterator(_head->_prev);
        // }
        // const_reverse_iterator rend() const
        // {
        //     return const_reverse_iterator(_head);
        // }

        // 方式二：使用模板参数，实现反向迭代器 --适配器
        typedef list_reverse_iterator<iterator, T &, T *> reverse_iterator;
        typedef list_reverse_iterator<const_iterator, const T &, const T *> const_reverse_iterator;

        reverse_iterator rbegin()
        {
            return reverse_iterator(end());
        }
        reverse_iterator rend()
        {
            return reverse_iterator(begin());
        }
        const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator(end());
        }
        const_reverse_iterator rend() const
        {
            return const_reverse_iterator(begin());
        }

    public:
        // 空初始化
        void empty_init()
        {
            _head = new ListNode;
            // 头尾相衔
            _head->_prev = _head;
            _head->_next = _head;
            _size = 0;
        }

        // 构造
        list()
        {
            empty_init();
        }
        // 拷贝构造
        // lt2(lt1)，const 对象转为范围 for 使用 const 迭代器，不写 const 迭代器会报错
        // list(const list<T>& lt)
        list(list<T> &lt)
        {
            empty_init();
            for (auto &e : lt)
            {
                push_back(e);
            }
        }

        void swap(list<T> &lt)
        {
            std::swap(_head, lt._head);
            std::swap(_size, lt._size);
        }
        // 重载赋值 =
        // lt1 = lt3
        list<T> &operator=(list<T> lt) // 移动赋值
        {
            swap(lt);
            return *this;
        }

        // 多个数据,但需要先重载 operator=
        // 语法一：初始化（构造）—— 只需 initializer_list 构造函数，不需要 operator=
        // list<int> lt1 = {1, 2, 3};   // 拷贝列表初始化，直接调用构造函数
        // list<int> lt2{1, 2, 3};       // 直接列表初始化，也是构造函数
        // 语法二：赋值（先构造再赋值）—— 需要 initializer_list 构造函数 + operator=
        // list<int> lt3;
        // lt3 = {1, 2, 3};              // 先用构造函数把 {1,2,3} 变成临时对象，再调 operator=
        list(std::initializer_list<T> il)
        {
            empty_init();
            for (auto &e : il)
            {
                push_back(e);
            }
        }

        // 尾插：需要给定 T 类型的一个数据，只进行插入，不会修改，所以用const
        // 使用引用 & 可以避免不必要的拷贝，尤其对于 std::string std::vector 等自定义大对象，
        // 节省了一次深拷贝（内存分配和复制），性能提升显著。
        void push_back(const T &data)
        {
            // 直接复用 insert 即可
            insert(end(), data);

            // // 1、尾节点
            // ListNode *tail = _head->_prev;
            // // 2、创建新节点并插入
            // ListNode *node = new ListNode;
            // node->_data = data;
            // node->_prev = tail;
            // node->_next = _head;

            // tail->_next = node;
            // _head->_prev = node;

            // ++_size;
        }

        void push_front(const T &data)
        {
            insert(begin(), data);
        }

        void pop_back()
        {
            if (_size == 0)
            {
                return;
            }

            // 注意 end 是 _head 节点，所以要 --
            erase(--end());

            // ListNode* tmp = _head->_prev;
            // tmp->_prev->_next = _head;
            // _head->_prev = tmp->_prev;
            // delete tmp;
            // --_size;
        }

        void pop_front()
        {
            if (_size == 0)
            {
                return;
            }

            // 此处的 begin 就是 _head->_next，符合期望
            erase(begin());
        }

        // 指定位置插入
        void insert(iterator pos, const T &data)
        {
            ListNode *cur = pos._node;
            ListNode *prev = cur->_prev;
            ListNode *next = cur->_next;

            ListNode *newNode = new ListNode(data);
            newNode->_prev = prev;
            newNode->_next = cur;

            prev->_next = newNode;
            cur->_prev = newNode;

            ++_size;
        }

        // 防止迭代器失效，返回迭代器
        iterator erase(iterator pos)
        {
            assert(pos != end());

            ListNode *cur = pos._node;
            ListNode *next = cur->_next;
            ListNode *prev = cur->_prev;

            prev->_next = next;
            next->_prev = prev;
            delete cur;

            --_size;
            // next 前移到 pos 位置
            // return next; // 依赖隐式转换，虽然此处也能用，但是后期如果加 explicit 后会挂
            return iterator(next);
        }

        size_t size()
        {
            return _size;
        }

        size_t size() const
        {
            return _size;
        }

        // 不清除哨兵位
        void clear()
        {
            auto it = begin();
            while (it != end())
            {
                it = erase(it);
            }
        }

        ~list()
        {
            // // 删除节点
            // while (_head->_next != _head)
            // {
            //     ListNode *tmp = _head->_next;
            //     _head->_next = tmp->_next;
            //     tmp->_next->_prev = _head;
            //     delete tmp;
            //     --_size;
            // }

            clear();
            delete _head;
            _head = nullptr;
        }
    };

}
