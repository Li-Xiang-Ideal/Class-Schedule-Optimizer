/**********************************************************
 * 
 *                Class Arrangement Lib 
 *                     class_arrg.h
 * 
 *               Copyright (C) 2019 李想
 * Released under the GNU General Public License Version 3
 * 
 *********************************************************/

#include<algorithm>
#include<cctype>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<iostream>
#include<string>
#include<vector>

#include"class_data.h"

#ifndef CLASS_ARRG
#define CLASS_ARRG

#ifndef NOT_USING_STD
using std::cout;
using std::endl;
using std::vector;
#endif

/**********************************************************
 * 课程信息结构体_Lesson只是测试beta版, 需要完善 -> 已完善
 * 方案要求_Lesson.course的编号是递增的
 * 课程大纲即用户输入的要选的课程
 * 课程表设为全局变量, 通过传址输出
 * 设置了起始位与终末位, 便于分必修课和选修课单独计算
 *********************************************************/

/**********************************************************
 * 更新日志: 
 * 2019.11.29 使用<vector>存储数据, 提高了代码的可读性
 * 2019.12.03 添加了剪枝函数, 提高了代码效率
 * 2019.12.06 修复了一些已知问题
 * 2019.12.08 整理了剪枝函数, 明显减少了代码量并提高了稳定性
 *********************************************************/

#ifndef CLASS_DATA
//全局课程信息
struct _Lesson
{
    int number; //选课序号
    int course; //课程大纲
	int t_week; //周次
	vector<int>t_time; //课程时间, 不计周次
    vector<int>time; //课程时间
};
//全局课程表
struct _Table
{
    int number; //选课序号
    int List_number; //课程编码
};
#endif

//Arrange类，安排课程
class Arrange
{
    public:
        //构造函数，输入需安排的课程起始位与终末位
        Arrange(int a,int b,int c=196)
        {
            begin_lesson=a;
            end_lesson=b;
            amount_lesson=b-a+1;
            Table_size=c;
            _Table _Table_default_temp;
            _Table_default_temp.number=-1;
            _Table_default_temp.List_number=-1;
            vector<_Table>Table_default_temp(Table_size,_Table_default_temp);
            Table_default.assign(Table_default_temp.begin(),Table_default_temp.end()); //设置默认初始课程表
        }
        //析构函数
        ~Arrange()
        {
            /* 原本写了个用*ptr,new[]和delete[]的方案, 但为了增加代码可读性而用了vector. 毕竟stl它不香吗? */
        }

        //对外接口：输入课程信息
        void setlesson(_Lesson input[])
        {
            int course_index=-1,course_temp=-1;
            _Arrange_Lesson setlesson_temp;
            for (int i=0;i<amount_lesson;i++)
            {
                if (input[begin_lesson+i].time.size()!=0)
                {
                    if(course_temp<input[begin_lesson+i].course) //将输入的course信息连续化，以便于操作
                    {
                        course_temp=input[begin_lesson+i].course;
                        course_index++;
                    }
                    //输入至setlesson_temp
                    int sign1=input[begin_lesson+i].number;
                    setlesson_temp.number=input[begin_lesson+i].number;
                    setlesson_temp.course=course_index;
                    setlesson_temp.time.assign(input[begin_lesson+i].time.begin(),input[begin_lesson+i].time.end());
                    //压栈 (实际上是"压堆")
                    List.push_back(setlesson_temp);
                }
            }
            amount_lesson=List.size();
            amount_course=course_index+1;
            setinvList();
        }

        //对外接口：输入初始课程表
        void settable(const vector<_Table>domain_Table)
        {
            Table_default.assign(domain_Table.begin(),domain_Table.end()); //设置默认初始课程表
        }
        
        //对外接口：输出课程表
        bool gettable(vector<vector<_Table> >&codomain_Table)
        {
            bool if_success;
            laylesson(Table_default);
            if_success=arrangelesson(Table_default);
            codomain_Table.assign(Table.begin(),Table.end());
            return if_success;
        }
        
        //对外接口：测试函数, 输出课程信息
        int getamount(void)
        {
            return Table.size();
        }

        //对外接口：测试函数, 输出课程信息
        void getlesson(void)
        {
            cout<<endl<<"Output Lesson:"<<endl;
            for (int i=0;i<amount_lesson;i++)
            {
                cout<<"Lesson "<<i<<endl;
                cout<<List[i].number<<endl;
                cout<<List[i].course<<endl;
                cout_vector_int(List[i].time);
            }
            cout<<endl;
            for (int i=0;i<amount_course;i++)
            {
                cout<<"Course "<<i<<endl;
                for (int k=0;k<invList[i].size();k++)
                {
                    cout<<invList[i][k]<<endl;
                }
            }
            cout<<endl;
        }

    private:
        int begin_lesson,end_lesson; //需安排的课程起始位与终末位
        int amount_lesson,amount_course; //课程数量, 课程大纲项目数量
        int Table_size; //课程表大小
        //Arrange类内课程信息
        struct _Arrange_Lesson
        {
            int number; //选课序号
            int course; //课程大纲
            vector<int>time; //课程时间
        };
        vector<_Arrange_Lesson>List; //其元素为课程信息
        vector<vector<int> >invList; //其元素为动态数组，数组的个数为课程大纲总数，数组的元素为课程大纲所对应课程的编码
        vector<vector<_Table> >Table; //其元素为动态数组，每个数组是一张课程表，数组的个数为可能的课程表总数，数组的元素为各时间的课程信息
        vector<_Table>Table_default; //一张初始课程表，在构造函数中每节课都被初始化为-1
        vector<vector<int> >Table_layer; //由所有课程在时间表上叠成的层，用于确认哪些课程与哪些课程冲突。其元素为动态数组，每个数组是一节课的时间，数组的元素为这一时间的课程编码
        vector<int>Blocklist; //一个FILO栈，用于标出所有与当前课程冲突的课程，其元素为课程编码
        vector<int>Blocknumber; //一个FILO栈，记录了每次压入Blocklist的课程个数
        
        //内部函数：生成课程大纲对应的课程
        void setinvList(void)
        {
            vector<int>setinvList_temp;
            for (int i=0;i<amount_course;i++)
            {
                setinvList_temp.clear();
                for (int k=0;k<amount_lesson;k++)
                {
                    if (i==List[k].course) //在List中找到与课程大纲中第i门Course对应的课程
                    {
                        setinvList_temp.push_back(k); //将找到的课程压入缓存
                    }
                }
                invList.push_back(setinvList_temp); //将缓存压入invList
            }
        }

        //内部函数：叠放课程至课程表层
        void laylesson(vector<_Table>Table_layer_temp)
        {
            vector<int>temp_lay; //temp_lay是每节时间段的第一层课程，其元素为课程编码
            //初始化课程表层
            for (int i=0;i<Table_size;i++)
            {
                temp_lay.push_back(Table_layer_temp[i].List_number);
                Table_layer.push_back(temp_lay); //在课程表层的每一节时间叠上初始课程
                temp_lay.pop_back();
                //后续增加将非空的初始课程提前压入Blocklist的代码
            }
            //叠放课程
            for (int i=0;i<amount_lesson;i++)
            {
                for (int j=0;j<List[i].time.size();j++)
                {
                    if (Table_layer[List[i].time[j]][0]==-1) Table_layer[List[i].time[j]].pop_back(); //如果一节时间有课，则弹出初始值-1
                    Table_layer[List[i].time[j]].push_back(i); //在时间List[i].time上叠放课程
                }
            }
        }

        //内部函数：动态数组压入元素
        template<typename T>void vector_push(T &Block_list,int &Block_sum_number,const vector<int>&k,int j)
        {
            vector<int>layer_temp;
            Block_sum_number=0;
            for (int i=0;i<List[invList[j][k[j]]].time.size();i++)
            {
                layer_temp.assign(Table_layer[List[invList[j][k[j]]].time[i]].begin(),Table_layer[List[invList[j][k[j]]].time[i]].end());
                delete_from_vector_int(layer_temp,invList[j][k[j]]);
                Blocklist.insert(Blocklist.end(),layer_temp.begin(),layer_temp.end());
                Block_sum_number+=layer_temp.size();
            }
        }

        //内部函数：动态数组弹出元素
        template<typename T>void vector_pop(T &input_vector,int pop_num)
        {
            for (int i=0;i<pop_num;i++)
            {
                input_vector.pop_back(); //从队尾弹出pop_num个元素
            }
        }

        //内部函数：从动态数组中删除指定元素
        void delete_from_vector_int(vector<int>&target_vec,int to_delete)
        {
            vector<int>::iterator it=find(target_vec.begin(),target_vec.end(),to_delete);
            while (it!=target_vec.end())
            {	
                it=target_vec.erase(it);
            	it=find(it,target_vec.end(),to_delete);
            }
        }

        //内部函数：判断数列是否与输入值重复以及下标是否越界
        bool if_continue(const vector<int>&k,int j)
        {
            if (k[j]>=invList[j].size()) return true;
            else return (count(Blocklist.begin(),Blocklist.end(),invList[j][k[j]])!=0);
        }

        //内部函数：回溯
        bool branch(vector<int>&k)
        {
            static bool pushed=false;
            int Block_sum_number=0;

            for (int j=0;j<amount_course;j++)
            {
                if (k[j]==-1||(pushed&&j==amount_course-1)) k[j]++;
                if (j!=0&&!pushed)
                {
                    vector_push(Blocklist,Block_sum_number,k,j-1);
                    Blocknumber.push_back(Block_sum_number);
                }
                while (if_continue(k,j))
                {
                    if (k[j]<invList[j].size()-1)
                    {
                        k[j]++;
                    }
                    else
                    {
                        k[j]=0;
                        j--;
                        pushed=false;
                        if (j<0) return false;
                        k[j]++;
                        vector_pop(Blocklist,Blocknumber.back());
                        Blocknumber.pop_back();
                    }
                }
            }
            pushed=true;
            return true;
        }

        //内部函数：生成课程表
        bool arrangelesson(vector<_Table>Table_temp)
        {
            //vector<int>Table_temp(98,-1);
            int time_cur_course;
            bool failed=true,k_i_failed=false;
            vector<int>k(amount_course,-1);
            vector<_Table>Table_Temp_bak;
            Table_Temp_bak.assign(Table_temp.begin(),Table_temp.end());
            //遍历(invList[0].size()*invList[1].size()*...*invList[amount_course].size())维空间
            while (branch(k))
            {
                k_i_failed=false;
                for (int i=0;i<amount_course;i++) //第i项课程大纲中第k[i]门课程
                {
                    for (int l=0;l<List[invList[i][k[i]]].time.size();l++) //第k门课程中第l节课
                    {
                        time_cur_course=List[invList[i][k[i]]].time[l];
                        if (Table_temp[time_cur_course].number==-1) //第k门课程填写成功
                        {
                            Table_temp[time_cur_course].number=List[invList[i][k[i]]].number;
                            Table_temp[time_cur_course].List_number=invList[i][k[i]];
                        }
                        else
                        {
                            k_i_failed=true;
                            break;
                        }
                    }
                    if (k_i_failed)
                    {
                        Table_temp.assign(Table_Temp_bak.begin(),Table_Temp_bak.end());
                        break;
                    }
                }
                if (!k_i_failed)
                {
                    //将缓存压入Table中
                    Table.push_back(Table_temp);
                    Table_temp.assign(Table_Temp_bak.begin(),Table_Temp_bak.end());
                    failed=false;
                }
            }
            return !failed;
        }
            
        //测试函数：输出vector<int>
        void cout_vector_int(vector<int> output)
        {
            for (int i=0;i<output.size(); i++)
            {
                cout<<output[i]<<" ";
            }
            cout<<endl;
        }
        
};

#endif
