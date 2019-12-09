/**********************************************************
 * 
 *                  Class Main Program
 *                    class_main.cpp
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

using namespace std;

#include"lib\class_data.h"
#include"lib\class_arrg.h"
#include"lib\class_io.h"

#define N 2201   //八里台2201 津南1431 

/*************************
		主函数main 
*************************/
int main()
{
	void input_Curriculum(struct _Curriculum cl[]);	//原始数据录入函数 
	void input_Syllabus(struct _Syllabus cu[]);	//选课数据录入函数 
	void delivery(struct _Lesson de[],struct _Curriculum cl[],struct _Syllabus cu[]);	//数据传递函数
	void output(struct _Curriculum cl[]);	//课表输出函数 
	struct _Curriculum cl[N],*p_cl=cl;
	struct _Syllabus cu[30],*p_cu=cu;
	struct _Lesson de[50],*p_Lesson=de;
	input_Curriculum(p_cl);
	input_Syllabus(p_cu);
	delivery(p_Lesson,p_cl,p_cu);
	Arrange Arrange1(0,50-1,196); //根据我们的经验, 与用户教学大纲有关的课程数一般不高于50
	Arrange1.setlesson(de);
	cout<<"Now arranging, please wait..."<<endl<<endl;
	bool success = Arrange1.gettable(class_Table);
	cout<<"You get "<<Arrange1.getamount()<<" table(s)"<<endl<<endl;
	if (success) cout<<"Success !"<<endl;
	else cout<<"Failed !"<<endl;
	output(p_cl);
	system("pause");
	return 0;
}
