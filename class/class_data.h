/**********************************************************
 * 
 *                    Class Data Lib 
 *                     class_data.h
 * 
 *             Copyright (C) 2019 李想, 张航
 * Released under the GNU General Public License Version 3
 * 
 *********************************************************/

#include<cctype>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<iostream>
#include<string>
#include<vector>

#ifndef CLASS_DATA
#define CLASS_DATA

#ifndef NOT_USING_STD
using std::cout;
using std::endl;
using std::vector;
#endif

//全局总课表
struct _Curriculum
{
	char num[5];	//选课序号
	char class_name[40];	//课程名称
	char teacher_name[40];	//教师姓名 
	char week[10];	//星期
	char course_duration[10];	//课程节次
	char course_week[20];	//起止周次
	char address[100];	//教室
};

//全局用户输入课程数据 
struct _Syllabus
{
	char num[5];	//选课序号 
	char class_name[40];	//课程名称 
	char teacher_name[40];  //教师姓名 
};

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
