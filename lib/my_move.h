/**********************************************************
 * 
 *                        My Lib 
 *                       my_move.h
 * 
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
 * 本文件包含一些常用的函数. 除中文注释和部分删减外相比STL未改动
 * 本文件参考了侯捷《STL源码剖析》, 特此说明并致谢
 ***********************************************************/

/************************************************************
 * 更新日志:
 * 2020.05.24 复制自bits\move.h并添加了中文注释
 ***********************************************************/

#ifndef MY_MOVE
#define MY_MOVE

#include <bits/c++config.h>
#include <bits/concept_check.h>

namespace my_lib
{
	// address 的实现, 简单封装&, 返回对象的地址
  	template<typename _Tp>
    inline _Tp* __addressof(_Tp& __r)
    {
    	return reinterpret_cast<_Tp*>
		(&const_cast<char&>(reinterpret_cast<const volatile char&>(__r)));
    }

	// address 的实现, 简单封装&, 返回对象的地址
	template<typename _Tp>
    inline _Tp*
    addressof(_Tp& __r) noexcept
    { return __addressof(__r); }

} // namespace my_lib

#if __cplusplus >= 201103L
#include "my_type_traits.h"

namespace my_lib
{
	// 获得右值引用, 用于参数包
    template<typename _Tp>
    constexpr _Tp&&
    forward(typename remove_reference<_Tp>::type& __t) // rvalue
    { return static_cast<_Tp&&>(__t); }

    // 获得右值引用, 用于参数包
    template<typename _Tp>
    constexpr _Tp&&
    forward(typename remove_reference<_Tp>::type&& __t) // rvalue
    {
        static_assert(!is_lvalue_reference<_Tp>::value, "template argument"
		    " substituting _Tp is an lvalue reference type");
        return static_cast<_Tp&&>(__t);
    }

	// 获得右值引用
  	template<typename _Tp>
    constexpr typename remove_reference<_Tp>::type&&
    move(_Tp&& __t) noexcept
    { return static_cast<typename remove_reference<_Tp>::type&&>(__t); }

} // namespace my_lib

#define _MY_LIB_MOVE(__val) my_lib::move(__val)
#define _MY_LIB_FORWARD(_Tp, __val) my_lib::forward<_Tp>(__val)
#else
#define _MY_LIB_MOVE(__val) (__val)
#define _MY_LIB_FORWARD(_Tp, __val) (__val)
#endif

#endif /* MY_MOVE */