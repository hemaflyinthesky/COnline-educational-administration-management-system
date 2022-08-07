#ifndef STUDENT_H
#define STUDENT_H
     
//学生模块主函数，调用功能函数
void student_time(void);
		   
//显示学生模块首页
void student_first(void);
			     
//返回语文排名
int ranking_chinese(char *chinese_temp);
				   
//对语文进行排序，返回平均分
double sort_chinese(char *chinese_temp);
					  
//返回数学排名
int ranking_math(char *math_temp);
						 
//对数学进行排序，返回平均分
double sort_math(char *math_temp);
						     
//返回英语排名
int ranking_english(char *english_temp);
							    
//对英语进行排序，返回平均分
double sort_english(char *english_temp);
								  
//对总分进行排名，返回排名
int sort_toal(char *total_temp);
									  
#endif//STUDENT_H

