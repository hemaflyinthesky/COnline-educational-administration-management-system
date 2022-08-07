#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <getch.h>
#include <stdio.h>
#include "admin.h"
#include "tools.h"
#define TEA Id_tea-1   //定义宏存储教师总人数
extern char Admin_password[20];//引用全局变量校长的密码
extern char Autho; //引用全局变量校长密码输入次数
extern int Id_tea;
void admin_time(void)
{
	for(;;)
	{
		switch(show_admin()) //初始化校长界面
		{
			case 49:reset();break;
			case 50:reset_tea();break;
			case 51:add_tea();break;
			case 52:del_tea();break;
			case 53:show_in_tea();break;
			case 54:show_out_tea();break;
			case 55:free_tea();break;
			case 56:return;
		}
	}
}

int show_admin(void)  //初始化校长界面
{
	system("clear");
	puts("    欢迎校长，请选择你要进行的操作    ");
	puts("**************************************");
	puts("1.重置密码");
	puts("2.重置教师密码");
	puts("3.添加教师");
	puts("4.删除教师");
	puts("5.显示在职教师");
	puts("6.显示离职教师");
	puts("7.解锁教师帐号");
	puts("8.退出");
	puts("**************************************");
	char cmd=getch();
	return cmd;
}
void reset(void) //重置密码
{
	printf("重置成默认密码请按1,修改密码请按2，返回请按其他键\n");
	char cmd=getch();
	if(cmd==49) 
	{
		Autho=1;  //密码归为默认，输入次数归为1
		strcpy(Admin_password,"123");
		msg_show("重置密码成功\n",1);
		return;
	}
	if(cmd==50)
	{
		pass_change(3,0);  //调用pass_change函数修改密码
		msg_show("修改密码成功\n",1);
		return;
	}
}
void reset_tea(void) //重置教师密码
{
	int num=0,i=0;
	printf("请输入要重置密码教师的工号\n");
	scanf("%d",&num);
	for(i=0;i<TEA;i++)
	{
		if(tea[i].id==num) //通过工号判断是否存在并重置密码
		{
			printf("%s %06d",tea[i].name,tea[i].id);
			strcpy(tea[num-1].password,"123");
			tea[num-1].is_drop=false;
			msg_show("该教师密码重置成功！\n",1);
			return;
		}
	}
	if(i==TEA)
	{
		msg_show("查无此人！\n",1);
		return;
	}
}
void add_tea(void)   //添加教师
{
	printf("请选择添加方式，按1单个添加，按2批量导入\n");
	char cmd=getch();
	if(cmd==49)
	{
		printf("请输入新添加老师的姓名、性别，其他信息会自动生成\n");
		stdin->_IO_read_ptr=stdin->_IO_read_end; //清空输入缓冲区
		limit_import(tea[TEA].name,30);    //限制输入长度
		if(strcmp(tea[TEA].name,"\0")==0)  //超过限制长度则不添加
		{
			strcmp(tea[TEA].name,"\0");
			msg_show("添加失败\n",1);         //提示输入过长添加失败
			return;
		}
		limit_import(&stu[TEA].sex,4);
		tea[TEA].id=Id_tea;
		strcpy(tea[TEA].password,"123");
		tea[TEA].is_drop=false;
		tea[TEA].autho=0;
		Id_tea++;//教师学号也自动生成
		msg_show("添加教师成功！\n",1);
		return;
	}
	if(cmd==50)
	{
		char file[20]={};
		printf("请输入需要导入的教师文件：\n");
		scanf("%s",file);
		FILE* fteap=fopen(file,"r");
		if(NULL==fteap) //判断文件是否输入错误
		{
			msg_show("源文件不存在，请检查\n",1);
			return;
		}
		while(fscanf(fteap,"%s %c",tea[TEA].name,&tea[TEA].sex) && 0!=strcmp(tea[TEA].name,"\0"))
		{
			tea[TEA].id=Id_tea; 
			strcpy(tea[TEA].password,"123");
			tea[TEA].is_drop=false;
			tea[TEA].autho=0;
			Id_tea++; //每读取到一位教师信息总人数加一
		}
		msg_show("批量导入教师成功！\n",1);
		fclose(fteap);
		return;
	}
}
void del_tea(void)
{
	int num=0,sure=0;
	printf("请输入要删除的教师工号：\n");
	stdin->_IO_read_ptr=stdin->_IO_read_end;
	scanf("%d",&num);
	for(int i=0;i<TEA;i++)
	{
		if(tea[i].id==num) //通过工号确认教师
		{
			printf("是否删除教师%s %s %06d，确认删除请再次输入该教师工号\n",tea[i].name,tea[i].sex=='w'?"女":"男",tea[i].id);
			scanf("%d",&sure);
			if(sure==num)
			{
				tea[i].is_drop=true;//删除成功则教师显示离职
				msg_show("删除成功！\n",1);
				return;
			}
			else
			{
				msg_show("删除失败！\n",1);
				return;
			}
		}
	}
	msg_show("查无此人，删除失败！\n",1);
}
void show_in_tea(void)
{
	for(int i=0;i<TEA;i++)
	{
		if(tea[i].is_drop==false) //判断是否离职显示在职教师
		{
			printf("%8s %4s %06d %8s %8s\n",tea[i].name,tea[i].sex=='w'?"女":"男",tea[i].id,tea[i].is_drop?"已离职":"在职",tea[i].autho==4?"帐号已锁定":"帐号未锁定");
		}
	}
	anykey_continue();
}
void show_out_tea(void)
{
	for(int i=0;i<TEA;i++)
	{
		if(tea[i].is_drop) //判断是否离职显示离职教师
		{
			printf("%8s %4s %06d %8s %8s\n",tea[i].name,tea[i].sex=='w'?"女":"男",tea[i].id,tea[i].is_drop?"已离职":"在职",tea[i].autho==4?"帐号已锁定":"帐号未锁定");
		}
	}
	anykey_continue();
}
void free_tea(void)
{
	int num=0,i=0;
	printf("请输入需要解锁的教师的工号：\n");
	scanf("%d",&num);
	for(i=0;i<TEA;i++)
	{
		if(tea[i].id==num) //通过工号确认要解锁的教师
		{
			printf("%s %06d",tea[i].name,tea[i].id);
			tea[i].autho=1; //密码输入次数归1
			break;
		}
	}
	if(i==TEA)
	{
		msg_show("查无此人！\n",1);
		return;
	}
	msg_show("该教师帐号已解锁！\n",1);
	return;
}
