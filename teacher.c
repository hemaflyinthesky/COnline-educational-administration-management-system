#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getch.h>
#include "tools.h"
#include "teacher.h"
extern int Id_stu;   
extern int Serial;	//全局变量
#define STU (Id_stu-1)  //定义宏存储学生总人数
void teacher_time(void)
{
	for(;;)
	{
		switch(show_tea())//初始化教师身份后界面
		{
			case 49:add();break;
			case 50:del();break;
			case 51:find();break;
			case 52:mod();break;
			case 53:input_grade();break;
			case 54:reset_password();break;
			case 55:show_in();break;
			case 56:show_out();break;
			case 57:return;
		}
	}
}
//初始化教师界面
int show_tea(void)
{
	system("clear");
	puts("    欢迎老师，请选择你要进行的操作    ");
	puts("**************************************");
	puts("1.添加学生");
	puts("2.删除学生");
	puts("3.查找学生");
	puts("4.修改学生信息");
	puts("5.录入学生成绩");
	puts("6.重置学生密码");
	puts("7.显示在校学生");
	puts("8.显示离校学生");
	puts("9.退出");
	puts("**************************************");
	char cmd=getch();
	return cmd; //返回按键值进行操作
}

//添加学生
void add(void)
{
	printf("请选择添加方式，按1单个添加，按2批量导入,按其他键返回\n");
	char cmd=getch();
	if(cmd==49)
	{
		printf("请输入新添加学生的姓名、性别，其他信息会自动生成\n");
    	stdin->_IO_read_ptr=stdin->_IO_read_end;//清空输入缓冲区
		limit_import(stu[STU].name,30);   //限制输入长度
		if(strcmp(stu[STU].name,"\0")==0)  //若长度超过限定长度显示添加失败并且不添加
		{
			strcmp(stu[STU].name,"\0");
			msg_show("添加失败\n",1);
			return;
		}
		limit_import(&stu[STU].sex,4);   //限制性别输入长度
		stu[STU].id=Id_stu;//相应信息自动生成
		strcpy(stu[STU].password,"123");
		stu[STU].is_drop=false;
		stu[STU].autho=0;
		Id_stu++;//加入一个学生后学号自动加1
		msg_show("添加学生成功！\n",1);
		return;
	}
	if(cmd==50)
	{
		char file[20]={};
		printf("请输入需要导入的学生文件：\n");
		scanf("%s",file);
		FILE* fstup=fopen(file,"r");
		if(NULL==fstup)//判断文件是否输入无误
		{
			msg_show("源文件不存在，请检查\n",1);
			return;
		}
		while(fscanf(fstup,"%s %c",stu[STU].name,&stu[STU].sex) && 0!=strcmp(stu[STU].name,"\0")) //读取文件内学生的姓名和性别
		{
			stu[STU].id=Id_stu;  //其他信息自动生成默认值
			strcpy(stu[STU].password,"123");
			stu[STU].is_drop=false;
			stu[STU].autho=0;
			Id_stu++;                 //每添加一个学生信息学号就加1
		}
		msg_show("批量导入学生成功！\n",1);
		fclose(fstup);
		return;
	}
}

//删除学生
void del(void)
{
	int num=0,sure=0;
	printf("请输入要删除的学生学号：\n");
	stdin->_IO_read_ptr=stdin->_IO_read_end; //清空输入缓冲区
	scanf("%d",&num);
	for(int i=0;i<STU;i++)//通过学生学号判断是否存在该学生并确认删除
	{
		if(stu[i].id==num) 
		{
			printf("是否删除该学生%s %s %06d，确认删除请再输入该学生学号\n",stu[i].name,stu[i].sex=='w'?"女":"男",stu[i].id);
			scanf("%d",&sure);
			if(sure==num)   //再次确认是否删除
			{
				stu[i].is_drop=true;
				msg_show("删除成功！\n",1);
				return;
			}
			else    //再次输入学号，两次相同则删除成功，两次不同则删除失败
			{
				msg_show("删除失败！\n",1);
				return;
			}
		} 
	}
	msg_show("查无此人，删除失败！\n",1);  //未进入查找到学生分支则显示查无此人
}

//查找学生
void find(void)
{
	char key[20]={};
	printf("请输入要查寻的关键字：\n");
	scanf("%s",key);
	for(int i=0;i<STU;i++)
	{
		if(strstr(stu[i].name,key) || atoi(key)==stu[i].id) //输入学号或者姓名显示包含有该信息的所有学生
		{
			printf("%s %s %06d %s %s 成绩：语文=%lf，数学=%lf，英语=%lf\n ",stu[i].name,stu[i].sex=='w'?"女":"男",stu[i].id,stu[i].is_drop?"已退学":"在校",stu[i].autho==4?"帐号已锁定":"帐号未锁定",stu[i].chinese_mark,stu[i].math_mark,stu[i].english_mark);
		}
	}
	anykey_continue();
}
//修改学生信息
void mod(void)
{
	char key[20]={},key1[20]={};
	int num=0,i=0,lie=0;
	printf("请输入要修改的学生学号\n");
	stdin->_IO_read_ptr=stdin->_IO_read_end;
	scanf("%d",&num);
	for(i=0;i<STU;i++)
	{
		if(stu[i].id==num)//先显示该学生原先信息
		{
			lie=i;    //标记下标
			printf("学生信息：%s %s %06d %s %s 成绩：语文=%lf，数学=%lf，英语=%lf\n ",stu[i].name,stu[i].sex=='w'?"女":"男",stu[i].id,stu[i].is_drop?"已退学":"在校",stu[i].autho==4?"帐号已锁定":"帐号未锁定",stu[i].chinese_mark,stu[i].math_mark,stu[i].english_mark);
			break;	
		}	
	}
	if(i==STU) //遍历后未找到相应学生则姓名和学号都未找到
	{
		msg_show("查无此人！\n",1);
		return;
	}
	printf("请选择要修改的学生信息，按1修改学生基础信息，按2修改学生成绩\n");
	stdin->_IO_read_ptr=stdin->_IO_read_end;
	char cmd=getch();
	if(cmd==49)
	{
		printf("请重新输入学生的姓名、性别、是否退学信息\n");
		scanf("%s %s %s",stu[lie].name,key,key1);
		stu[lie].sex=strcmp(key,"男")?'w':'m';
		stu[lie].is_drop=strcmp(key1,"是")?false:true; //修改学生信息是否退学实现学生退学、复学
		msg_show("修改学生信息成功!\n",1);
		return;
	}
	if(cmd==50)
	{
		printf("请重新输入学生的语文、数学、英语成绩\n");
		scanf("%lf %lf %lf",&stu[i].chinese_mark,&stu[i].math_mark,&stu[i].english_mark);
		msg_show("修改学生成绩成功！\n",1);
		return;
	}
}

//录入学生成绩
void input_grade(void)
{
	printf("请选择成绩录入方式，按1单个录入，按2批量导入\n");
	char cmd=getch();
	if(cmd==49)
	{
		int num=0,i=0,lie=0;
		printf("请输入要导入的学生学号\n");
		scanf("%d",&num);
		for(i=0;i<STU;i++)
		{
			if(stu[i].id==num) //遍历学生找到要导入成绩的学生
			{
				lie=i;
				printf("学生信息：%s %s %06d\n",stu[i].name,stu[i].sex=='w'?"女":"男",stu[i].id); //显示学生信息
				break;	
			}
		
		}
		if(i==STU)
		{
			msg_show("查无此人！\n",1);
			return;
		}
		printf("请录入学生的语文成绩\n");
		if(scanf("%lf",&stu[lie].chinese_mark)) printf("请录入学生的数学成绩\n");
		else 
		{
			msg_show("成绩录入错误！\n",1);
			return;
		}
		if(scanf("%lf",&stu[lie].math_mark)) printf("请录入学生的英语成绩\n");
		else 
		{
			msg_show("成绩录入错误！\n",1);
			return;
		}
		if(scanf("%lf",&stu[lie].english_mark)) msg_show("成绩录入成功！",1);
		else 
		{
			msg_show("成绩录入错误！\n",1);
			return;
		}
	}
	if(cmd==50)
	{
		char file[20]={};
		printf("请输入需要导入的学生成绩文件：\n");
		scanf("%s",file);
		FILE* fstu_markp=fopen(file,"r");
		if(NULL==fstu_markp) //判断文件是否输入错误
		{
			msg_show("源文件不存在，请检查\n",1);
			return;
		}
		int a=0,ak=0;
		double chinese_mark=0,math_mark=0,english_mark=0; //设置中间变量存储文件中读到的学号和成绩赋值给相应的学生
		while(fscanf(fstu_markp,"%d %lf %lf %lf",&a,&chinese_mark,&math_mark,&english_mark) && ak<Id_stu)
		{
			for(int i=0;i<STU;i++)
			{
				if(stu[i].id==a) //遍历学生，学号与文件内学号相同则录入对应成绩 
				{
					stu[i].chinese_mark=chinese_mark;
					stu[i].math_mark=math_mark;
					stu[i].english_mark=english_mark;
				}
			}
			ak++;   //退出循环
		}
		msg_show("批量导入学生成绩成功！\n",1);
		fclose(fstu_markp);
	}
	
}

//重置学生密码
void reset_password(void)
{
	int num=0,i=0;
	printf("请输入要重置密码学生的学号\n");
	scanf("%d",&num);
	for(i=0;i<STU;i++)
	{
		if(stu[i].id==num) break;
	}
	if(i==STU) 
	{
		msg_show("查无此人！\n",1);
		return;
	}
	if(stu[num-1].autho==4)    //通过学号确认学生并重置密码，再通过学生登录是否输错三次密码确认是否可解锁帐号
	{
		printf("%s %06d",stu[num-1].name,stu[num-1].id);
		stu[num-1].autho=1;
		strcpy(stu[num-1].password,"123");
		msg_show("该学生密码重置成功！\n",1);
		return;
	}
	else //不满足锁定条件则不解锁,但是密码仍然重置
	{
		strcpy(stu[num-1].password,"123");
		msg_show("密码重置成功！该学生帐号未锁定!\n",1);
		return;
	}
}

//显示学生信息（在校、离校）
void show_in(void)
{
	for(int i=0;i<STU;i++)
	{
		if(stu[i].is_drop==false) //通过判断是否在校显示在校学生
		{
			printf("%8s %4s %06d %8s %8s 成绩：语文=%.2lf，数学=%.2lf，英语=%.2lf\n",stu[i].name,stu[i].sex=='w'?"女":"男",stu[i].id,stu[i].is_drop?"已退学":"在校",stu[i].autho==4?"帐号已锁定":"帐号未锁定",stu[i].chinese_mark,stu[i].math_mark,stu[i].english_mark);
		}
	}
	printf("\n部分学生成绩为0可能是成绩未录入！\n");
	anykey_continue();
}
void show_out(void)
{
	for(int i=0;i<STU;i++)
	{
		if(stu[i].is_drop) //通过判断是否离校显示离校学生
		{
			printf("%8s %4s %06d %8s %8s 成绩：语文=%.2lf，数学=%.2lf，英语=%.2lf\n",stu[i].name,stu[i].sex=='w'?"女":"男",stu[i].id,stu[i].is_drop?"已退学":"在校",stu[i].autho==4?"帐号已锁定":"帐号未锁定",stu[i].chinese_mark,stu[i].math_mark,stu[i].english_mark);
		}
	}
	printf("\n部分学生成绩为0可能是成绩未录入！\n");
	anykey_continue();
}
