/**********************************************************
 * 
 *                        My Lib 
 *                     my_algobase.h
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
 * 本文件包含部分不依赖于容器的常用功能函数. 本文件使用了traits
 * 技巧并进行了相对详尽的注释以备温习或查阅. 部分代码相比STL进行
 * 了少量调整.
 * 本文件参考了侯捷《STL源码剖析》, 特此说明并致谢
 ***********************************************************/

/************************************************************
 * 更新日志:
 * 2020.05.21 添加了部分功能函数
 * 2020.05.22 添加了部分功能函数
 ***********************************************************/

#ifndef MY_ALGOBASE
#define MY_ALGOBASE

#include <cstdlib>
#include <bits/c++config.h>
#include <bits/functexcept.h>

#include "my_type_traits.h"
#include "my_iterator.h"

namespace my_lib
{
    // 基础数据处理函数, 不涉及容器与内存写入

    // min(a,b)
    template<typename _Tp>
    inline const _Tp&
    min(const _Tp& __a, const _Tp& __b)
    {
        //return __b < __a ? __b : __a;
        if (__b < __a) { return __b; }
        else { return __a; }
    }

    // min(a,b) for number type
    inline const long double&
    min(const long double& __a, const long double& __b) { return (__b < __a ? __b : __a); }

    //inline const size_t&
    //min(const size_t& __a, const size_t& __b) { return (__b < __a ? __b : __a); }

    // max(a,b)
    template<typename _Tp>
    inline const _Tp&
    max(const _Tp& __a, const _Tp& __b)
    {
        //return  __a < __b ? __b : __a;
        if (__a < __b) { return __b; }
        else { return __a; }
    }

    // max(a,b) for number type
    inline const long double&
    max(const long double& __a, const long double& __b) { return (__a < __b ? __b : __a); }

    //inline const size_t&
    //max(const size_t& __a, const size_t& __b) { return (__a < __b ? __b : __a); }

    // min(a,b) for given compare(a,b) (true if a < b) __comp() 为仿函数
    template<typename _Tp, typename _Compare>
    inline const _Tp&
    min(const _Tp& __a, const _Tp& __b, _Compare __comp)
    {
        //return __comp(__b, __a) ? __b : __a;
        if (__comp(__b, __a)) { return __b; }
        else { return __a; }
    }

    // max(a,b) for given compare(a,b) (true if a < b) __comp() 为仿函数
    template<typename _Tp, typename _Compare>
    inline const _Tp&
    max(const _Tp& __a, const _Tp& __b, _Compare __comp)
    {
        //return __comp(__a, __b) ? __b : __a;
        if (__comp(__a, __b)) { return __b; }
        else { return __a; }
    }


    // 相等判断 [first1,last1)==[first2,first2+(last1-first1))
    // 泛化non-trivial
    template<bool _BoolType>
    struct __equal_aux
    {
        template<typename _Input_Iter1, typename _Input_Iter2>
        static bool
        __equal(_Input_Iter1 __first1, _Input_Iter1 __last1, _Input_Iter2 __first2)
        {
	        for (; __first1 != __last1; ++__first1, ++__first2)
            { if (!(*__first1 == *__first2)) { return false; } }
	        return true;
	    }
    };

    // 特化
    template<>
    struct __equal_aux<true>
    {
        template<typename _Tp>
        static bool
        __equal(const _Tp* __first1, const _Tp* __last1, const _Tp* __first2)
        {
	        return (!__builtin_memcmp(__first1, __first2, sizeof(_Tp) * (__last1 - __first1)));
	    }
    };

    // equal [first1,last1)==[first2,first2+(last1-first1)) 判断两个序列是否相等
    template<typename _Input_Iter1, typename _Input_Iter2>
    inline bool
    equal(_Input_Iter1 __first1, _Input_Iter1 __last1, _Input_Iter2 __first2)
    {
        typedef typename iterator_traits<_Input_Iter1>::value_type _ValueType1;
        typedef typename iterator_traits<_Input_Iter2>::value_type _ValueType2;
        const bool __simple = ((is_integer<_ValueType1>::value
			                 || is_pointer<_ValueType1>::value)
	                         && is_pointer<_Input_Iter1>::value
	                         && is_pointer<_Input_Iter2>::value
			                 && are_same_type<_ValueType1, _ValueType2>::value);

        return __equal_aux<__simple>::__equal(__first1, __last1, __first2);
    }

    // equal [first1,last1)==[first2,first2+(last1-first1)), 利用给定的 __cmp_equal (true if equal) 判断
    template<typename _Input_Iter1, typename _Input_Iter2, typename _Cmp_equal>
    inline bool
    equal(_Input_Iter1 __first1, _Input_Iter1 __last1, _Input_Iter2 __first2, _Cmp_equal __cmp_equal)
    {
	    for (; __first1 != __last1; ++__first1, ++__first2)
        { if (!__cmp_equal(*__first1, *__first2)) { return false; } }
	    return true;
	}

    // 区别比较 Finds the places in ranges which don't match
    // return [__first1, __last1) 中与 [__first2 , __first2 + (__last1 - __first1)) 第一个不相等位的迭代器
    // 不想自己重新写一遍pair, 所以仅返回[__first1,__last1)中的迭代器.
    template<typename _Input_Iter1, typename _Input_Iter2>
    _Input_Iter1 mismatch(_Input_Iter1 __first1, _Input_Iter1 __last1, _Input_Iter2 __first2)
    {
        for (; __first1 != __last1 && *__first1 == *__first2; ++__first1, ++__first2) { }
        return __first1;
	}

    // Finds the places in ranges which don't match, 利用给定的 __cmp_equal (true if equal) 仿函数判断
    // return [__first1, __last1) 中与 [__first2 , __first2 + (__last1 - __first1)) 第一个不相等位的迭代器
    template<typename _Input_Iter1, typename _Input_Iter2, typename _Cmp_equal>
    _Input_Iter1 mismatch(_Input_Iter1 __first1, _Input_Iter1 __last1, 
                            _Input_Iter2 __first2, _Cmp_equal __cmp_equal)
    {
        for (; __first1 != __last1 && __cmp_equal(*__first1, *__first2); ++__first1, ++__first2) { }
        return __first1;
	}

    // count() return [__first, __last) 中与 __value 相等的元素个数
    template<typename _Input_Iter, typename _Tp>
    typename iterator_traits<_Input_Iter>::difference_type
    count(_Input_Iter __first, _Input_Iter __last, const _Tp& __value)
    {
        typename iterator_traits<_Input_Iter>::difference_type __n = 0;
        for (; __first != __last ; ++__first) 
        { if (*__first == __value) { ++__n; } }
        return __n;
    }

    // count() return [__first, __last) 中与 __value 相等的元素个数, 利用 __cmp_equal (true if equal) 仿函数
    template<typename _Input_Iter, typename _Tp, typename _Cmp_equal>
    typename iterator_traits<_Input_Iter>::difference_type
    count(_Input_Iter __first, _Input_Iter __last, const _Tp& __value, _Cmp_equal __cmp_equal)
    {
        typename iterator_traits<_Input_Iter>::difference_type __n = 0;
        for (; __first != __last ; ++__first) 
        { if (__cmp_equal(*__first, __value)) { ++__n; } }
        return __n;
    }

    // count_if() return [__first, __last) 中 __cond == true 的元素个数, __cond() 为仿函数
    template<typename _Input_Iter, typename _Cond>
    typename iterator_traits<_Input_Iter>::difference_type
    count_if(_Input_Iter __first, _Input_Iter __last, _Cond __cond)
    {
        typename iterator_traits<_Input_Iter>::difference_type __n = 0;
        for (; __first != __last ; ++__first) 
        { if (__cond(*__first)) { ++__n; } }
        return __n;
    }

    // exist() return [__first, __last) 中是否存在与 __value 相等的元素
    template<typename _Input_Iter, typename _Tp>
    bool exist(_Input_Iter __first, _Input_Iter __last, const _Tp& __value)
    {
        for (; __first != __last ; ++__first) 
        { if (*__first == __value) { return true; } }
        return false;
    }

    // exist() return [__first, __last) 中是否存在与 __value 相等的元素
    // 利用 __cmp_equal (true if equal) 仿函数
    template<typename _Input_Iter, typename _Tp, typename _Cmp_equal>
    bool exist(_Input_Iter __first, _Input_Iter __last, const _Tp& __value, _Cmp_equal __cmp_equal)
    {
        typename iterator_traits<_Input_Iter>::difference_type __n = 0;
        for (; __first != __last ; ++__first) 
        { if (__cmp_equal(*__first, __value)) { return true; } }
        return false;
    }

    // exist_if() return [__first, __last) 中是否存在 __cond == true 的元素, __cond() 为仿函数
    template<typename _Input_Iter, typename _Cond>
    bool exist_if(_Input_Iter __first, _Input_Iter __last, _Cond __cond)
    {
        typename iterator_traits<_Input_Iter>::difference_type __n = 0;
        for (; __first != __last ; ++__first) 
        { if (__cond(*__first)) { return true; } }
        return false;
    }

    // find() return [__first, __last) 中第一个与 __value 相等的元素的迭代器
    template<typename _Input_Iter, typename _Tp>
    _Input_Iter find(_Input_Iter __first, _Input_Iter __last, const _Tp& __value)
    {
        for (; __first != __last && *__first != __value; ++__first) { }
        return __first;
    }

    // find() return [__first, __last) 中第一个与 __value 相等的元素的迭代器
    // 利用 __cmp_equal (true if equal) 仿函数
    template<typename _Input_Iter, typename _Tp, typename _Cmp_equal>
    _Input_Iter find(_Input_Iter __first, _Input_Iter __last, const _Tp& __value, _Cmp_equal __cmp_equal)
    {
        for (; __first != __last && !__cmp_equal(*__first, __value); ++__first) { }
        return __first;
    }

    // find() return [__first, __last) 中第一个 __cond == true 的元素的迭代器, __cond() 为仿函数
    template<typename _Input_Iter, typename _Tp, typename _Cond>
    _Input_Iter find_if(_Input_Iter __first, _Input_Iter __last, _Cond __cond)
    {
        for (; __first != __last && !__cond(*__first); ++__first) { }
        return __first;
    }

    // for_each element in [__first, __last) do __function(), __function() 为仿函数
    template<typename _Input_Iter, typename _Function>
    _Function for_each(_Input_Iter __first, _Input_Iter __last, _Function __function)
    {
        for (; __first != __last; ++__first) { __function(*__first); }
        return __function; // 通常并没有什么用, 之前__function()的返回值全丢了
    }

    // function on [__first1, __last1) to [__first1, __first2 + (__last1 - __first1))
    // 将对 [__first1, __last1) 每一元素运算的结果存储至 [__first1, __first2 + (__last1 - __first1))
    // __function() 为仿函数
    template<typename _Input_Iter, typename _Forward_Iter, typename _Function>
    _Function function_on(_Input_Iter __first1, _Input_Iter __last1, 
                            _Forward_Iter __first2, _Function __function)
    {
        for (; __first1 != __last1; ++__first1, ++__first2) 
        { *__first2 = __function(*__first1); } // 保存每一个返回值的正确示范
        return __function;
    }

    // generate elements on [__first, __last) with __generate() 仿函数
    template<typename _Output_Iter, typename _Generator>
    void generate(_Output_Iter __first, _Output_Iter __last, _Generator __generate)
    {
        for (; __first != __last; ++__first) { *__first = __generate(); }
    }

    // generate elements on [__first, __first + __n) with __generate() 仿函数
    template<typename _Output_Iter, typename _Generator>
    _Output_Iter generate_n(_Output_Iter __first, size_t __n, _Generator __generate)
    {
        for (; __n > 0; --__n, ++__first) { *__first = __generate(); }
        return __first;
    }

    // min_element in [__first, __last) 获得 [__first, __last) 中的最小值
    template<typename _Forward_Iter>
    _Forward_Iter min_element(_Forward_Iter __first, _Forward_Iter __last)
    {
        _Forward_Iter __result = __first;
        for (; __first != __last; ++__first) 
        { if (*__first < *__result) { __result = __first; } }
        return __result;
    }

    // min_element in [__first, __last) 获得 [__first, __last) 中的最小值 
    // __comp(a,b) == true if a < b, __comp() 为仿函数
    template<typename _Forward_Iter, typename _Comp>
    _Forward_Iter min_element(_Forward_Iter __first, _Forward_Iter __last, _Comp __comp)
    {
        _Forward_Iter __result = __first;
        for (; __first != __last; ++__first) 
        { if (__comp(*__result, *__first)) { __result = __first; } }
        return __result;
    }

    // max_element in [__first, __last) 获得 [__first, __last) 中的最大值
    template<typename _Forward_Iter>
    _Forward_Iter max_element(_Forward_Iter __first, _Forward_Iter __last)
    {
        _Forward_Iter __result = __first;
        for (; __first != __last; ++__first) 
        { if (*__first > *__result) { __result = __first; } }
        return __result;
    }

    // max_element in [__first, __last) 获得 [__first, __last) 中的最大值 
    // __comp(a,b) == true if a < b, __comp() 为仿函数
    template<typename _Forward_Iter, typename _Comp>
    _Forward_Iter max_element(_Forward_Iter __first, _Forward_Iter __last, _Comp __comp)
    {
        _Forward_Iter __result = __first;
        for (; __first != __last; ++__first) 
        { if (!__comp(*__result, *__first)) { __result = __first; } }
        return __result;
    }

    // replace __old_value in [__first, __last) by __new_value
    // 将 [__first, __last) 中的 __old_value 替换为 __new_value
    template<typename _Forward_Iter, typename _Tp>
    void replace(_Forward_Iter __first, _Forward_Iter __last, 
                    const _Tp& __old_value, const _Tp& __new_value)
    {
        for (; __first != __last; ++__first) 
        { if (*__first == __old_value) { *__first = __new_value; } }
    }

    // replace __old_value in [__first, __last) by __new_value then copy to __result
    // 将 [__first, __last) 中的 __old_value 替换为 __new_value
    // 不改变原序列的值, 而将替换后的序列复制到 __result 处
    template<typename _Input_Iter, typename _Output_Iter, typename _Tp>
    _Output_Iter replace_copy(_Input_Iter __first, _Input_Iter __last, _Output_Iter __result,
                                const _Tp& __old_value, const _Tp& __new_value)
    {
        for (; __first != __last; ++__first, ++__result) 
        { *__result = *__first == __old_value ? __new_value : *__first; }
        return __result;
    }

    // replace elements in [__first, __last) whose __cond == true by __new_value
    // 将 [__first, __last) 中 __cond == true 的元素替换为 __new_value, __cond() 为仿函数
    template<typename _Forward_Iter, typename _Tp, typename _Cond>
    void replace_if(_Forward_Iter __first, _Forward_Iter __last, _Cond __cond, const _Tp& __new_value)
    {
        for (; __first != __last; ++__first) 
        { if (__cond(*__first)) { *__first = __new_value; } }
    }

    // replace elements in [__first, __last) whose __cond == true by __new_value then copy to __result
    // 将 [__first, __last) 中 __cond == true 的元素替换为 __new_value, __cond() 为仿函数
    // 不改变原序列的值, 而将替换后的序列复制到 __result 处
    template<typename _Input_Iter, typename _Output_Iter, typename _Tp, typename _Cond>
    _Output_Iter replace_copy_if(_Input_Iter __first, _Input_Iter __last, _Output_Iter __result,
                                _Cond __cond, const _Tp& __new_value)
    {
        for (; __first != __last; ++__first, ++__result) 
        { *__result = __cond(*__first) ? __new_value : *__first; }
        return __result;
    }

    // select elements in [__first, __last) whose __cond == true and copy to __result
    // 将 [__first, __last) 中 __cond == true 的元素复制到 __result, __cond() 为仿函数
    template<typename _Input_Iter, typename _Output_Iter, typename _Tp, typename _Cond>
    _Output_Iter select(_Input_Iter __first, _Input_Iter __last, _Output_Iter __result, _Cond __cond)
    {
        // 注意原本在 [__first, __last) 中可能离散的值复制后连续化了!
        for (; __first != __last; ++__first) 
        { if (__cond(*__first)) { *__result = __first; ++__result; } }
        return __result;
    }






    // 内存处理函数, 仅处理内存, 不构造对象!

    // __fill 泛化 for Input_Iterator
    template<typename>
    struct __fill_aux
    {
        template<typename _Forward_Iter, typename _Tp>
        static void __fill(_Forward_Iter __first, _Forward_Iter __last, const _Tp& __value) 
        {
            for (; __first != __last; ++__first) { *__first = __value; }
        }
    };

    // __fill 特化 for Random_Access_Iterator
    template<>
    struct __fill_aux<random_access_iterator_tag>
    {
        template<typename _Random_Access_Iter, typename _Tp>
        static void __fill(_Random_Access_Iter __first, _Random_Access_Iter __last, const _Tp& __value) 
        {
            typename iterator_traits<_Random_Access_Iter>::difference_type __n;
            for (__n = __last - __first; __n > 0 ; --__n, ++__first) { *__first = __value; }
        }
    };
    
    // fill [__first,__last) with value
    template<typename _Forward_Iter, typename _Tp>
    void fill(_Forward_Iter __first, _Forward_Iter __last, const _Tp& __value) 
    {
        typedef typename iterator_traits<_Forward_Iter>::iterator_category    _Category;
        __fill_aux<_Category>::__fill(__first, __last, __value);
    }

    // fill [__first,__first + __n) with value, return __first + __n
    template<typename _Forward_Iter, typename _Tp>
    _Forward_Iter fill_n(_Forward_Iter __first, size_t __n, const _Tp& value) 
    {
        for (; __n > 0 ; --__n, ++__first) { *__first = value; }
        return __first;
    }

    // iter_swap, swap *__a <=> *__b
    template<typename _Forward_Iter1, typename _Forward_Iter2>
    inline void iter_swap(_Forward_Iter1 __a, _Forward_Iter2 __b)
    {
        typedef typename iterator_traits<_Forward_Iter1>::value_type value_type;
        value_type tmp = *__a; //直接用类型名作参数推断, 比生成临时对象快
        *__a = *__b;
        *__b = tmp;
	}

    // swap, swap &__a <=> &__b
    template<typename _Tp>
    inline void swap(_Tp& __a, _Tp& __b)
    {
        _Tp tmp = __a; 
        __a = __b;
        __b = tmp;
	}


    // 内存处理函数 Copy [__first,__last) into __result

    // __copy 泛化 for non-trivial assignment and Input_Iterator
    template<bool, typename>
    struct __copy_aux
    {
        template<typename _Input_Iter, typename _Output_Iter>
        static _Output_Iter 
        __copy(_Input_Iter __first, _Input_Iter __last, _Output_Iter __result)
        {
            for (; __first != __last ; ++__first, ++__result) { *__result = *__first; }
            return __result;
        }
    };

    // __copy 特化/强化 for trivial assignment and Random_Access_Iterator
    template<>
    struct __copy_aux<true, random_access_iterator_tag>
    {
        // 原生指针
        template<typename _Tp>
        static _Tp* __copy(const _Tp* __first, const _Tp* __last, _Tp* __result)
        {
            __builtin_memmove(__result, __first, sizeof(_Tp) * (__last - __first));
            return __result + (__last - __first);
        }

        // 其他 random_access 且 value_type trivial 的迭代器(要求operator&对迭代器有定义)
        template<typename _Random_Access_Iter, typename _Output_Iter>
        static _Output_Iter
        __copy(_Random_Access_Iter __first, _Random_Access_Iter __last, _Output_Iter __result)
        {
            typedef typename iterator_traits<_Random_Access_Iter>::value_type value_type;
            // 通过*(&Iterator)保证所取地址是迭代器指向的对象的地址, 而非迭代器本身的地址
            __builtin_memmove(*(&__result), *(&__first), sizeof(value_type) * (__last - __first));
            return __result + (__last - __first);
        }
    };

    // __copy 特化 for non-trivial assignment and Random_Access_Iterator
    template<>
    struct __copy_aux<false, random_access_iterator_tag>
    {
        template<typename _Random_Access_Iter, typename _Output_Iter>
        static _Output_Iter 
        __copy(_Random_Access_Iter __first, _Random_Access_Iter __last, _Output_Iter __result)
        {
            typename iterator_traits<_Random_Access_Iter>::difference_type __n;
            for (__n = __last - __first; __n > 0 ; --__n, ++__first, ++__result) 
            { *__result = *__first; }
            return __result;
        }
    };

    /*/以上给出的是一个参考了 libstdc++-v3 的版本, 尽可能避免了复杂的多层泛化/偏特化.
    // 以下为《STL源码剖析》中 copy() 的实现, 用了四级的泛化/偏特化/强化, 结构过于复杂, 不用.
    // __copy_d() 第四级强化 for Random_Access_Iterator
    template<typename _Random_Access_Iter, typename _Output_Iter, typename _Difference>
    inline _Output_Iter 
    __copy_d(_Random_Access_Iter __first, _Random_Access_Iter __last, 
                _Output_Iter __result, _Difference)
    {
        std::cerr<<"__copy_d"<<std::endl;
        for (_Difference n = __last - __first; n > 0 ; --n, ++__first, ++__result) 
        { *__result = *__first; }
        return __result;
    }

    // __copy() 第三级特化 for Input_Iterator
    template<typename _Input_Iter, typename _Output_Iter>
    inline _Output_Iter 
    __copy(_Input_Iter __first, _Input_Iter __last, _Output_Iter __result, input_iterator_tag)
    {
        std::cerr<<"__copy II"<<std::endl;
        for (; __first != __last ; ++__first, ++__result) { *__result = *__first; }
        return __result;
    }

    // __copy() 第三级特化 for Random_Access_Iterator
    template<typename _Random_Access_Iter, typename _Output_Iter>
    inline _Output_Iter 
    __copy(_Random_Access_Iter __first, _Random_Access_Iter __last, 
                _Output_Iter __result, random_access_iterator_tag)
    {
        return __copy_d(__first, __last, __result, difference_type(__first));
    }

    // __copy_t() 第三级特化 for value_type without trivial_assignment_operator
    template<bool>
    struct __copy_t_aux
    {
        template<typename _Tp>
        static _Tp* __copy_t(const _Tp* __first, const _Tp* __last, const _Tp* __result)
        {
            // difference_type(const _Tp*) 就是 ptrdiff_t(), 直接代入
            return __copy_d(__first, __last, __result, ptrdiff_t());
        }
    };

    // __copy_t() 第三级特化 for value_type with trivial_assignment_operator
    template<>
    struct __copy_t_aux<true>
    {
        template<typename _Tp>
        static _Tp* __copy_t(const _Tp* __first, const _Tp* __last, _Tp* __result)
        {
            std::cerr<<"__copy_t memmove"<<std::endl;
            __builtin_memmove(__result, __first, sizeof(_Tp) * (__last - __first));
            return __result + (__last - __first);
        }
    };
    
    // _copy_dispatch_aux<> 第二级泛化 for iterator
    template<typename _Input_Iter, typename _Output_Iter>
    struct _copy_dispatch_aux
    {
        //《STL源码剖析》通过operator()仿函数实现第三级的特化
        _Output_Iter operator()(_Input_Iter __first, _Input_Iter __last, _Output_Iter __result)
        {
            // 通过临时对象实现参数自动推导
            return __copy(__first, __last, __result, iterator_category(__first));
        }
    };

    // _copy_dispatch_aux<> 第二级特化 for pointer
    template<typename _Tp>
    struct _copy_dispatch_aux<_Tp*, _Tp*>
    {
        // const bool __simple = has_trivial_assignment_operator<_Tp>::value;
        _Tp* operator()(_Tp* __first, _Tp* __last, _Tp* __result)
        {
            return __copy_t_aux<has_trivial_assignment_operator<_Tp>::value>::__copy_t(__first, __last, __result);
        }
    };

    // _copy_dispatch_aux<> 第二级特化 for const pointer
    template<typename _Tp>
    struct _copy_dispatch_aux<const _Tp*, _Tp*>
    {
        typedef has_trivial_assignment_operator<_Tp> if_trivial_assigned;
        _Tp* operator()(const _Tp* __first, const _Tp* __last, _Tp* __result)
        {
            return __copy_t_aux<if_trivial_assigned::value>::__copy_t(__first, __last, __result);
        }
    };  //*/

    // Copy the range [__first,__last) into [__result, __result + (__last - __first)),
    // return __result + (__last - __first), 第一级泛化
    template<typename _Input_Iter, typename _Output_Iter>
    inline _Output_Iter copy(_Input_Iter __first, _Input_Iter __last, _Output_Iter __result)
    {
        typedef typename iterator_traits<_Input_Iter>::value_type           _Value_type;
        typedef typename iterator_traits<_Input_Iter>::iterator_category    _Category;
        typedef has_trivial_assignment_operator<_Value_type>                is_trivial;
        //const bool __simple = (has_trivial_assignment_operator<_Value_type>::value);
        return __copy_aux<is_trivial::value, _Category>::__copy(__first, __last, __result);
        //return _copy_dispatch_aux<_Input_Iter, _Output_Iter>()(__first, __last, __result);
    }

    // Copy the range [__first,__last) into __result, 第一级特化 for char*
    inline char* copy(char* __first, char* __last, char* __result)
    {
        __builtin_memmove(__result, __first, __last - __first);
        return __result + (__last - __first);
    }

    // Copy the range [__first,__last) into __result, 第一级特化 for wchar_t*
    inline wchar_t* copy(wchar_t* __first, wchar_t* __last, wchar_t* __result)
    {
        __builtin_memmove(__result, __first, sizeof(wchar_t*) * (__last - __first));
        return __result + (__last - __first);
    }


    // 内存处理函数 Copy_backward [__first,__last) into [__result - (__last - __first), __result) 倒序
    
    // __copy_backward 泛化 for non-trivial assignment and Input_Iterator
    template<bool, typename>
    struct __copy_backward_aux
    {
        template<typename _Bidirectional_Iter1, typename _Bidirectional_Iter2>
        static _Bidirectional_Iter2
        __copy_backward(_Bidirectional_Iter1 __first, _Bidirectional_Iter1 __last, 
                        _Bidirectional_Iter2 __result)
        {
            for (; __first != __last ; --__last, --__result) { *__result = *__last; }
            return __result;
        }
    };

    // __copy_backward 特化/强化 for trivial assignment and Random_Access_Iterator
    template<>
    struct __copy_backward_aux<true, random_access_iterator_tag>
    {
        // 原生指针
        template<typename _Tp>
        static _Tp* __copy_backward(const _Tp* __first, const _Tp* __last, _Tp* __result)
        {
            __builtin_memmove(__result - (__last - __first), __first, sizeof(_Tp) * (__last - __first));
            return __result - (__last - __first);
        }

        // 其他 random_access 且 value_type trivial 的迭代器(要求operator&对迭代器有定义)
        template<typename _Random_Access_Iter, typename _Bidirectional_Iter>
        static _Bidirectional_Iter
        __copy_backward(_Random_Access_Iter __first, _Random_Access_Iter __last, 
                        _Bidirectional_Iter __result)
        {
            typedef typename iterator_traits<_Random_Access_Iter>::value_type value_type;
            // 通过*(&Iterator)保证所取地址是迭代器指向的对象的地址, 而非迭代器本身的地址
            __builtin_memmove(*(&(__result - (__last - __first))), *(&__first), 
                                sizeof(value_type) * (__last - __first));
            return __result - (__last - __first);
        }
    };

    // __copy 特化 for non-trivial assignment and Random_Access_Iterator
    template<>
    struct __copy_backward_aux<false, random_access_iterator_tag>
    {
        template<typename _Random_Access_Iter, typename _Bidirectional_Iter>
        static _Bidirectional_Iter
        __copy_backward(_Random_Access_Iter __first, _Random_Access_Iter __last, 
                        _Bidirectional_Iter __result)
        {
            typename iterator_traits<_Random_Access_Iter>::difference_type __n;
            for (__n = __last - __first; __n > 0 ; --__n, --__last, --__result) 
            { *__result = *__last; }
            return __result;
        }
    };

    // Copy the range [__first,__last) into [__result - (__last - __first), __result) 倒序
    // return __result - (__last - __first), 第一级泛化
    template<typename _Bidirectional_Iter1, typename _Bidirectional_Iter2>
    inline _Bidirectional_Iter2 copy_backward(_Bidirectional_Iter1 __first, _Bidirectional_Iter1 __last, 
                                                _Bidirectional_Iter2 __result)
    {
        typedef typename iterator_traits<_Bidirectional_Iter1>::value_type          _Value_type;
        typedef typename iterator_traits<_Bidirectional_Iter1>::iterator_category   _Category;
        typedef has_trivial_assignment_operator<_Value_type>                        is_trivial;
        return __copy_aux<is_trivial::value, _Category>::__copy_backward(__first, __last, __result);
    }

    // Copy the range [__first,__last) into [__result - (__last - __first), __result) 倒序
    inline char* copy_backward(char* __first, char* __last, char* __result)
    {
        __builtin_memmove(__result - (__last - __first), __first, __last - __first);
        return __result - (__last - __first);
    }

    // Copy the range [__first,__last) into [__result - (__last - __first), __result) 倒序
    inline wchar_t* copy_backward(wchar_t* __first, wchar_t* __last, wchar_t* __result)
    {
        __builtin_memmove(__result - (__last - __first), __first, sizeof(wchar_t*) * (__last - __first));
        return __result - (__last - __first);
    }
    
} // namespace my_lib


#endif // MY_ALGOBASE
