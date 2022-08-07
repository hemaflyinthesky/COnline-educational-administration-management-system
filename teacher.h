#ifndef TEACHER_H
#define TEACHER_H




//
void teacher_time(void);

//初始化教师界面
int show_tea(void);

//添加学生
void add(void);

//删除学生
void del(void);

//查找学生
void find(void);

//修改学生信息
void mod(void);

//录入学生成绩
void input_grade(void);

//重置学生密码
void reset_password(void);

//显示学生信息（在校、离校）
void show_in(void);

void show_out(void);
#endif //TEACHER_H
