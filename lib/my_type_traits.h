/**********************************************************
 * 
 *                        My Lib 
 *                   my_type_traits.h
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
 * 本文件包含常用的类相关示性函数. 本文件使用了traits技巧并进行
 * 了相对详尽的注释以备温习或查阅. 部分代码相比STL进行了少量调整
 * 本文件参考了侯捷《STL源码剖析》, 特此说明并致谢
 ***********************************************************/

/************************************************************
 * 更新日志:
 * 2020.05.02 添加了部分功能函数
 ***********************************************************/

#ifndef MY_TYPE_TRAITS
#define MY_TYPE_TRAITS

#define __MY_LIB_TEMPLATE_NULL template<>

#include <bits/c++config.h>
#include <bits/postypes.h>
#include <iostream>

namespace my_lib
{
    //用两个结构体代替true&false, 用于参数类型推断
    struct __true_type { };
    struct __false_type { };

    //主模板non-trivial
    template<typename _Tp>
    struct type_traits
    {
        typedef __true_type     this_dummy_member_must_be_first;
        typedef __false_type    _has_trivial_default_constructor;
        typedef __false_type    _has_trivial_copy_constructor;
        typedef __false_type    _has_trivial_assignment_operator;
        typedef __false_type    _has_trivial_destructor;
        typedef __false_type    _is_POD_type;
    };

    //对常见的trivial数据类型偏特化
    __MY_LIB_TEMPLATE_NULL struct type_traits<char>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<signed char>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
    };
    
    __MY_LIB_TEMPLATE_NULL struct type_traits<unsigned char>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<short>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<unsigned short>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<int>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<unsigned int>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<long>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<unsigned long>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<long long>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<unsigned long long>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<float>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<double>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<long double>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<wchar_t>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
    };

    /*在已经对size_t和ptrdiff_t的原型进行过处理后, 不必再对size_t和ptrdiff_t重复处理
    __MY_LIB_TEMPLATE_NULL struct type_traits<size_t>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<ptrdiff_t>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
    };*/

    //原生指针本身自然也是trivial的
    template<typename _Tp>
    struct type_traits<_Tp*>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
    };

    //inline函数获得_has_trivial_default_constructor
    template<typename _Tp>
    inline typename type_traits<_Tp>::_has_trivial_default_constructor
    _has_trivial_default_constructor(const _Tp&)
    {
        //tips:如果发现class<type>::object中的object没有被自动语法高亮, 则需要在typedef后加上typename
        typedef typename type_traits<_Tp>::_has_trivial_default_constructor 
            _has_trivial_default_constructor;
        return _has_trivial_default_constructor(); //括号用于生成临时对象
    }

    //_has_trivial_copy_constructor
    template<typename _Tp>
    inline typename type_traits<_Tp>::_has_trivial_copy_constructor
    _has_trivial_copy_constructor(const _Tp&)
    {
        typedef typename type_traits<_Tp>::_has_trivial_copy_constructor 
            _has_trivial_copy_constructor;
        return _has_trivial_copy_constructor();
    }

    //_has_trivial_assignment_operator
    template<typename _Tp>
    inline typename type_traits<_Tp>::_has_trivial_assignment_operator
    _has_trivial_assignment_operator(const _Tp&)
    {
        typedef typename type_traits<_Tp>::_has_trivial_assignment_operator 
            _has_trivial_assignment_operator;
        return _has_trivial_assignment_operator();
    }

    //_has_trivial_destructor
    template<typename _Tp>
    inline typename type_traits<_Tp>::_has_trivial_destructor
    _has_trivial_destructor(const _Tp&)
    {
        typedef typename type_traits<_Tp>::_has_trivial_destructor
            _has_trivial_destructor;
        return _has_trivial_destructor();
    }

    //_is_POD_type
    template<typename _Tp>
    inline typename type_traits<_Tp>::_is_POD_type
    _is_POD_type(const _Tp&)
    {
        typedef typename type_traits<_Tp>::_is_POD_type _is_POD_type;
        return _is_POD_type();
    }
    
} // namespace my_lib


#endif
