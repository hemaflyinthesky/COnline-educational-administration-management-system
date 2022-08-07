#ifndef TOOLS_H
#define TOOLS_H
char sign_in(void);
int Serial;//当前帐号
void readfile(const char* src,const char* dest,const char* admin,const char* pw_stu,const char* pw_tea); //读取文件
void savefile(const char* src,const char* dest,const char* admin,const char* pw_stu,const char* pw_tea); //存储信息
void pass_change(char who,int num); //修改密码
void msg_show(const char* msg,float sec); //打印提示信息
void anykey_continue(void); //任意键退出
void no_see_in(char* password,int n); //密码是否可见
int limit_import(char *import,int n); //限制输入长度
typedef struct Teacher
{
	char name[256];  //教师姓名
	char sex;			//教师性别
	char password[15];	//教师密码
	int id;					//教师工号
	bool is_drop;		//教师是否在职
	char autho;		//教师密码输入次数
}Teacher;
Teacher* tea;      //教师结构体
typedef struct Student 
{
	char name[256];   //学生姓名
	char sex;		//学生性别
	char password[15];		//学生密码
	double chinese_mark; //语文成绩
	double math_mark;	//数学成绩
	double english_mark;	//英语成绩
	int id;		//学生学号
	bool is_drop;    //是否退学
	char autho;      //登录权限
}Student;
Student* stu;
#endif//TOOLS_H
