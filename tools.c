#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <getch.h>
#include <unistd.h>
#include "tools.h"
int Serial=0;						//用于登陆时进行遍历账号，登录成功后Serial即表示现在所处的界面是哪个人
extern int Id_stu;
extern int Id_tea;
extern int Autho;
extern char Admin_password[20];

int limit_import(char* import, int n)   //限制输入信息长度
{
	int i = 0;
	for (i = 0; i < n * 2; i++)
	{
		import[i] = getchar();  //读取按键输入信息
		if (import[n] != '\0')    //若第n（限定长度）位上扔有数
		{
			for (i = 0; i < n; i++) import[i] = '\0';  //清空之前的输入并提示信息
			msg_show("输入过长！\n", 1);
			break;
		}
		if (import[i] == ' ' || import[i] == '\n')  //遇到 或 '\0'读取完
		{
			import[i] = '\0';
			break;
		}
	}
	stdin->_IO_read_ptr = stdin->_IO_read_end;
	if (i == n) return 0;
	else return 1;
}

//输入时按Tab选择是否不可见  、 限制输入长度
void no_see_in(char* password,int n)     //password代表要写入的数组，n代表要限制的输入长度
{
	int i=0,m=0,k=0;					 //k记录现在密码显示状态，0为不可见，1为可见 m用于接住每一次的输入 i为现在已经输入的数量和即将输入的位数的下标
	for(;;)
	{
		stdin->_IO_read_ptr=stdin->_IO_read_end;
		m=getch();
		if(m==9&&k==0)					//如果输入Tab且现在状态为不可见，回到行首，通过循环将现在显示的'*'替换为现在已经输入的
		{
			putchar('\r');				
			k=1;
			for(int j=0;j<i;j++)
			{
				putchar(password[j]);
			}
		}
		else if(m==9&&k==1)				//如果输入Tab且现在状态为不可见，回到行首，通过循环将现在显示的密码替换为'*'
		{
			putchar('\r');
			k=0;
			for(int j=0;j<i;j++)
			{
				putchar('*');
			}			
		}
		else							//如果没有按Tab键
		{
			if(i==n){}					//当输入个数到最大值时，继续接键值但是除了enter键和删除键不会继续操作
			if(m==10&&i!=0)break;		//当输入enter按键且长度不为0时，直接跳出循环结束输入
			else if(m==10&&i==0){}
			else if(m==127)				//当输入删除键时
			{
				if(i>0)					//当输入个数不为0时将光标前移一格替换为空格后再前移一格
				{
					putchar('\b');
					putchar(' ');
					putchar('\b');
					--i;
				}
				else{}				    //当输入个数为0时删除键不起效果
			}
			else if(i<n&&k==0)
			{
				password[i]=m;			//当现在状态为不显示时，继续输入非Tab、enter、删除键的键则打印'*'
				putchar('*');
				i++;
			}
			else if(i<n&&k==1)
			{							//当现在状态为显示时，继续输入非Tab、enter、删除键的键则打印实际密码
				password[i]=m;
				putchar(password[i]);
				i++;		
			}
		}
	}
	password[i]='\0';					//结束输入在字符串末尾赋结束符
	putchar('\n');
}
//登录函数
char sign_in(void)
{
	system("clear");
	printf("*************************************************\n");
	printf("            欢迎使用河马管理系统！\n");
	printf("请选择你的身份：1、学生 2、教师 3、校长 4、退出\n");
	printf("*************************************************\n");
	int a=0;
	char m=0;
	for(;;)
	{
		char b[256]={};                                 		   //b用来存储输入的密码
		stdin->_IO_read_ptr=stdin->_IO_read_end;
		m=getch();                                      		   //用来判断按1进入学生，按2进入教师，按3进入校长
		if(m==49)												   //选择身份界面按1进入判断学生学号密码分支
		{
			system("clear");
			printf("请输入帐号:\n");
			scanf("%d",&a);
			for(Serial=0;Serial<Id_stu-1;Serial++)     			   //遍历所有学号与输入的学号进行比较
			{
				if((!(stu[Serial].is_drop))&&(stu[Serial].id==a))  //如果学号相等且未退学，则进入输密码界面
				{
					if(stu[Serial].autho==4){printf("帐号已锁定并强制退出\n请重新选择身份1、学生2、老师3、校长4、退出：\n");break;}//判断帐号是否输错密码达到3次，若达到则不让继续操作
					printf("\n按下Tab键 显示/隐藏 密码\n");
					printf("输入密码（默认123）：\n");
						stdin->_IO_read_ptr=stdin->_IO_read_end;
						no_see_in(b,14);                                //让用户输入密码且限制为14位
						if(strcmp(stu[Serial].password,b)==0)           //如果密码输入正确则进入
						{
							if(stu[Serial].autho==0)                    //如果此前一次登录都没有过则强制进入修改密码界面
							{
								stdin->_IO_read_ptr=stdin->_IO_read_end;
								pass_change(1,Serial+1);
								stu[Serial].autho=1;					//密码修改完成后将autho数值改为1，下次重新进入时不需要再修改密码
								return 1;								//返回值用于调用后续教师、学生或校长，此处为学生
							}
							else
							{
								stu[Serial].autho=1;					//登录成功后autho数值置1,重新开始密码错误累计
								return 1;								//返回值用于调用后续教师、学生或校长，此处为学生
							}
						}
						else											//密码输错直接进入此分支，autho-1即现在密码错误的次数
						{
							if(stu[Serial].autho!=0)					//非初次登录失败
							{
								printf("登陆失败,本帐号累计错误%d次\n\n\n请重新选择身份:1、学生2、老师3、校长4、退出\n",stu[Serial].autho);
								stu[Serial].autho++;
								break;
							}
							else if(stu[Serial].autho==0)
							{
								printf("初次登录失败！！！\n\n\n请重新选择身份1、学生2、老师3、校长4、退出\n");//初次登录失败
								break;
							}
						}
				}
			}
			if(Serial==Id_stu-1) printf("查无此人\n重新选择身份:1、学生2、老师3、校长4、退出\n");   //如果遍历完所有的学号仍然找不到与用户输入对应的学号进入此分支
		}
		else if(m==50)											    					   //选择身份界面按2进入判断教师账号密码分支
		{
			system("clear");
			printf("请输入帐号:\n");
			scanf("%d",&a);
			for(Serial=0;Serial<Id_tea-1;Serial++)										   //遍历所有账号与输入的账号进行比较
			{
				if((!(tea[Serial].is_drop))&&(tea[Serial].id==a))						   //如果账号相等且未退学，则进入输密码界面
				{
					if(tea[Serial].autho==4){printf("帐号已锁定并强制退出\n请重新选择身份1、学生2、老师3、校长4、退出：\n");break;}
					printf("\n按下Tab键 显示/隐藏 密码\n");									  
					printf("输入密码（默认123）：\n");
					stdin->_IO_read_ptr=stdin->_IO_read_end;
					no_see_in(b,14);													   //让用户输入密码且限制为14位
					if(strcmp(tea[Serial].password,b)==0)								   //如果密码输入正确则进入
					{
						if(tea[Serial].autho==0)										   //如果此前一次登录都没有过则强制进入修改密码界面
						{
							stdin->_IO_read_ptr=stdin->_IO_read_end;
							pass_change(2,Serial+1);
							tea[Serial].autho=1;									//密码修改完成后将autho数值改为1，下次重新进入时不需要再修改密码
							return 2;												//返回值用于调用后续教师、学生或校长，此处为教师
						}
						else
						{
							if(tea[Serial].autho!=0)					//非初次登录失败
							{						
								tea[Serial].autho=1;									//登录成功后autho数值置1,重新开始密码错误累计
								return 2;												//返回值用于调用后续教师、学生或校长，此处为校长
							}
							else if(tea[Serial].autho==0)										//初次登录失败
							{
								printf("初次登录失败！！！\n\n\n请重新选择身份1、学生2、老师3、校长4、退出\n");//初次登录失败
								break;
							}
						}
					}
					else															//密码输错直接进入此分支，autho-1即现在密码错误的次数
					{
						printf("登陆失败,本帐号累计错误%d次\n\n\n请重新选择身份:1、学生2、老师3、校长4、退出\n",tea[Serial].autho);
						tea[Serial].autho++;
						break;
					}
				}
			}
			if(Serial==Id_tea-1) printf("查无此人\n重新选择身份:1、学生2、老师3、校长4、退出：\n");//如果遍历完所有的学号仍然找不到与用户输入对应的学号进入此分支
		}
			else if(m==51)
			{
				system("clear");
				printf("请输入帐号：\n");
				stdin->_IO_read_ptr=stdin->_IO_read_end;   //清空输入缓冲区
				if(scanf("%d",&a) && a==0)
				{
					if(Autho>3) printf("您已多次输错密码，如忘记密码请联系工作人员！\n"); //多次输入错误则提示
					if(Autho==0) //判断是否第一次进入系统，是则修改密码
					{
						printf("请输入密码（默认123）：\n");
						printf("\n按下Tab键 显示/隐藏 密码\n");									  
						no_see_in(b,14);
						stdin->_IO_read_ptr=stdin->_IO_read_end;
						printf("请修改密码：\n");
						stdin->_IO_read_ptr=stdin->_IO_read_end;
						no_see_in(Admin_password,14);
						Autho=1;
						return 3;
					}
					if(Autho>0)  //不是第一次进入则只提示输入密码不修改
					{
						printf("请输入密码（默认123）：\n");
						printf("\n按下Tab键 显示/隐藏 密码\n");									  
						no_see_in(b,14);
						stdin->_IO_read_ptr=stdin->_IO_read_end;
						if(strcmp(b,Admin_password)==0)  //密码输入正确则进入校长系统且输入次数归1
						{
							Autho=1;
							return 3;
						}
						else//密码错误则返回
						{
							printf("登录失败，请重新选择身份：1、学生2、老师3、校长4、退出\n");
							Autho++;//累计输入错误次数
						}
					}
				 }
				 else printf("查无此人\n重新选择身份:1、学生2、老师3、校长4、退出\n");   //校长帐号只有一个，输错直接退出！
				 
			}
			else if(m==52) return 4; //退出系统
			else //选择其他身份提示错误
			{
				system("clear");
				printf("输入不合规，请重新输入身份1、学生2、教师3、校长：\n");
			}
	}
}
//密码修改函数
void pass_change(char who,int num)     //who为1代表对学生进行操作，2代表对老师，3代表对校长  num代表对哪个学生进行密码修改操作
{
	char new_pass[256];				   //两个数组分别接输入的两次密码
	char new_pass2[256];
	char wrong[40]="密码不一致!!!请重新输入\n";
	for(;;)
	{
		system("clear");
		printf("按Tab 显示/隐藏 密码\n请输入新密码：\n");
		no_see_in(new_pass,14);		  //输入一次密码
		printf("确认密码：\n");		  
		no_see_in(new_pass2,14);	  //输入第二次密码
		if(!strcmp(new_pass,new_pass2))	//如果两次密码输入一致
		{
			if(who==1)			//输入的第一个参数为1则对学生修改
			{strcpy(stu[num-1].password,new_pass);return;}
			else if(who==2)		//输入为2则对教师修改
			{strcpy(tea[num-1].password,new_pass);return;}
			else if(who==3)		//输入为3则对校长修改
			{strcpy(Admin_password,new_pass);return;}
		}
		else msg_show(wrong,1);	//两次密码不一致则输出错误提示一段时间之后重新输入密码
	}
	return;
}

void msg_show(const char* msg,float sec) //打印提示信息
{
	printf("%s",msg);
	fflush(stdout);
	usleep(sec*1000000);
	return;
}
void anykey_continue(void)   //按任意键继续
{
	printf("请按任意键继续...");
	stdin->_IO_read_ptr=stdin->_IO_read_end;
	getch();
}
void readfile(const char* src,const char* dest,const char* admin,const char* pw_stu,const char* pw_tea)  //读取文件，参数接收学生信息文件、教师信息文件、校长信息文件、学生密码文件、教师密码文件
{
	FILE* freadp_stu=fopen(src,"r");
	FILE* freadp_tea=fopen(dest,"r");
	FILE* freadp_admin=fopen(admin,"r");  //打开文件
	FILE* freadp_pw_stu=fopen(pw_stu,"r");
	FILE* freadp_pw_tea=fopen(pw_tea,"r");
	int i=0;
	char key[20]={},temp[20]={};    //局部变量作中间值
	while(fscanf(freadp_stu,"%s %c %lf %lf %lf %d %s %hhd ",stu[i].name,&stu[i].sex,&stu[i].chinese_mark,&stu[i].math_mark,&stu[i].english_mark,&stu[i].id,key,&stu[i].autho) && stu[i].id)
	{  //读取文件内信息，两个判断条件结束循环
		stu[i].is_drop=strcmp(key,"在校");
		i++;
		Id_stu++;  //学生总人数加1
	}
	for(int j=0;j<Id_stu;j++)
	{  //遍历学生通过判断id是否存在进行解密
		fscanf(freadp_pw_stu,"%s ",stu[j].password);//读取学生密码
		if(stu[j].id)
		{    //解密从文件中读取的密码
			for(int k=0;k<strlen(stu[j].password);k++)
			{
				stu[j].password[k]-=(('A'-60)*5-5)/4;   //荷马夫兰斯基加密法
			}
		}
		else break;
	}
	i=0;
	while(fscanf(freadp_tea,"%s %c %d %s %hhd ",tea[i].name,&tea[i].sex,&tea[i].id,key,&tea[i].autho) && tea[i].id)
	{   //读取文件内信息，两个判断条件结束循环
		tea[i].is_drop=strcmp(key,"在职");
		i++;
		Id_tea++; //教师总人数加1
	}
	for(int j=0;j<Id_tea;j++)
	{   //遍历教师通过判断id是否存在进行解密
		fscanf(freadp_pw_tea,"%s ",tea[j].password);
		if(tea[j].id)
		{
			for(int k=0;k<strlen(tea[j].password);k++)
			{
				tea[j].password[k]-=(('A'-60)*5-5)/4;
			}
		}
		else break;
	}
	fscanf(freadp_admin,"%d %s",&Autho,temp);
	for(char j=0;j<strlen(temp);j++)  //读取校长信息
	{
		Admin_password[j]=temp[j]-(('A'-60)*5-5)/4;
	}
	fclose(freadp_stu);
	fclose(freadp_tea);
	fclose(freadp_admin); //关闭文件
	fclose(freadp_pw_stu);
	fclose(freadp_pw_tea);
}
void savefile(const char* src,const char* dest,const char* admin,const char* pw_stu,const char* pw_tea)  //存储信息
{
	FILE* fsavep_stu=fopen(src,"w");
	FILE* fsavep_tea=fopen(dest,"w");
	FILE* fsavep_admin=fopen(admin,"w"); //打开文件
	FILE* fsavep_pw_stu=fopen(pw_stu,"w");
	FILE* fsavep_pw_tea=fopen(pw_tea,"w");
	int i=0;
	char temp[20]={};
	while(stu[i].id && fprintf(fsavep_stu,"%s %c %lf %lf %lf %d %s %hhd ",stu[i].name,stu[i].sex,stu[i].chinese_mark,stu[i].math_mark,stu[i].english_mark,stu[i].id,stu[i].is_drop?"离校":"在校",stu[i].autho))
	{  //存储信息，通过两个条件结束循环，运用相与运算短路特性
		i++;
	}
	for(int j=0;j<Id_stu;j++)
	{   //遍历学生通过id是否存在进行密码加密
		if(stu[j].id)
		{
			for(int k=0;k<strlen(stu[j].password);k++)
			{
				stu[j].password[k]+=(('A'-60)*5-5)/4;
			}
			fprintf(fsavep_pw_stu,"%s ",stu[j].password);
		}
		else break;
	}
	i=0;
	while(tea[i].id && fprintf(fsavep_tea,"%s %c %d %s %hhd ",tea[i].name,tea[i].sex,tea[i].id,tea[i].is_drop?"离职":"在职",tea[i].autho))
	{  //存储信息，通过两个条件结束循环
		i++;
	}
	for(int j=0;j<Id_tea;j++)
	{
		if(tea[j].id)
		{ //遍历教师通过id是否存在进行密码加密
			for(int k=0;k<strlen(tea[j].password);k++)
			{
				tea[j].password[k]+=(('A'-60)*5-5)/4;
			}
			fprintf(fsavep_pw_tea,"%s ",tea[j].password);
		}
		else break;
	}
	for(char j=0;j<strlen(Admin_password);j++) //存储校长信息并加密
	{
		temp[j]=Admin_password[j]+(('A'-60)*5-5)/4;
	}
	fprintf(fsavep_admin,"%d %s",Autho,temp);
	fclose(fsavep_stu);
	fclose(fsavep_tea);
	fclose(fsavep_admin); //关闭文件
	fclose(fsavep_pw_stu);
	fclose(fsavep_pw_tea);
}
