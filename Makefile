all:main.o tools.o teacher.o student.o admin.o
	gcc main.o tools.o teacher.o student.o admin.o -o best
main.o:main.c tools.h teacher.h admin.h student.h
	gcc -c main.c
teacher.o:teacher.c teacher.h tools.h
	gcc -c teacher.c
tools.o:tools.c tools.h
	gcc -c tools.c
admin.o:admin.c admin.h tools.h
	gcc -c admin.c
student.o:student.c student.h tools.h 
	gcc -c student.c


clean:
	rm -rf best main.o teacher.o tools.o admin.o student.o
