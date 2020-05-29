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
 * 2020.05.04 添加了部分示性类
 * 2020.05.21 添加了部分示性类与功能函数
 ***********************************************************/

#ifndef MY_TYPE_TRAITS
#define MY_TYPE_TRAITS

#define __MY_LIB_TEMPLATE_NULL template<>

#include <cstdlib>
#include <bits/c++config.h>
#include <bits/functexcept.h>

namespace my_lib
{
    //用两个结构体代替true&false, 用于参数类型推断
    struct __true_type { static const bool value = true; typedef __true_type type; };
    struct __false_type { static const bool value = false; typedef __false_type type; };

    //主模板泛化non-trivial
    template<typename _Tp>
    struct type_traits
    {
        typedef __true_type     this_dummy_member_must_be_first;
        typedef __false_type    _has_trivial_default_constructor;
        typedef __false_type    _has_trivial_copy_constructor;
        typedef __false_type    _has_trivial_assignment_operator;
        typedef __false_type    _has_trivial_destructor;
        typedef __false_type    _is_POD_type;
        typedef __false_type    _is_integer;
        typedef __false_type    _is_floating;
        typedef __false_type    _is_pointer;
    };

    //对常见的trivial数据类型偏特化
    __MY_LIB_TEMPLATE_NULL struct type_traits<bool>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
        typedef __true_type     _is_integer;
        typedef __false_type    _is_floating;
        typedef __false_type    _is_pointer;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<char>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
        typedef __true_type     _is_integer;
        typedef __false_type    _is_floating;
        typedef __false_type    _is_pointer;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<signed char>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
        typedef __true_type     _is_integer;
        typedef __false_type    _is_floating;
        typedef __false_type    _is_pointer;
    };
    
    __MY_LIB_TEMPLATE_NULL struct type_traits<unsigned char>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
        typedef __true_type     _is_integer;
        typedef __false_type    _is_floating;
        typedef __false_type    _is_pointer;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<wchar_t>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
        typedef __true_type     _is_integer;
        typedef __false_type    _is_floating;
        typedef __false_type    _is_pointer;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<char16_t>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
        typedef __true_type     _is_integer;
        typedef __false_type    _is_floating;
        typedef __false_type    _is_pointer;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<char32_t>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
        typedef __true_type     _is_integer;
        typedef __false_type    _is_floating;
        typedef __false_type    _is_pointer;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<short>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
        typedef __true_type     _is_integer;
        typedef __false_type    _is_floating;
        typedef __false_type    _is_pointer;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<unsigned short>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
        typedef __true_type     _is_integer;
        typedef __false_type    _is_floating;
        typedef __false_type    _is_pointer;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<int>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
        typedef __true_type     _is_integer;
        typedef __false_type    _is_floating;
        typedef __false_type    _is_pointer;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<unsigned int>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
        typedef __true_type     _is_integer;
        typedef __false_type    _is_floating;
        typedef __false_type    _is_pointer;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<long>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
        typedef __true_type     _is_integer;
        typedef __false_type    _is_floating;
        typedef __false_type    _is_pointer;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<unsigned long>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
        typedef __true_type     _is_integer;
        typedef __false_type    _is_floating;
        typedef __false_type    _is_pointer;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<long long>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
        typedef __true_type     _is_integer;
        typedef __false_type    _is_floating;
        typedef __false_type    _is_pointer;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<unsigned long long>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
        typedef __true_type     _is_integer;
        typedef __false_type    _is_floating;
        typedef __false_type    _is_pointer;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<float>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
        typedef __false_type    _is_integer;
        typedef __true_type     _is_floating;
        typedef __false_type    _is_pointer;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<double>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
        typedef __false_type    _is_integer;
        typedef __true_type     _is_floating;
        typedef __false_type    _is_pointer;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<long double>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
        typedef __false_type    _is_integer;
        typedef __true_type     _is_floating;
        typedef __false_type    _is_pointer;
    };

    /*在已经对size_t和ptrdiff_t的原型进行过处理后, 不必再对size_t和ptrdiff_t重复处理
    __MY_LIB_TEMPLATE_NULL struct type_traits<size_t>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
        typedef __true_type     _is_integer;
        typedef __false_type    _is_floating;
        typedef __false_type    _is_pointer;
    };

    __MY_LIB_TEMPLATE_NULL struct type_traits<ptrdiff_t>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
        typedef __true_type     _is_integer;
        typedef __false_type    _is_floating;
        typedef __false_type    _is_pointer;
    };//*/

    //原生指针本身自然也是trivial的, 注意pointer特指原生指针, iterator不在其列
    template<typename _Tp>
    struct type_traits<_Tp*>
    {
        typedef __true_type     _has_trivial_default_constructor;
        typedef __true_type     _has_trivial_copy_constructor;
        typedef __true_type     _has_trivial_assignment_operator;
        typedef __true_type     _has_trivial_destructor;
        typedef __true_type     _is_POD_type;
        typedef __false_type    _is_integer;
        typedef __false_type    _is_floating;
        typedef __true_type     _is_pointer;
    };

    
    //以下一组示性类均为对typename(而非具体对象)给出traits对应的typename(而非临时对象)

    //type_traits<_Tp>::_has_trivial_default_constructor
    template<typename _Tp> struct has_trivial_default_constructor 
    : public type_traits<_Tp>::_has_trivial_default_constructor { };

    //type_traits<_Tp>::_has_trivial_copy_constructor
    template<typename _Tp> 
    struct has_trivial_copy_constructor 
    : public type_traits<_Tp>::_has_trivial_copy_constructor { };

    //type_traits<_Tp>::_has_trivial_assignment_operator
    template<typename _Tp> 
    struct has_trivial_assignment_operator 
    : public type_traits<_Tp>::_has_trivial_assignment_operator { };

    //type_traits<_Tp>::_has_trivial_destructor
    template<typename _Tp> 
    struct has_trivial_destructor 
    : public type_traits<_Tp>::_has_trivial_destructor { };

    //type_traits<_Tp>::_is_POD_type
    template<typename _Tp> 
    struct is_POD_type 
    : public type_traits<_Tp>::_is_POD_type { };

    //type_traits<_Tp>::_is_integer
    template<typename _Tp> 
    struct is_integer
    : public type_traits<_Tp>::_is_integer { };

    //type_traits<_Tp>::_is_floating
    template<typename _Tp> 
    struct is_floating
    : public type_traits<_Tp>::_is_floating { };

    //type_traits<_Tp>::_is_pointer
    template<typename _Tp> 
    struct is_pointer
    : public type_traits<_Tp>::_is_pointer { };


    //以下一组示性函数均为对具体对象(而非typename)返回一临时对象(而非typename)

    //inline函数获得当前object的_has_trivial_default_constructor(作为object而非typename)
    template<typename _Tp>
    inline typename type_traits<_Tp>::_has_trivial_default_constructor
    _has_trivial_default_constructor(const _Tp&)
    {
        //tips:如果发现class<type>::object中的object没有被自动语法高亮, 则需要在typedef后加上typename
        typedef typename type_traits<_Tp>::_has_trivial_default_constructor 
            _has_trivial_default_constructor;
        return _has_trivial_default_constructor(); //括号用于生成临时对象
    }

    //获得当前object的_has_trivial_copy_constructor(作为object而非typename)
    template<typename _Tp>
    inline typename type_traits<_Tp>::_has_trivial_copy_constructor
    _has_trivial_copy_constructor(const _Tp&)
    {
        typedef typename type_traits<_Tp>::_has_trivial_copy_constructor 
            _has_trivial_copy_constructor;
        return _has_trivial_copy_constructor();
    }

    //获得当前object的_has_trivial_assignment_operator(作为object而非typename)
    template<typename _Tp>
    inline typename type_traits<_Tp>::_has_trivial_assignment_operator
    _has_trivial_assignment_operator(const _Tp&)
    {
        typedef typename type_traits<_Tp>::_has_trivial_assignment_operator 
            _has_trivial_assignment_operator;
        return _has_trivial_assignment_operator();
    }

    //获得当前object的_has_trivial_destructor(作为object而非typename)
    template<typename _Tp>
    inline typename type_traits<_Tp>::_has_trivial_destructor
    _has_trivial_destructor(const _Tp&)
    {
        typedef typename type_traits<_Tp>::_has_trivial_destructor
            _has_trivial_destructor;
        return _has_trivial_destructor();
    }

    //获得当前object的_is_POD_type(作为object而非typename)
    template<typename _Tp>
    inline typename type_traits<_Tp>::_is_POD_type
    _is_POD_type(const _Tp&)
    {
        typedef typename type_traits<_Tp>::_is_POD_type _is_POD_type;
        return _is_POD_type();
    }


    //以下一组示性函数均为对具体对象(而非typename)返回具体值(bool)

    //inline函数获得当前object的_has_trivial_default_constructor值(bool)
    template<typename _Tp>
    inline bool ___has_trivial_default_constructor(const _Tp&)
    {
        typedef typename type_traits<_Tp>::_has_trivial_default_constructor 
            _has_trivial_default_constructor;
        return _has_trivial_default_constructor().value;
    }

    //获得当前object的_has_trivial_copy_constructor值(bool)
    template<typename _Tp>
    inline bool ___has_trivial_copy_constructor(const _Tp&)
    {
        typedef typename type_traits<_Tp>::_has_trivial_copy_constructor 
            _has_trivial_copy_constructor;
        return _has_trivial_copy_constructor().value;
    }

    //获得当前object的_has_trivial_assignment_operator值(bool)
    template<typename _Tp>
    inline bool ___has_trivial_assignment_operator(const _Tp&)
    {
        typedef typename type_traits<_Tp>::_has_trivial_assignment_operator 
            _has_trivial_assignment_operator;
        return _has_trivial_assignment_operator().value;
    }

    //获得当前object的_has_trivial_destructor值(bool)
    template<typename _Tp>
    inline bool ___has_trivial_destructor(const _Tp&)
    {
        typedef typename type_traits<_Tp>::_has_trivial_destructor
            _has_trivial_destructor;
        return _has_trivial_destructor().value;
    }

    //获得当前object的_is_POD_type值(bool)
    template<typename _Tp>
    inline bool ___is_POD_type(const _Tp&)
    {
        typedef typename type_traits<_Tp>::_is_POD_type _is_POD_type;
        return _is_POD_type().value;
    }




    //以下为integral_constant结构体模板, STL中所有traits的基石

    //这里不打算将integral_constant用作基类, 而将其简单地视为获得type的工具
    template<typename _Tp, _Tp __val>
    struct integral_constant
    {
        static constexpr _Tp                    value = __val;
        typedef _Tp                             value_type;
        typedef integral_constant<_Tp, __val>   type;
        constexpr operator value_type() const { return value; }
#if __cplusplus > 201103L
#define __cpp_lib_integral_constant_callable 201304
        constexpr value_type operator()() const { return value; }
#endif
    };
    
    template<typename _Tp, _Tp __v>
    constexpr _Tp integral_constant<_Tp, __v>::value;

    /* 还是值得让大家看一看STL是如何使用integral_constant的
    typedef integral_constant<bool, true>       true_type;

    typedef integral_constant<bool, false>      false_type; //*/




    //以下为一些其他功能类, 单列于此

    //移除类型名的引用
    template<typename _Tp>
    struct remove_reference
    { typedef _Tp   type; };

    template<typename _Tp>
    struct remove_reference<_Tp&>
    { typedef _Tp   type; };

    template<typename _Tp>
    struct remove_reference<_Tp&&>
    { typedef _Tp   type; };

    //移除const标记
    template<typename _Tp>
    struct remove_const
    { typedef _Tp   type; };

    template<typename _Tp>
    struct remove_const<_Tp const>
    { typedef _Tp   type; };
  
    //移除volatile标记
    template<typename _Tp>
    struct remove_volatile
    { typedef _Tp   type; };

    template<typename _Tp>
    struct remove_volatile<_Tp volatile>
    { typedef _Tp   type; };
  
    //移除const和volatile标记
    template<typename _Tp>
    struct remove_cv
    {
        typedef typename
        remove_const<typename remove_volatile<_Tp>::type>::type     type;
    };
  
    //添加const标记
    template<typename _Tp>
    struct add_const
    { typedef _Tp const     type; };
   
    //添加volatile标记
    template<typename _Tp>
    struct add_volatile
    { typedef _Tp volatile  type; };
  
    //添加const和volatile标记
    template<typename _Tp>
    struct add_cv
    {
        typedef typename
        add_const<typename add_volatile<_Tp>::type>::type           type;
    };




    //以下为一些其他示性类, 单列于此

    //比较类型是否相同
    template<typename, typename>
    struct are_same_type
    {
        enum { value = false };
        typedef __false_type    type;
    };
    
    template<typename _Tp>
    struct are_same_type<_Tp, _Tp>
    {
        enum { value = true };
        typedef __true_type     type;
    };

    //判断是否为void类型
    template<typename _Tp>
    struct is_void
    {
        enum { value = false };
        typedef __false_type    type;
    };

    template<>
    struct is_void<void>
    {
        enum { value = true };
        typedef __true_type     type;
    };

    //判断是否为左值引用
    template<typename>
    struct is_lvalue_reference
    {
        enum { value = false };
        typedef __false_type    type;
    };

    template<typename _Tp>
    struct is_lvalue_reference<_Tp&>
    {
        enum { value = true };
        typedef __true_type     type;
    };

    //判断是否为右值引用
    template<typename>
    struct is_rvalue_reference
    {
        enum { value = false };
        typedef __false_type    type;
    };

    template<typename _Tp>
    struct is_rvalue_reference<_Tp&&>
    {
        enum { value = true };
        typedef __true_type     type;
    };

    //判断是否为原生指针(含_Tp* const, _Tp* volatile)
    template<typename _Tp>
    struct be_pointer
    : public is_pointer<typename remove_cv<_Tp>::type>
    { };




    //以下为条件判断与逻辑运算示性类

    //定义 conditional::type = _Cond ? _Iftrue : _Iffalse
    template<bool _Cond, typename _Iftrue, typename _Iffalse>
    struct conditional
    { typedef _Iftrue type; };

    template<typename _Iftrue, typename _Iffalse>
    struct conditional<false, _Iftrue, _Iffalse>
    { typedef _Iffalse type; };

    //逻辑运算:与
    template<typename...>
    struct __and_;

    //逻辑运算:与  _B1 为示性类
    template<typename _B1>
    struct __and_<_B1>
    : public _B1
    { };

    //逻辑运算:与  _B1, _B2 均为示性类
    template<typename _B1, typename _B2>
    struct __and_<_B1, _B2>
    : public conditional<_B1::value, _B2, _B1>::type
    { };

    //逻辑运算:与  _B1, _B2, ..., _Bn 均为示性类
    template<typename _B1, typename _B2, typename _B3, typename... _Bn>
    struct __and_<_B1, _B2, _B3, _Bn...>
    : public conditional<_B1::value, __and_<_B2, _B3, _Bn...>, _B1>::type
    { };

    //逻辑运算:或
    template<typename...>
    struct __or_;

    //逻辑运算:或  _B1 为示性类
    template<typename _B1>
    struct __or_<_B1>
    : public _B1
    { };

    //逻辑运算:或  _B1, _B2 均为示性类
    template<typename _B1, typename _B2>
    struct __or_<_B1, _B2>
    : public conditional<_B1::value, _B1, _B2>::type
    { };

    //逻辑运算:或  _B1, _B2, ..., _Bn 均为示性类
    template<typename _B1, typename _B2, typename _B3, typename... _Bn>
    struct __or_<_B1, _B2, _B3, _Bn...>
    : public conditional<_B1::value, _B1, __or_<_B2, _B3, _Bn...>>::type
    { };

    //逻辑运算:非
    template<typename _B1>
    struct __not_
    : public conditional<_B1::value, __false_type, __true_type>::type
    { };

    //定义 enable_if::type 当且仅当 bool = true
    template<bool, typename _Tp = void>
    struct enable_if { };

    template<typename _Tp>
    struct enable_if<true, _Tp>
    { typedef _Tp type; };

    //定义 _Require 当 _Cond 均为真时有定义
    template<typename... _Cond>
    using _Require = typename enable_if<__and_<_Cond...>::value>::type;





} // namespace my_lib


#endif
