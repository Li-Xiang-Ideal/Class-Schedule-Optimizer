/**********************************************************
 * 
 *                        My Lib 
 *                      my_memory.h
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
 * 本文件包含一个二级空间配置器, 与STL中代码基本相同, 仅做略微
 * 调整. 主要添加了自动调整分配空间大小的功能.
 * 本文件参考了侯捷《STL源码剖析》, 特此说明并致谢
 ***********************************************************/

/************************************************************
 * 更新日志:
 * 2020.05.17 添加了自动调整分配空间大小的功能.
 ***********************************************************/

#ifndef MY_MEMORY
#define MY_MEMORY

#include <new>
#include <cstdlib>
#include <iostream>
#include <bits/c++config.h>
#include <bits/functexcept.h>

#define USE_SUB_ALLOCATOR_STAT

#define max(a,b) ((a)>(b)?(a):(b))

namespace my_lib
{
    //allocator将memory_pool作为静态成员 //或作为成员, 内存池作静态成员?
    class sub_allocator    // Freelist of 8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,128 bytes
    {
    protected:
        //free_list参数
        enum{ __ALIGN = 8 }; //size of obj
        enum{ __MAX_BYTES = 128 }; //max size of node
        enum{ __NFREELISTS = __MAX_BYTES/__ALIGN }; //amount of freelists

        //free_list区块, 大小为8bytes, 8*n bytes的节点由n个区块组成
        union obj
        {
            union obj* next_node;
            char node_data[1];
        };

        //将bytes上调至8的倍数
        static size_t ROUND_UP(size_t bytes) { return (((bytes) + __ALIGN - 1) & ~(__ALIGN - 1)); }

        //将bytes_to_get调至1KB以上
        static size_t CEIL_UP(size_t bytes) { return max(bytes,0x0400); }

        //free_lists
        static obj* volatile free_list[__NFREELISTS];

        //free_list的refill频次等, 用于估计nobjs
        static size_t freelist_freq[__NFREELISTS];
        static size_t freelist_frequency; //总频次
        static float freelist_sigma; //标准差

        //根据bytes选择适合的free_list
        static size_t FREELIST_INDEX(size_t bytes) 
        { return (size_t)(((bytes) + __ALIGN - 1)/__ALIGN - 1); }

        //内存池状态
        static char* begin_free; //内存池起始位
        static char* end_free; //内存池终末位
        static size_t heap_size;  //根据以往申请空间的大小自动调整, 用于确定下一次要申请的空间

        //内存池历史
        static char** begin_pool; //一个数组, 用于记录曾经new过的内存池的begin_free, 以便delete
        static char** end_pool;

        //配置内存池
        static char* obj_alloc(size_t size, int& nobjs)
        {
            char* result;
            size_t total_bytes = size * nobjs;
            size_t bytes_left = end_free - begin_free; //内存池剩余空间
            //设置begin_pool
            if (begin_pool == 0)
            {
                begin_pool = new char*[0x0400];
                end_pool = begin_pool;
            }
            //根据情况进行配置
            if (bytes_left > total_bytes) 
            {
                //直接让refill从begin_free开始取空间
                result = begin_free;
                begin_free += total_bytes;
                return (result) ;
            }
            else if (bytes_left >= size)
            {
                //先根据情况调整nobjs, 再让refill从begin_free开始取空间
                nobjs = bytes_left/size;
                total_bytes = size * nobjs;
                result = begin_free;
                begin_free += total_bytes;
                return (result) ;
            }
            else
            {
                //申请2*total_bytes+n (n自动调整)的空间
                size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
                //如果当前内存池还有剩余空间
                if (bytes_left > 0)
                {
                    //找到合适的free_list将剩余的空间编进去, 从此这一内存池全部交给free_list管理
                    obj* volatile *this_free_list = free_list + FREELIST_INDEX(bytes_left);
                    ((obj*)begin_free)->next_node = *this_free_list;
                    *this_free_list = (obj*)begin_free;
                }

                //获得一块新的内存池
                begin_free = (char*)(::operator new(CEIL_UP(bytes_to_get)));

#ifndef SUB_ALLOCATOR_NO_DELETE

                //记录此内存池
                *end_pool = begin_free; end_pool++;
                
#endif
                //如果new失败了
                if (begin_free == 0)
                {
                    //那就exit(1);(划掉) STL觉得自己还能抢救一下, 模仿之
                    int i;
                    obj* volatile *this_free_list, *p;
                    //试图在更大的free_list中找出一块空余的拨出来用
                    for (int i = size; i <= __MAX_BYTES; i += __ALIGN)
                    {
                        this_free_list = free_list + FREELIST_INDEX(bytes_left);
                        p = *this_free_list;
                        if (p != 0)
                        {
                            *this_free_list = p->next_node;
                            begin_free = (char*)p;
                            end_free = begin_free + i;
                            return (obj_alloc(size, nobjs));
                        }
                        end_free = 0;
                        std::cerr << "out of memory" << std::endl; exit(1);
                    }
                    
                }

                //如果new成功
                heap_size += bytes_to_get;
                end_free = begin_free + CEIL_UP(bytes_to_get);
                return (obj_alloc(size,nobjs));

            }   
        }

        //配置free_list
        static void* refill(size_t _n)
        {

#ifdef USE_SUB_ALLOCATOR_STAT

            //统计
            freelist_freq[FREELIST_INDEX(_n)] += 1;
            freelist_frequency += 1;
            freelist_sigma = (freelist_frequency % 16 == 0)?stdevp():freelist_sigma;
            int delta = ((float)freelist_freq[FREELIST_INDEX(_n)]/freelist_sigma); //*/

#else
            int delta =0;
#endif
            //向内存池申请空间
            int nobjs = 20 + max(delta,0); //保证nobjs>=20
            char* chunk = obj_alloc(_n, nobjs);
            obj* volatile *this_free_list;
            obj *result, *current_obj, *next_obj;

            //如果只获得一个区块, 则直接返回
            if (nobjs == 1) { return (chunk); }

            //若获得多于一个区块, 则同时配置free_list
            this_free_list = free_list + FREELIST_INDEX(_n);
            //将chunk返回给allocate()
            result = (obj*)chunk;
            //从chunk+_n开始配置free_list
            *this_free_list = next_obj = (obj*)(chunk + _n);
            //将free_list的节点串起来
            for (int i = 1; ; i++)
            {
                current_obj = next_obj;
                next_obj = (obj*)((char*)next_obj + _n);
                if (nobjs - 1 == i)
                {
                    //注意调用refill时原free_list已无空闲空间, 故不将current_obj连接到原free_list上
                    current_obj->next_node = 0; 
                    break;
                }else
                {
                    current_obj->next_node = next_obj;
                }
            }
            return (result);
        }

        //计算标准差
        static float stdevp(void)
        {
            float x,y,z; x=0; y=0;
            for (int i = 0; i < __NFREELISTS; i++) { x += freelist_freq[i]/(double)__NFREELISTS; }
            for (int i = 0; i < __NFREELISTS; i++)
            { y += ((float)freelist_freq[i]-x)*((float)freelist_freq[i]-x)/(double)__NFREELISTS; }
            z = 0.5 * y;
            int k = *(int*)&y;
            k = 0x5F375A86 - (k >> 1);
            y = *(float*)&k;
            y = y*(1.5 - z * y * y);
            return (1.0/y);
        }

    public:

#ifdef SUB_ALLOCATOR_NO_DELETE

        //此版本的sub_allocator没有大小限制, 但无法释放内存
        sub_allocator(){}
        ~sub_allocator(){}
        
#else
        //此版本的sub_allocator有大小限制, 可以释放内存
        sub_allocator() 
        {
            begin_pool = new char*[0x0400]; //sub_allocator的最大容量为1MB
            end_pool = begin_pool;
        }
        ~sub_allocator(){}

        //手动释放内存
        void delete_sub_allocator() 
        {
            //之所以要区分自动与手动释放内存, 是为了在手动模式下脱离实例化进行空间配置
            char** iter = begin_pool;
            for (; iter != end_pool; iter++)
            { ::operator delete(iter); }
            delete[] begin_pool;
        }

#endif
        //allocate
        static void* allocate(ptrdiff_t size)
        {
            obj* volatile *this_free_list;
            obj* result;

            //超出sub_allocator范围
            if (size > (size_t)__MAX_BYTES) 
            { std::cerr << "memory error" << std::endl; exit(1); }

            //选择适合的free_list
            this_free_list = free_list + FREELIST_INDEX(size);
            result = *this_free_list;
            //向内存池申请空间
            if (result == 0)
            { return (void*)(refill(ROUND_UP(size))); }
            //调整free_list
            *this_free_list = result->next_node;
            return (result);
        }

        //deallocate
        static void deallocate(void* p, size_t _n)
        {
            obj* q = (obj*)p;
            obj* volatile *this_free_list;

            //超出sub_allocator范围
            if (_n > (size_t)__MAX_BYTES) 
            { std::cerr << "memory error" << std::endl; exit(1); }

            //选择适合的free_list
            this_free_list = free_list + FREELIST_INDEX(_n);
            //收回空间至free_list
            q->next_node = *this_free_list;
            *this_free_list = q;
        }

    };

    //初始化
    char* sub_allocator::begin_free = 0;

    char* sub_allocator::end_free = 0;

    size_t sub_allocator::heap_size = 0;

    char** sub_allocator::begin_pool = 0;

    char** sub_allocator::end_pool = 0;

    size_t sub_allocator::freelist_frequency = 0;

    float sub_allocator::freelist_sigma = 4;

    sub_allocator::obj* volatile sub_allocator::free_list[__NFREELISTS] = 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    size_t sub_allocator::freelist_freq[__NFREELISTS] = 
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


    //封装sub_allocator

    // sub_allocate的实现, 简单封装sub_allocator::allocate()
    template<typename _Tp>
    inline _Tp* _sub_allocate(ptrdiff_t size, _Tp*) 
    { return (_Tp*)sub_allocator::allocate((size_t)size*sizeof(_Tp)); }

    // sub_deallocate的实现, 简单封装sub_allocator::deallocate()
    template <typename _Tp>
    inline void _sub_deallocate(_Tp* __p, size_t _n) 
    { sub_allocator::deallocate(__p, _n); }
    
    
} // namespace my_memory

#undef max

#endif // MY_MEMORY