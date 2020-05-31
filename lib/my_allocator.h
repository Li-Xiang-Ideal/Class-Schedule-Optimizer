/**********************************************************
 * 
 *                        My Lib 
 *                    my_allocator.h
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
 * 本文件包含一个空间配置器, 针对_has_trivial_destructor进行了
 * 优化, 并进行了相对详尽的注释以备温习或查阅. 对小于128bytes的
 * 片段使用二级空间配置器. 部分代码相比STL进行了少量调整. 
 * 本文件参考了侯捷《STL源码剖析》, 特此说明并致谢
 ***********************************************************/

/************************************************************
 * 更新日志:
 * 2020.04.14 针对_has_trivial_destructor进行了优化
 * 2020.05.02 对部分代码进行了调整
 * 2020.05.04 重写了_destroy(), 提高了代码效率
 * 2020.05.17 添加了二级空间配置器sub_allocator
 * 2020.05.19 添加了operator==与!=, 提高了兼容性
 ***********************************************************/

#ifndef MY_ALLOCATOR
#define MY_ALLOCATOR

#include <new>
#include <cstdlib>
#include <iostream>
#include <bits/c++config.h>
#include <bits/functexcept.h>

//#include <bits/move.h>
//#include <bits/postypes.h>
//#include <memory>
#if __cplusplus >= 201103L
//#include <type_traits>
#endif

#include "my_type_traits.h"
#include "my_iterator.h"
#include "my_move.h"
#include "my_alloc_mem.h"

#define USE_SUB_ALLOCATOR

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
    template<typename _Tp1>
    inline void _construct(_Tp1* __p, const _Tp1& __val)
	{ ::new((void *)__p) _Tp1(__val); }

    // construct的实现, 直接调用对象的有参构造函数
    template<typename _Tp1, typename... _Vals>
    inline void _construct(_Tp1* __p, _Vals&&... __vals)
	{ ::new((void *)__p) _Tp1(my_lib::forward<_Vals>(__vals)...); }

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


    /* destroy的实现之二, 使用函数参数类型推断

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
        /*不能使用以下方法: 在_destroy_aux处使用__true_type推断, 而在这里使用如下代码:
        _destroy_aux<has_trivial_destructor<_Value_type>>::__destroy(_first, _last);
        原因在于编译器会在_destroy_aux<...>处直接推断为non-trivial, 
        而不会先推断has_trivial_destructor<_Value_type>再推断_destroy_aux<...>*/

        /*也不能使用以下方法: 用___has_trivial_destructor(_Value_type()).value推断, 
        因为这是试图用函数返回值推断, 是无法进行的*/

        /*另一种实现, 使用inline函数进行函数参数类型推断
        __destroy(_first, _last, _has_trivial_destructor(_Value_type())); /* 括号用于生成临时对象 */
    }

    // STL对char*与wchar_t*设计了特化的_destroy(), 此处模仿之设置char*与wchar_t*的特化
    inline void _destroy(char*, char*) { }
    inline void _destroy(wchar_t*, wchar_t*) { }

#else
    // construct的实现, 直接调用对象的无参构造函数
    template<typename _Tp1>
    inline void _construct(_Tp1* __p)
	{ ::new((void *)__p) _Tp1(); }

    // construct的实现, 直接调用对象的有参构造函数
    template<typename _Tp1>
    inline void _construct(_Tp1* __p, const _Tp1& __val)
	{ ::new((void *)__p) _Tp1(__val); }

    // construct的实现, 直接调用对象的有参构造函数
    //template<typename _Tp1, typename... _Vals>
    //inline void _construct(_Tp1* __p, _Vals&&... __vals)
	//{ ::new((void *)__p) _Tp1(std::forward<_Vals>(__vals)...); }

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

        // rebind allocator of type _Tp1 通过rebind可以获得当前所用配置器的类模板
        template<typename _Tp1>
        struct rebind
        { typedef allocator<_Tp1> other; };

#if __cplusplus >= 201103L
        // _GLIBCXX_RESOLVE_LIB_DEFECTS
        // 2103. propagate_on_container_move_assignment
        typedef std::true_type propagate_on_container_move_assignment;
#endif

#ifndef USE_SUB_ALLOCATOR
        // allocate, deallocate, construct和destroy函数均调用上面的实际实现
        // hint used for locality. ref.[Austern],p189
        pointer allocate(size_type _n, const void* hint = 0) {
            return _allocate((difference_type)_n, (pointer)0);
        }
        void deallocate(pointer _p, size_type _n) { _deallocate(_p); }

#else
        // Enable sub_allocator for size<=128bytes
        
        // allocate函数, 调用_allocate()或sub_allocator
        pointer allocate(size_type _n, const void* hint = 0) 
        {
            if (_n * sizeof(value_type) > 128) 
            { return _allocate((difference_type)_n, (pointer)0); }
            else { return _sub_allocate((difference_type)_n, (pointer)0); }
        }

        // deallocate函数, 调用_deallocate()或sub_allocator
        void deallocate(pointer _p, size_type _n) 
        { 
            if (_n * sizeof(value_type) > 128)  { _deallocate(_p);  }
            else { _sub_deallocate(_p, _n * sizeof(value_type)); }
        }
        
#endif

#if __cplusplus >= 201103L
        void construct(pointer _p) { _construct(_p); }
        void construct(pointer _p, const _Tp& _Arg) { _construct(_p, _Arg); }

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

        pointer address(reference _x) const _GLIBCXX_NOEXCEPT { return (pointer)&_x; }
        const_pointer const_address(const_reference _x) const _GLIBCXX_NOEXCEPT 
        { return (const_pointer)&_x; }
        size_type max_size() const _GLIBCXX_NOEXCEPT { return size_type(UINT_MAX / sizeof(_Tp)); }   
    };

    // 以下代码在STL的许多库(如vector.tcc)中有用, 不可删去
    // allocator_always_compares_equal, 所有my_lib::allocator都相等
    template<typename _T1, typename _T2>
    inline bool
    operator==(const allocator<_T1>&, const allocator<_T2>&)
    { return true; }

    template<typename _Tp>
    inline bool
    operator==(const allocator<_Tp>&, const allocator<_Tp>&)
    { return true; }

    template<typename _T1, typename _T2>
    inline bool
    operator!=(const allocator<_T1>&, const allocator<_T2>&)
    { return false; }

    template<typename _Tp>
    inline bool
    operator!=(const allocator<_Tp>&, const allocator<_Tp>&)
    { return false; }

} // namespace my_lib

#endif // MY_ALLOCATOR    template<typename _ForwardIterator>
        void destroy(_ForwardIterator _first, _ForwardIterator _last) 
        { _destroy(_first, _last); }
#else
        void construct(pointer _p) { _construct(_p); }
        void construct(pointer _p, const _Tp& _Arg) { _construct(_p, _Arg); }
        void destroy(pointer _p) { _destroy(_p); }
#endif

        pointer address(reference _x) const _GLIBCXX_NOEXCEPT { return (pointer)&_x; }
        const_pointer const_address(const_reference _x) const _GLIBCXX_NOEXCEPT 
        { return (const_pointer)&_x; }
        size_type max_size() const _GLIBCXX_NOEXCEPT { return size_type(UINT_MAX / sizeof(_Tp)); }   
    };

    // 以下代码在STL的许多库(如vector.tcc)中有用, 不可删去
    // allocator_always_compares_equal, 所有my_lib::allocator都相等
    template<typename _T1, typename _T2>
    inline bool
    operator==(const allocator<_T1>&, const allocator<_T2>&)
    { return true; }

    template<typename _Tp>
    inline bool
    operator==(const allocator<_Tp>&, const allocator<_Tp>&)
    { return true; }

    template<typename _T1, typename _T2>
    inline bool
    operator!=(const allocator<_T1>&, const allocator<_T2>&)
    { return false; }

    template<typename _Tp>
    inline bool
    operator!=(const allocator<_Tp>&, const allocator<_Tp>&)
    { return false; }

} // namespace my_lib

#endif // MY_ALLOCATOR
