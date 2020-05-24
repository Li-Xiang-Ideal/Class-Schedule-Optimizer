/**********************************************************
 * 
 *                        My Lib 
 *                     my_iterator.h
 * 
 *               Copyright (C) 2020 李想
 * Released under the GNU General Public License Version 3
 * Some codes was originally from Standard Template Library
 * 
 *********************************************************/

/************************************************************
 * My Lib 的目的在于构建一系列完整包含STL六大组件(allocators, 
 * containers, iterators, adapters, functors, algorithms)的
 * 库文件, 追求在某些场景下实现比STL更高的效率, 同时提高可读性.
 ***********************************************************/

/************************************************************
 * 说明: 
 * 本文件包含一个供继承的迭代器模板, 并提供了若干常用的iterator
 * 相关示性及功能函数. 本文件使用了traits技巧并进行了相对详尽的
 * 注释以备温习或查阅. 部分代码相比STL进行了少量调整. 
 * 本文件参考了侯捷《STL源码剖析》, 特此说明并致谢
 ***********************************************************/

/************************************************************
 * 更新日志:
 * 2020.05.02 添加了部分功能函数
 * 2020.05.24 添加了一个更具实用价值的iterator模板
 ***********************************************************/

#ifndef MY_ITERATOR
#define MY_ITERATOR

#define __MY_LIB_TEMPLATE_NULL template<>

#include <cstdlib>
#include <bits/c++config.h>
#include <bits/functexcept.h>

namespace my_lib
{
    //5种基本的iterator类型
    struct input_iterator_tag { };
    struct output_iterator_tag { };
    struct forward_iterator_tag : public input_iterator_tag, public output_iterator_tag { };
    struct bidirectional_iterator_tag : public forward_iterator_tag { };
    struct random_access_iterator_tag : public bidirectional_iterator_tag { };

    //最基本的iterator模板, 自定义iterator时继承此iterator以获得typedef
    template<typename Category, typename _Tp, typename Distance = ptrdiff_t, 
                typename Pointer = _Tp*, typename Reference = _Tp&>
    struct iterator {
	    typedef Category    iterator_category;
	    typedef _Tp         value_type;
	    typedef Distance    difference_type;
	    typedef Pointer     pointer;
	    typedef Reference   reference;
    };

    //主模板
    template<typename _Iterator>
    struct __iterator_traits
    {
        typedef typename _Iterator::iterator_category   iterator_category;
        typedef typename _Iterator::value_type          value_type;
        typedef typename _Iterator::difference_type     difference_type;
        typedef typename _Iterator::pointer             pointer;
        typedef typename _Iterator::reference           reference;
    };

    //对原生指针偏特化
    template<typename _Tp>
    struct __iterator_traits<_Tp*>
    {
        //tips:通常来说, 没有作用域符号::的地方不需要typedef typename, 直接typedef即可
        typedef random_access_iterator_tag  iterator_category;
        typedef _Tp                         value_type;
        typedef ptrdiff_t                   difference_type;
        typedef _Tp*                        pointer;
        typedef _Tp&                        reference;
    };

    //对const原生指针偏特化
    template<typename _Tp>
    struct __iterator_traits<const _Tp*>
    {
        typedef random_access_iterator_tag  iterator_category;
        typedef _Tp                         value_type;
        typedef ptrdiff_t                   difference_type;
        typedef _Tp*                        pointer;
        typedef _Tp&                        reference;
    };

    //对volatile原生指针偏特化
    template<typename _Tp>
    struct __iterator_traits<volatile _Tp*>
    {
        typedef random_access_iterator_tag  iterator_category;
        typedef _Tp                         value_type;
        typedef ptrdiff_t                   difference_type;
        typedef _Tp*                        pointer;
        typedef _Tp&                        reference;
    };

    //通过继承对_Iterator进行类型推断, 获得iterator_traits
    template<typename _Iterator>
    struct iterator_traits : public __iterator_traits<_Iterator> { };
    //我也不知道为什么STL这里要选择先定义一个__class再令class继承之, 看起来没必要. 不过模仿STL应该不会有错

    //inline函数获得iterator_category
    template<typename _Iterator>
    inline typename iterator_traits<_Iterator>::iterator_category
    Iterator_category(const _Iterator&) //为了避免与其他文件中定义的typename重名, 这里函数名一律首字母大写
    {
        //tips:如果发现class<type>::object中的object没有被自动语法高亮, 则需要在typedef后加上typename
        typedef typename iterator_traits<_Iterator>::iterator_category iterator_category;
        return iterator_category(); //括号用于生成临时对象
    }

    //获得value_type
    template<typename _Iterator>
    inline typename iterator_traits<_Iterator>::value_type
    Value_type(const _Iterator&)
    {
        typedef typename iterator_traits<_Iterator>::value_type value_type;
        return value_type();
    }

    //获得value_type的指针value_type_pointer
    template<typename _Iterator>
    inline typename iterator_traits<_Iterator>::pointer
    Value_type_pointer(const _Iterator&)
    {
        typedef typename iterator_traits<_Iterator>::pointer pointer;
        return (pointer)(0);
    }
    
    //difference_type
    template<typename _Iterator>
    inline typename iterator_traits<_Iterator>::difference_type
    Difference_type(const _Iterator&)
    {
        typedef typename iterator_traits<_Iterator>::difference_type difference_type;
        return difference_type();
    }

    //difference_type_pointer
    template<typename _Iterator>
    inline typename iterator_traits<_Iterator>::difference_type*
    Difference_type_pointer(const _Iterator&)
    {
        typedef typename iterator_traits<_Iterator>::difference_type difference_type;
        return (difference_type*)(0);
    }

    //distance between first and last, forward_iterator偏特化
    template<typename _Forward_Iterator>
    inline typename iterator_traits<_Forward_Iterator>::difference_type
    __distance(_Forward_Iterator first, _Forward_Iterator last, forward_iterator_tag) 
    {
	    typename iterator_traits<_Forward_Iterator>::difference_type _n = 0;
	    for (; first != last; ++first) { ++_n; }
	    return _n;
    }

    //distance between first and last, random_access_iterator偏特化
    template<typename _Forward_Iterator>
    inline typename iterator_traits<_Forward_Iterator>::difference_type
    __distance(_Forward_Iterator first, _Forward_Iterator last, random_access_iterator_tag) 
    {
	    return last - first;
    }

    //distance between first and last的实现, 识别iterator_category
    template<typename _Iterator>
    inline typename iterator_traits<_Iterator>::difference_type
    distance(_Iterator first, _Iterator last) 
    {
        typedef typename iterator_traits<_Iterator>::iterator_category iterator_category;
	    return __distance(first, last, iterator_category());
    }

    //advance iter for n times, forward_iterator偏特化
    template<typename _Forward_Iterator, typename _Distance>
    inline void __advance(_Forward_Iterator& iter, _Distance _n, forward_iterator_tag) 
    {
	    while (_n--) { ++iter; }
    }

    //advance iter for n times, bidirectional_iterator偏特化
    template<typename _Bidirectional_Iterator, typename _Distance>
    inline void __advance(_Bidirectional_Iterator& iter, _Distance _n, bidirectional_iterator_tag) 
    {
        if (_n >= 0)
        {   while (_n--) { ++iter; } }
        else
        {   while (_n++) { --iter; } }
    }

    //advance iter for n times, random_access_iterator偏特化
    template<typename _Random_Access_Iterator, typename _Distance>
    inline void __advance(_Random_Access_Iterator& iter, _Distance _n, random_access_iterator_tag) 
    {
	    iter += _n;
    }

    //advance iter for n times的实现, 识别iterator_category
    template<typename _Iterator, typename _Distance>
    inline void advance(_Iterator iter, _Distance _n) 
    {
	    __advance(iter, _n, Iterator_category(iter));
    }


    //有实用价值的iterator模板, 定义了构造函数,解引用*,作用域->,以及一系列比较运算符
    template<typename Category, typename _Tp, typename Distance = ptrdiff_t, 
                typename Pointer = _Tp*, typename Reference = _Tp&>
    struct iterator_base : public iterator<Category, _Tp, Distance, Pointer, Reference>
    {
        typedef iterator<Category, _Tp, Distance, Pointer, Reference>   _Iterator;
        typedef typename _Iterator::iterator_category                   iterator_category;
        typedef typename _Iterator::value_type                          value_type;
        typedef typename _Iterator::difference_type                     difference_type;
        typedef typename _Iterator::pointer                             pointer;
        typedef typename _Iterator::reference                           reference;
        typedef iterator_base                                           self_type;
    protected:
        pointer _pointer;
    public:
        iterator_base() { };
        iterator_base(const pointer& _ptr)     { _pointer = _ptr; }
        iterator_base(const self_type& _iter)  { _pointer = &(*_iter); }
        ~iterator_base() { };
        // Iterator requirements
        reference
        operator*()  const  { return *_pointer; }
        pointer
        operator->() const  { return _pointer; }
    };
    
    template<typename _Tp, typename _Tag1, typename _Tag2>
    inline bool
    operator==(const iterator_base<_Tag1, _Tp>& __lhs, const iterator_base<_Tag2, _Tp>& __rhs)
    { return &(*__lhs) == &(*__rhs); }

    template<typename _Tp, typename _Tag1>
    inline bool
    operator==(const iterator_base<_Tag1, _Tp>& __lhs, const _Tp* __rhs)
    { return &(*__lhs) == &(*__rhs); }

    template<typename _Tp, typename _Tag2>
    inline bool
    operator==(const _Tp* __lhs, const iterator_base<_Tag2, _Tp>& __rhs)
    { return &(*__lhs) == &(*__rhs); }
    
    template<typename _Tp, typename _Tag1, typename _Tag2>
    inline bool
    operator!=(const iterator_base<_Tag1, _Tp>& __lhs, const iterator_base<_Tag2, _Tp>& __rhs)
    { return &(*__lhs) != &(*__rhs); }

    template<typename _Tp, typename _Tag1>
    inline bool
    operator!=(const iterator_base<_Tag1, _Tp>& __lhs, const _Tp* __rhs)
    { return &(*__lhs) != &(*__rhs); }

    template<typename _Tp, typename _Tag2>
    inline bool
    operator!=(const _Tp* __lhs, const iterator_base<_Tag2, _Tp>& __rhs)
    { return &(*__lhs) != &(*__rhs); }
    
    template<typename _Tp, typename _Tag1, typename _Tag2>
    inline bool
    operator<(const iterator_base<_Tag1, _Tp>& __lhs, const iterator_base<_Tag2, _Tp>& __rhs)
    { return &(*__lhs) < &(*__rhs); }

    template<typename _Tp, typename _Tag1>
    inline bool
    operator<(const iterator_base<_Tag1, _Tp>& __lhs, const _Tp* __rhs)
    { return &(*__lhs) < &(*__rhs); }

    template<typename _Tp, typename _Tag2>
    inline bool
    operator<(const _Tp* __lhs, const iterator_base<_Tag2, _Tp>& __rhs)
    { return &(*__lhs) < &(*__rhs); }
    
    template<typename _Tp, typename _Tag1, typename _Tag2>
    inline bool
    operator>(const iterator_base<_Tag1, _Tp>& __lhs, const iterator_base<_Tag2, _Tp>& __rhs)
    { return &(*__lhs) > &(*__rhs); }

    template<typename _Tp, typename _Tag1>
    inline bool
    operator>(const iterator_base<_Tag1, _Tp>& __lhs, const _Tp* __rhs)
    { return &(*__lhs) > &(*__rhs); }

    template<typename _Tp, typename _Tag2>
    inline bool
    operator>(const _Tp* __lhs, const iterator_base<_Tag2, _Tp>& __rhs)
    { return &(*__lhs) > &(*__rhs); }
    
    template<typename _Tp, typename _Tag1, typename _Tag2>
    inline bool
    operator<=(const iterator_base<_Tag1, _Tp>& __lhs, const iterator_base<_Tag2, _Tp>& __rhs)
    { return &(*__lhs) <= &(*__rhs); }

    template<typename _Tp, typename _Tag1>
    inline bool
    operator<=(const iterator_base<_Tag1, _Tp>& __lhs, const _Tp* __rhs)
    { return &(*__lhs) <= &(*__rhs); }

    template<typename _Tp, typename _Tag2>
    inline bool
    operator<=(const _Tp* __lhs, const iterator_base<_Tag2, _Tp>& __rhs)
    { return &(*__lhs) <= &(*__rhs); }
    
    template<typename _Tp, typename _Tag1, typename _Tag2>
    inline bool
    operator>=(const iterator_base<_Tag1, _Tp>& __lhs, const iterator_base<_Tag2, _Tp>& __rhs)
    { return &(*__lhs) >= &(*__rhs); }

    template<typename _Tp, typename _Tag1>
    inline bool
    operator>=(const iterator_base<_Tag1, _Tp>& __lhs, const _Tp* __rhs)
    { return &(*__lhs) >= &(*__rhs); }

    template<typename _Tp, typename _Tag2>
    inline bool
    operator>=(const _Tp* __lhs, const iterator_base<_Tag2, _Tp>& __rhs)
    { return &(*__lhs) >= &(*__rhs); }


    //以下提供一个random_access_iterator模板供使用或继承或抄
    template<typename _Tp>
    class random_access_iterator : public iterator_base<random_access_iterator_tag, _Tp>
    {
        typedef iterator<random_access_iterator_tag, _Tp>   _Iterator;
        typedef typename _Iterator::iterator_category       iterator_category;
        typedef typename _Iterator::value_type              value_type;
        typedef typename _Iterator::difference_type         difference_type;
        typedef typename _Iterator::pointer                 pointer;
        typedef typename _Iterator::reference               reference;
        typedef random_access_iterator                      self_type;
    protected:
        pointer _pointer;
    public:
        random_access_iterator() { }
        random_access_iterator(const pointer& _ptr)     { _pointer = _ptr; }
        random_access_iterator(const self_type& _iter)  { _pointer = &(*_iter); }
        ~random_access_iterator() { }
        const pointer base() const { return _pointer; }
        // Iterator requirements
        reference
        operator*()  const  { return *_pointer; }
        pointer
        operator->() const  { return _pointer; }
        // Forward Iterator requirements
        self_type&
        operator++() { ++_pointer; return *this; }
        self_type&
        operator++(int) { return self_type(_pointer++); }
        // Bidirectional Iterator requirements
        self_type&
        operator--() { --_pointer; return *this; }
        self_type&
        operator--(int) { return self_type(_pointer--); }
        // Random Access Iterator requirements
        reference
        operator[](difference_type __n) { return _pointer[__n]; }
        self_type&
        operator+=(difference_type __n) { _pointer += __n; return *this; }
        self_type
        operator+(difference_type __n) { return self_type(_pointer + __n); }
        self_type&
        operator-=(difference_type __n) { _pointer -= __n; return *this; }
        self_type
        operator-(difference_type __n) { return self_type(_pointer - __n); }
    };


} // namespace my_lib


#endif
