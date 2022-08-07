#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "student.h"
#include "teacher.h"
#include "admin.h"
#include "tools.h"
int Id_stu=1;  //定义学生人数全局变量
int Id_tea=1;  //定义教师人数全局变量
char Admin_password[20];  //定义校长密码全局变量
int Autho;  //定义校长密码输入次数全局变量
int main(int argc,const char* argv[])
{
	stu=calloc(sizeof(Student),500);//申请学生堆内存
	tea=calloc(sizeof(Teacher),500);//申请教师堆内存
	readfile("student_all.txt","teacher_all.txt","admin.txt","stu_pass.txt","tea_pass.txt"); //读取信息
	for(;;)
	{
		switch(sign_in()) //调用登录函数
		{
			case 1:student_time();break; //进入学生系统
			case 2:teacher_time();break; //进入教师系统
			case 3:admin_time();break;  //进入校长系统
			case 4:savefile("student_all.txt","teacher_all.txt","admin.txt","stu_pass.txt","tea_pass.txt");free(stu);free(tea);return 0;break;
			//先存储信息再释放堆内存然后结束程序
		}
	}
}
