#include <stdio.h>
#include <stdbool.h>
#include <getch.h>
#include <stdlib.h>
#include "student.h"
#include "tools.h"
extern int Id_stu;
//显示学生首界面
void student_first(void)
{
	system("clear");
	puts("    欢迎同学，请选择你要进行的操作");
	puts("**************************************");
	puts("1、查询成绩");
	puts("2、修改密码");
	puts("3、查看信息");
	puts("4、退出系统");
	puts("***************************************");
	printf("请选择功能：\n");
}
//给语文成绩进行冒泡排序并写入chinese_temp数组  返回语文的平均分
double sort_chinese(char *chinese_temp)
{
	int t=0,cnt=0;double ave;							//ave用于接收计算出来的平均数
	for(int o=0;o<Id_stu-2;o++)							//遍历语文成绩数量少1次
	{
		for(int p=0;p<Id_stu-o-2;p++)					//遍历将大的数往后方运送直至未排序数组
		{
			if(chinese_temp[p]>chinese_temp[p+1])
			{
				t=chinese_temp[p];
				chinese_temp[p]=chinese_temp[p+1];
				chinese_temp[p+1]=t;
			}
		}
		cnt+=chinese_temp[Id_stu-o-2];					//将已经排好序的最大值累加
	}
	cnt+=chinese_temp[0];								//将最后一个最小的数加上
	ave=((double)cnt)/(Id_stu-1);						//计算语文平均分
	return ave;
}
//计算语文成绩的排名		遍历语文成绩找到现在登录的学生的排名并返回到返回值中
int ranking_chinese(char *chinese_temp)
{
	int cnt=0;
	for(int i=Id_stu-2;i>=0;i--)
	{
		if(chinese_temp[i]==stu[Serial].chinese_mark){return Id_stu-i-1;}
	}
}
//给数学成绩进行冒泡排序并写入math_temp数组  返回数学的平均分
double sort_math(char *math_temp)
{
	int t=0,cnt=0;double ave;							//ave用于接收计算出来的平均数
	for(int o=0;o<Id_stu-2;o++)							//遍历数学成绩数量少1次
	{
		for(int p=0;p<Id_stu-o-2;p++)					//遍历将大的数往后方运送直至未排序数组
		{
			if(math_temp[p]>math_temp[p+1])
			{
				t=math_temp[p];
				math_temp[p]=math_temp[p+1];
				math_temp[p+1]=t;
			}
		}
		cnt+=math_temp[Id_stu-o-2];					//将已经排好序的最大值累加		
	}
	cnt+=math_temp[0];								//将最后一个最小的数加上
	ave=((double)cnt)/(Id_stu-1);					//计算数学平均分
	return ave;
}
//计算数学成绩的排名     遍历数学成绩找到现在登录的学生的数学排名并返回到返回值中
int ranking_math(char *math_temp)
{
	int cnt=0;
	for(int i=Id_stu-2;i>=0;i--)
	{
		if(math_temp[i]==stu[Serial].math_mark){return Id_stu-i-1;}
	}
}
//给英语成绩进行冒泡排序并写入english_temp数组  返回英文的平均分
double sort_english(char *english_temp)
{
	int t=0,cnt=0;double ave;							//ave用于接收计算出来的平均数
	for(int o=0;o<Id_stu-2;o++)							//遍历英语成绩数量少1次
	{
		for(int p=0;p<Id_stu-o-2;p++)					//遍历将大的数往后方运送直至未排序数组
		{
			if(english_temp[p]>english_temp[p+1])
			{
				t=english_temp[p];
				english_temp[p]=english_temp[p+1];
				english_temp[p+1]=t;
			}
		}
		cnt+=english_temp[Id_stu-o-2];					//将已经排好序的最大值累加		
	}
	cnt+=english_temp[0];								//将最后一个最小的数加上
	ave=((double)cnt)/(Id_stu-1);					    //计算英语平均分
	return ave;
}
//计算英语成绩的排名   遍历英语成绩找到现在登录的学生的英语排名并返回到返回值中
int ranking_english(char *english_temp)
{
	int cnt=0;
	for(int i=Id_stu-2;i>=0;i--)
	{
		if(english_temp[i]==stu[Serial].english_mark)return Id_stu-i-1;
	}
}
//给总成绩进行冒泡排序并写入total_temp数组  返回当前操作的学生的总分排名
int sort_toal(char *total_temp)
{
	int t=0;double ave;
	for(int o=0;o<Id_stu-2;o++)							//遍历人数少1次
	{
		for(int p=0;p<Id_stu-o-2;p++)					//遍历将大的数往后方运送直至未排序数组					
		{
			if(total_temp[p]>total_temp[p+1])
			{
				t=total_temp[p];
				total_temp[p]=total_temp[p+1];
				total_temp[p+1]=t;
			}
		}		
	}
	for(int i=Id_stu-2;i>=0;i--)						//遍历全部人并找出现在操作的学生的总分排名
	{
		if(total_temp[i]==(stu[Serial].chinese_mark+stu[Serial].math_mark+stu[Serial].english_mark))return Id_stu-i-1;
	}
}
//学生模块主函数
void student_time(void)
{
	char *chinese_temp;char *math_temp;char *english_temp;char *total_temp;	//定义四个指针接堆内存，并分别用来接语文数学英语和总分的所有成绩
	char m=0,n=0;
	for(;;)
	{
		system("clear");
		student_first();											//调用学生初始化界面
		m=getch();													//输入1、2、3、4，分别进入查询成绩、修改密码、查询信息和退出系统
		switch(m)
		{
			case 49:												//按1进入查询成绩模块
			{	chinese_temp=calloc(500,1);							//申请四个堆内存用来接语数英各科成绩和总成绩
				math_temp=calloc(500,1);
				english_temp=calloc(500,1);
				total_temp=calloc(500,1);
				for(int o=0;o<Id_stu-1;o++)
				{
					chinese_temp[o]=stu[o].chinese_mark;
					math_temp[o]=stu[o].math_mark;
					english_temp[o]=stu[o].english_mark;
					total_temp[o]=chinese_temp[o]+math_temp[o]+english_temp[o];			
				}
				double c_av=sort_chinese(chinese_temp),m_av=sort_math(math_temp),e_av=sort_english(english_temp),total_av=c_av+m_av+e_av;	//调用排序函数的返回值，即语数英均分，总分均分即语数英均分之和
				int c_rank=ranking_chinese(chinese_temp),m_rank=ranking_math(math_temp),e_rank=ranking_english(english_temp);//语数英排名
				int total_rank=sort_toal(total_temp);//调用总分排序函数接函数返回值，即排名
				system("clear");
				printf("     语文    数学   英语\n");										//打印语数英分的个人总分数，排名，平均分，最高分，最低分
				printf("个人：%-3.2lf    %-3.2lf   %-3.2lf\n",stu[Serial].chinese_mark,stu[Serial].math_mark,stu[Serial].english_mark);
				printf("排名：%-4d   %-4d   %-4d\n",c_rank,m_rank,e_rank);
				printf("平均：%-4.1lf   %-4.1lf   %-4.1lf\n",c_av,m_av,e_av);
				printf("最高：%-3d    %-3d    %-3d\n",chinese_temp[Id_stu-2],math_temp[Id_stu-2],english_temp[Id_stu-2]);
				printf("最低：%-3d    %-3d    %-3d\n",chinese_temp[0],math_temp[0],english_temp[0]);
				printf("所有成绩：\n\n     语文    数学   英语\n");
				for(int i=0;i<Id_stu-1;i++)												//打印从高至低所有的语数英成绩
				{
					printf("      %-3d    %-3d    %-3d\n",chinese_temp[Id_stu-i-2],math_temp[Id_stu-i-2],english_temp[Id_stu-i-2]);
				}
				free(chinese_temp);														//依次释放各个堆内存并将指向堆内存的指针置空
				free(math_temp);
				free(english_temp);
				chinese_temp=NULL;
				math_temp=NULL;
				english_temp=NULL;
				printf("按任意按键退出\n");
				getch();
				break;
			}
			case 50:																    //按2进入更改密码模块
				system("clear");
				pass_change(1,Serial+1);												//更改现在登录的学生的密码
				printf("按任意按键退出\n");
				stdin->_IO_read_ptr=stdin->_IO_read_end;
				getch();
				break;
			case 51:																	 //按3进入查询信息模块，依次打印姓名性别学号
				system("clear");
				printf("姓名：%s 性别：%c 学号：%06d\n",stu[Serial].name,stu[Serial].sex,stu[Serial].id);
				printf("按任意按键退出\n");
				stdin->_IO_read_ptr=stdin->_IO_read_end;
				getch();
				break;
			case 52:return;																//按4退出学生模块
		}
	}
}
