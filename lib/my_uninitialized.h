/**********************************************************
 * 
 *                        My Lib 
 *                  my_uninitialized.h
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
 * 本文件包含部分常用内存处理函数. 本文件使用了traits技巧并进行
 * 了相对详尽的注释以备温习或查阅. 部分代码相比STL进行了少量调整
 * 本文件参考了侯捷《STL源码剖析》, 特此说明并致谢
 ***********************************************************/

/************************************************************
 * 更新日志:
 * 2020.05.21 添加了部分功能函数
 ***********************************************************/

#ifndef MY_UNINITIALIZED
#define MY_UNINITIALIZED

#include <cstdlib>
#include <bits/c++config.h>
#include <bits/functexcept.h>

#include "my_type_traits.h"
#include "my_iterator.h"
#include "my_algobase.h"
#include "my_allocator.h"

namespace my_lib
{
	// uninitialized_fill_n 利用 __value 构造对象
	// non-POD 泛化
	template<bool>
    struct __uninitialized_fill_n_aux
    {
      	template<typename _Forward_Iter, typename _Tp>
        static _Forward_Iter
        __uninitialized_fill_n(_Forward_Iter __first, size_t __n, const _Tp& __value)
        {
			for (; __n > 0; --__n, ++__first) { _construct(*(&__first), __value); }
			return __first;
		}
    };

	// POD 偏特化
	template<>
    struct __uninitialized_fill_n_aux<true>
    {
      	template<typename _Forward_Iter, typename _Tp>
        static _Forward_Iter
        __uninitialized_fill_n(_Forward_Iter __first, size_t __n, const _Tp& __value)
        { 
			return fill_n(__first, __n, __value); 
		}
    };

	// Fill [__fiset, __fiset + __n) with construct(pointer, __value)
	template<typename _Forward_Iter, typename _Tp>
    inline _Forward_Iter
    uninitialized_fill_n(_Forward_Iter __first, size_t __n, const _Tp& __value)
    {
		// POD 即 Plain Old Data, Old 似乎是相对于C/C++而言的, 即C语言中的data, 
		// 主要包含标量类型(例如int)和不含函数的结构体. POD 必然有平凡的构造/析构函数
		typedef typename iterator_traits<_Forward_Iter>::value_type value_type;
		return __uninitialized_fill_n_aux<is_POD_type<value_type>::value>::\
		__uninitialized_fill_n(__first, __n, __value); 
	}

	// Fill [__fiset, __fiset + __n) with __value for char 特化
    inline char* uninitialized_fill_n(char* __first, size_t __n, const char& __value)
    {
		// 对char可用内建函数增加效率
		__builtin_memset(__first, __value, __n);
		return __first + __n; 
	}

	// uninitialized_fill 利用 __value 构造对象
	// non-POD 泛化
	template<bool>
    struct __uninitialized_fill_aux
    {
      	template<typename _Forward_Iter, typename _Tp>
        static void
        __uninitialized_fill(_Forward_Iter __first, _Forward_Iter __last, const _Tp& __value)
        {
			for (; __first != __last; ++__first) { _construct(*(&__first), __value); }
		}
    };

	// POD 偏特化
	template<>
    struct __uninitialized_fill_aux<true>
    {
      	template<typename _Forward_Iter, typename _Tp>
        static void
        __uninitialized_fill(_Forward_Iter __first, _Forward_Iter __last, const _Tp& __value)
        { 
			fill(__first, __last, __value); 
		}
    };

	// Fill [__fiset, __last) with construct(pointer, __value)
	template<typename _Forward_Iter, typename _Tp>
    inline void
    uninitialized_fill(_Forward_Iter __first, _Forward_Iter __last, const _Tp& __value)
    { 
		typedef typename iterator_traits<_Forward_Iter>::value_type value_type;
		__uninitialized_fill_aux<is_POD_type<value_type>::value>::\
		__uninitialized_fill(__first, __last, __value); 
	}

	// Fill [__fiset, __last) with __value for char 特化
    inline void uninitialized_fill(char* __first, char* __last, const char& __value)
    {
		__builtin_memset(__first, __value, __last - __first);
	}

	// uninitialized_copy 利用 __value 构造对象
	// non-POD 泛化
	template<bool>
    struct __uninitialized_copy_aux
    {
      	template<typename _Input_Iter, typename _Forward_Iter>
        static _Forward_Iter
        __uninitialized_copy(_Input_Iter __first, _Input_Iter __last, _Forward_Iter __result)
        {
			for (; __first != __last; ++__result ,++__first) { _construct(*(&__result), *__first); }
			return __result;
		}
    };

	// POD 偏特化
	template<>
    struct __uninitialized_copy_aux<true>
    {
      	template<typename _Input_Iter, typename _Forward_Iter>
        static _Forward_Iter
        __uninitialized_copy(_Input_Iter __first, _Input_Iter __last, _Forward_Iter __result)
        { 
			return copy(__first, __last, __result); 
		}
    };

	// Copy [__fiset, __last) into __result with constructor
	template<typename _Input_Iter, typename _Forward_Iter>
    inline _Forward_Iter
    uninitialized_fill(_Input_Iter __first, _Input_Iter __last, _Forward_Iter __result)
    { 
		typedef typename iterator_traits<_Forward_Iter>::value_type value_type;
		return __uninitialized_copy_aux<is_POD_type<value_type>::value>::\
		__uninitialized_copy(__first, __last, __result); 
	}

	// Copy [__fiset, __last) into __result for char 特化
    inline char* uninitialized_copy(char* __first, char* __last, char* __result)
    {
		__builtin_memmove(__result, __first, __last - __first);
		return __result + (__last - __first);
	}

	// Copy [__fiset, __last) into __result for wchar_t 特化
    inline wchar_t* uninitialized_copy(wchar_t* __first, wchar_t* __last, wchar_t* __result)
    {
		__builtin_memmove(__result, __first, sizeof(wchar_t) * (__last - __first));
		return __result + (__last - __first);
	}

	
} // namespace my_lib


#endif // MY_UNINITIALIZED