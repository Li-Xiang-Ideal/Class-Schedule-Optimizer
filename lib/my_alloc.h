/**********************************************************
 * 
 *                        My Lib 
 *                      my_alloc.h
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
 * 本文件包含一个空间配置器, 对_has_trivial_destructor进行了优
 * 化. 二级空间配置器待添加. 
 * 本文件参考了侯捷《STL源码剖析》, 特此说明并致谢
 ***********************************************************/

/************************************************************
 * 更新日志:
 * 2020.04.14 对_has_trivial_destructor进行了优化
 * 2020.05.02 对部分代码进行了调整
 ***********************************************************/

#ifndef MY_ALLOC
#define MY_ALLOC

#include <new>
/*
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <climits>
#include <iostream>
#include <memory> //*/
#include <bits/c++config.h>
#include <bits/functexcept.h>
#include <bits/move.h>
#if __cplusplus >= 201103L
//#include <type_traits>
#endif

#include "my_type_traits.h"
#include "my_iterator.h"

namespace my_lib
{
    // allocate的实现, 简单封装new
    template <typename _Tp>
    inline _Tp* _allocate(ptrdiff_t size, _Tp*) {
        std::set_new_handler(0);
        _Tp* tmp = (_Tp*)(::operator new((size_t)(size*sizeof(_Tp))));
        if (tmp == 0) {
            std::cerr << "out of memory" << std::endl;
            exit(1);
        }
        return tmp;
    }

    // deallocate的实现, 简单封装delete
    template <typename _Tp>
    inline void _deallocate(_Tp* __p) { ::operator delete(__p); }

    // address的实现, 简单封装&
    template<typename _Tp>
    inline _Tp* _addressof(_Tp& __r) _GLIBCXX_NOEXCEPT {
        return reinterpret_cast<_Tp*>
        (&const_cast<char&>(reinterpret_cast<const volatile char&>(__r)));
    }
    
#if __cplusplus >= 201103L
    // construct的实现, 直接调用对象的无参构造函数
    template<typename _Tp1>
    inline void _construct(_Tp1* __p)
	{ ::new((void *)__p) _Tp1(); }

    // construct的实现, 直接调用对象的有参构造函数
    template<typename _Tp1, typename... _Vals>
    inline void _construct(_Tp1* __p, _Vals&&... __vals)
	{ ::new((void *)__p) _Tp1(std::forward<_Vals>(__vals)...); }

    // destroy的简单实现, 直接调用对象的析构函数
    template <typename _Tp>
    inline void _destroy(_Tp* ptr) { ptr->~_Tp(); }


    //*destroy的实现之一, 使用类模板参数类型推断

    // destroy的实现, 对象有非平凡析构函数之偏特化
    template<bool>
    struct _destroy_aux
    {
        template<typename _ForwardIterator>
        static void __destroy(_ForwardIterator __first, _ForwardIterator __last) 
        { for (; __first != __last; ++__first) { _destroy(_addressof(*__first)); } }
    };

    // destroy的实现, 对象没有非平凡析构函数之偏特化
    template<>
    struct _destroy_aux<true>
    {
        template<typename _ForwardIterator>
        static void __destroy(_ForwardIterator, _ForwardIterator) { }
    }; //注意这里用bool完成推断, 而非直接使用__true_type, 原因将在下文介绍*/


    /*destroy的实现之二, 使用函数参数类型推断

    // destroy的实现, 对象有非平凡析构函数之偏特化
    template<typename _ForwardIterator>
    void __destroy(_ForwardIterator __first, _ForwardIterator __last, __false_type) 
    { for (; __first != __last; ++__first) { _destroy(_addressof(*__first)); } }

    // destroy的实现, 对象没有非平凡析构函数之偏特化
    template<typename _ForwardIterator>
    void __destroy(_ForwardIterator __first, _ForwardIterator __last, __true_type) { } //*/
    

    // destroy的实现, 识别对象是否有非平凡析构函数
    template<typename _ForwardIterator>
    inline void _destroy(_ForwardIterator _first, _ForwardIterator _last)
    {
        typedef typename iterator_traits<_ForwardIterator>::value_type _Value_type;
        //以下实现使用了类模板参数类型推断
        _destroy_aux<has_trivial_destructor<_Value_type>::value>::__destroy(_first, _last);
        /*不能使用以下方法: 在_destroy_aux处使用__true_type推断, 而这里使用
        _destroy_aux<has_trivial_destructor<_Value_type>>::__destroy(_first, _last);
        原因在于编译器会在_destroy_aux<...>处直接推断为non-trivial, 
        而不会先推断has_trivial_destructor<_Value_type>再推断_destroy_aux<...>*/

        /*也不能使用以下方法: 用___has_trivial_destructor(_Value_type()).value推断, 
        因为这是试图用函数返回值推断, 是无法进行的*/

        /*另一种实现, 使用inline函数进行函数参数类型推断
        __destroy(_first, _last, _has_trivial_destructor(_Value_type())); /* 括号用于生成临时对象 */
    }

#else
    // construct的实现, 直接调用对象的无参构造函数
    template<typename _Tp1>
    inline void _construct(_Tp1* __p)
	{ ::new((void *)__p) _Tp1(); }

    // construct的实现, 直接调用对象的有参构造函数
    template<typename _Tp1, typename... _Vals>
    inline void _construct(_Tp1* __p, _Vals&&... __vals)
	{ ::new((void *)__p) _Tp1(std::forward<_Vals>(__vals)...); }

    // destroy的实现, 直接调用对象的析构函数
    template <typename _Tp>
    inline void _destroy(_Tp* ptr) { ptr->~_Tp(); }
#endif

    

    template <typename _Tp>
    class allocator {
    public:
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;
        typedef _Tp*        pointer;
        typedef const _Tp*  const_pointer;
        typedef _Tp&        reference;
        typedef const _Tp&  const_reference;
        typedef _Tp         value_type;

        // 构造函数
        allocator() { }
        allocator(const allocator&) { }
        template <typename _Tp1>
        allocator(const allocator<_Tp1>&) { }

        // rebind allocator of type _Tp1
        template<typename _Tp1>
        struct rebind
        { typedef allocator<_Tp1> other; };

#if __cplusplus >= 201103L
        // _GLIBCXX_RESOLVE_LIB_DEFECTS
        // 2103. propagate_on_container_move_assignment
        typedef std::true_type propagate_on_container_move_assignment;
#endif

        // allocate, deallocate, construct和destroy函数均调用上面的实际实现
        // hint used for locality. ref.[Austern],p189
        pointer allocate(size_type _n, const void* hint = 0) {
            return _allocate((difference_type)_n, (pointer)0);
        }
        void deallocate(pointer _p, size_type _n) { _deallocate(_p); }

#if __cplusplus >= 201103L
        void construct(pointer _p) { _construct(_p); }

        template<typename... _Args>
        void construct(pointer _p, const _Args&&... __args) { _construct(_p, __args...); }

        void destroy(pointer _p) { _destroy(_p); }

        template<typename _ForwardIterator>
        void destroy(_ForwardIterator _first, _ForwardIterator _last) 
        { _destroy(_first, _last); }
#else
        void construct(pointer _p) { _construct(_p); }
        void construct(pointer _p, const _Tp& _Arg) { _construct(_p, _Arg); }
        void destroy(pointer _p) { _destroy(_p); }
#endif

        pointer address(reference _x) const { return (pointer)&_x; }
        const_pointer const_address(const_reference _x) { return (const_pointer)&_x; }

        size_type max_size() const { return size_type(UINT_MAX / sizeof(_Tp)); }   
    };
} // namespace my_alloc

#endif // MY_ALLOC
