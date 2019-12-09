/**********************************************************
 * 
 *                Class Input Output Lib 
 *                      class_io.h
 * 
 *             Copyright (C) 2019 李想, 张航
 * Released under the GNU General Public License Version 3
 * 
 *********************************************************/

#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<vector>
#include<iomanip>

#include"class_data.h"
#include"class_arrg.h"

#ifndef CLASS_INPUT
#define CLASS_INPUT

#ifndef NOT_USING_STD
using namespace std; 
#endif

/**********************************************************
 * 数据类型全部转移到class_data.h中, 类型名称全部改为_开头
 * 函数定义全部转移到class_io.h中, 主函数放在class_main.cpp
 * 考虑到全程没有引入其他命名空间, 直接using namespace std;
 * 读取文件时, 默认文件与主程序位于同一目录下, 添加了错误暂停
 * 输入文件分别改名为Curriculum.txt和Syllabus.txt
 *********************************************************/

#define N 2201 //八里台2201 津南1431 

int flat;

int number_lesson;

vector<vector<_Table> >class_Table;

#ifndef CLASS_DATA

/************************
		结构体部分 
************************/ 
//课程原始数据 
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
//用户输入课程数据 
struct _Syllabus
{
	char num[5];	//选课序号 
	char class_name[40];	//课程名称 
	char teacher_name[40];  //教师姓名 
};
#endif

/************************** 
下方为各部分函数的代码部分 
**************************/
//原始数据录入函数 
void input_Curriculum(struct _Curriculum cl[])
{
	FILE *fp;
	if((fp=fopen("Curriculum_balitai.txt","r"))==NULL)
	{
		printf("Can't open file: Curriculum_balitai.txt!\n");
		system("pause");
		exit(1);
 	}
	for(int i=0;i<N;i++)
	{
		fscanf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t%s\n",cl[i].num,cl[i].class_name,cl[i].teacher_name,cl[i].week,cl[i].course_duration,cl[i].course_week,cl[i].address);
	}
	fclose(fp);
}
//选课数据录入函数 
void input_Syllabus(struct _Syllabus cu[])
{
	FILE *fp_input;
	if((fp_input=fopen("Syllabus.txt","r"))==NULL)
	{
		printf("Can't open file: Syllabus.txt!\n");
		system("pause");
		exit(1);
	}
	for(int i=0;i<30;i++)
	{
		fscanf(fp_input,"%s%s%s",cu[i].num,cu[i].class_name,cu[i].teacher_name);
		if(i>0)
		{
			if(strcmp(cu[i].num,"break")==0)
			{
				flat=i;															//flat为数据终止判定 
				break;
			}
		}
	}
	/*
		for(int i=0;i<flat;i++)
		{
			printf("%s\t%s\t%s\n",cu[i].num,cu[i].class_name,cu[i].teacher_name);
		}
	*/
	fclose(fp_input);
}
//数据传递函数 
void delivery(struct _Lesson de[],struct _Curriculum cl[],struct _Syllabus cu[])
{
	int q=0;
	char del_temp[40];
	del_temp[0]='2',del_temp[1]='3',del_temp[2]='3',del_temp[3]='3';
	for(int i=0,k=0;i<flat;i++)
	{
	 	for(int j=0;j<N;j++)
	 	{
	 		if((strcmp(cu[i].num,cl[j].num)==0||strcmp(cu[i].class_name,cl[j].class_name)==0)&&(strcmp(cu[i].teacher_name,cl[j].teacher_name)==0||(cu[i].teacher_name[0]=='N'&&cu[i].teacher_name[1]=='U')))
	 		{
				int F1(char str[]);
	 			int F2(char str[]);
	 			int F3(char str[]);
	 			int F4(char str[]);
	 			int F5(char str[]);
	 			int F6(char str[]);
	 			int term;
				de[q].number=F1(cl[j].num);
	 			if(strcmp(cl[j].class_name,del_temp)==0)
				{
	 				de[q].course=k-1;
				}
				else
				{
					de[q].course=k;
					k++;
				}
				strcpy(del_temp,cl[j].class_name);
				if(F5(cl[j].course_week)<=12&&F5(cl[j].course_week)>=8)
				{	
					for(term=F3(cl[j].course_duration);term<=F4(cl[j].course_duration);term++)
					{
						de[q].time.push_back((F2(cl[j].week)-1)*14+term+98-1);
					}
				}
				else
				if(F6(cl[j].course_week)<=12&&F6(cl[j].course_week)>=8)
				{
					for(term=F3(cl[j].course_duration);term<=F4(cl[j].course_duration);term++)
					{
						de[q].time.push_back((F2(cl[j].week)-1)*14+term-1);
					}	
				}
				else
				{
					for(term=F3(cl[j].course_duration);term<=F4(cl[j].course_duration);term++)
					{
						de[q].time.push_back((F2(cl[j].week)-1)*14+term-1);
					}
					for(term=F3(cl[j].course_duration);term<=F4(cl[j].course_duration);term++)
					{
						de[q].time.push_back((F2(cl[j].week)-1)*14+term+98-1);
					}		
				}
				if (strcmp(cl[j].num,j>0?cl[j+1].num:"2333")!=0) //(j>0?cl[j+1].num:"")
				{
					q++;
				}
				
			}
		}
	}
} 
//课表输出函数 
void output(struct _Curriculum cl[])
{
	int F1(char str[]);
	int flat;
	cout<<endl<<"Output Table:"<<endl;
	for (int i = 0; i < class_Table.size(); i++)
	{
		cout<<endl<<"第"<<(i+1)<<"张课表"<<endl<<endl;
		cout<<"上半学期: "<<endl;
		cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
   		cout << setiosflags(ios::left) << setw(14) << "节次&星期" << resetiosflags(ios::left)
       		 << setiosflags(ios::right) << setw(7) << "星期一" << setw(27) << "星期二" << setw(27) << "星期三"
			 << setw(27) << "星期四"<< setw(27) << "星期五"<< setw(27) << "星期六"<< setw(27) 
			 << "星期日"<< resetiosflags(ios::right) << endl;
	    cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
		for (int j = 0; j < 14; j++)
		{
			cout << setiosflags(ios::left) << setw((-(j+1)/10+1)*3) << "第" << j+1 << "节课" << resetiosflags(ios::left)<< setiosflags(ios::right) << setw(4) ; 
			for (int k=0;k<7;k++)
			{
				cout<< '|' ;
				if(class_Table[i][j+14*k].number==-1)cout<<' ';else
				if(class_Table[i][j+14*k].number==class_Table[i][j+14*k-1].number)
				{
					for(int x=0;x<N;x++)
					{
						if(class_Table[i][j+14*k].number==F1(cl[x].num))
						{
							cout<<cl[x].class_name;
							flat=x;
							break;
						}
					}
				}else
				cout<<class_Table[i][j+14*k].number;
				if(k!=6)
				{
					if(class_Table[i][j+14*k].number==-1)cout<<setw(26);else
					if(class_Table[i][j+14*k].number==class_Table[i][j+14*k-1].number)
					{
						int y;
						for(y=0;cl[flat].class_name[y]!='\0';y++);
						cout<< setw(27-y) ;
					}
					else
					{
						if(class_Table[i][j+14*k].number>999)
						cout<< setw(23) ;else
						if(class_Table[i][j+14*k].number>99)
						cout<< setw(24) ;else
						if(class_Table[i][j+14*k].number>9)
						cout<< setw(25) ;else
						cout<< setw(26) ;
					}
				}
			}
			cout<< resetiosflags(ios::right) << endl;
			cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"<<endl ;
		}
		cout<<endl<<"下半学期: "<<endl;
		cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
   		cout << setiosflags(ios::left) << setw(14) << "节次&星期" << resetiosflags(ios::left)
       		 << setiosflags(ios::right) << setw(9) << "星期一" << setw(27) << "星期二" << setw(27) << "星期三"
			 << setw(27) << "星期四"<< setw(27) << "星期五"<< setw(27) << "星期六"<< setw(27) 
			 << "星期日"<< resetiosflags(ios::right) << endl;
	    cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
		for (int j = 98; j < 112; j++)
		{
			cout << setiosflags(ios::left) << setw((-(j-97)/10+1)*3) << "第" << j-98+1 << "节课" << resetiosflags(ios::left)<< setiosflags(ios::right) << setw(4) ; 
			for (int k=0;k<7;k++)
			{
				cout<< '|' ;
				if(class_Table[i][j+14*k].number==-1)cout<<' ';else
				if(class_Table[i][j+14*k].number==class_Table[i][j+14*k-1].number)
				{
					for(int x=0;x<N;x++)
					{
						if(class_Table[i][j+14*k].number==F1(cl[x].num))
						{
							cout<<cl[x].class_name;
							flat=x;
							break;
						}
					}
				}else
				cout<<class_Table[i][j+14*k].number;
				if(k!=6)
				{
					if(class_Table[i][j+14*k].number==-1)cout<<setw(26);else
					if(class_Table[i][j+14*k].number==class_Table[i][j+14*k-1].number)
					{
						int y;
						for(y=0;cl[flat].class_name[y]!='\0';y++);
						cout<< setw(27-y) ;
					}
					else
					{
						if(class_Table[i][j+14*k].number>999)
						cout<< setw(23) ;else
						if(class_Table[i][j+14*k].number>99)
						cout<< setw(24) ;else
						if(class_Table[i][j+14*k].number>9)
						cout<< setw(25) ;else
						cout<< setw(26) ;
					}
				}
			}
			cout<< resetiosflags(ios::right) << endl;
			cout << "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"<<endl ;
		}
		cout<<endl;
	}
} 
//字符串转换整形数据函数 
int F1(char str[])
{
	int i=0,sum=0;
	for(;i<=3;i++)
	{
		sum=sum*10+(str[i]-48);	
	}
	return(sum);
} 
int F2(char str[])
{
	return(str[0]-48);
}
int F3(char str[])
{
	int sum=0,i=0;
	for(;;i++)
	{
		if(str[i]=='/')
		break;
		sum=sum*10+(str[i]-48);
	}
	return(sum);
}
int F4(char str[])
{
	int sum=0,i=0;
	for(int j=0;;j++)
	{
		if(str[j]=='/')
		{
			i=j+1;
			break;
		}
	}
	for(i;;i++)
	{
		if(str[i]<'0'||str[i]>'9')
		break;
		sum=sum*10+(str[i]-48);
	}
	return(sum);
}
int F5(char str[])
{
	int sum=0,i=0;
	for(;;i++)
	{
		if(str[i]=='-')
		break;
		sum=sum*10+(str[i]-48);
	}
	return(sum);
}
int F6(char str[])
{
	int sum=0,i=0;
	for(int j=0;;j++)
	{
		if(str[j]=='-')
		{
			i=j+1;
			break;
		}
	}
	for(i;;i++)
	{
		if(str[i]<'0'||str[i]>'9')
		break;
		sum=sum*10+(str[i]-48);
	}
	return(sum);
}

#endif
