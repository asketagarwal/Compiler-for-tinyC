a.out: lex.yy.c y.tab.c y.tab.h ass6_14CS30006_translator.h ass6_14CS30006_translator.cxx ass6_14CS30006_target_translator.cxx libmyl.a
	g++ -std=c++11 lex.yy.c y.tab.c ass6_14CS30006_translator.cxx ass6_14CS30006_target_translator.cxx -lfl -o a.out
	# Use "make test<number>" to make executables for all 5 test files......

libmyl.a: myl.o
	ar -rcs libmyl.a myl.o

myl.o: myl.c myl.h
	gcc -Wall -c myl.c

y.tab.h: ass6_14CS30006.y
	yacc -dtv ass6_14CS30006.y 
y.tab.c: ass6_14CS30006.y
	yacc -dtv ass6_14CS30006.y 
lex.yy.c: ass6_14CS30006.l y.tab.h
	flex ass6_14CS30006.l

clean: 
	rm a.out y.tab.h y.tab.c lex.yy.c y.output

test1: a.out ass6_14CS30006_test1.c
	./a.out < ass6_14CS30006_test1.c  1
	gcc -c ass6_14CS30006_1.s
	gcc  ass6_14CS30006_1.o -L. -g -lmyl -o test1.out
	./test1.out

test2: a.out ass6_14CS30006_test2.c
	./a.out < ass6_14CS30006_test2.c 2
	gcc -c ass6_14CS30006_2.s
	gcc  ass6_14CS30006_2.o -L. -g -lmyl -o test2.out
	./test2.out

test3: a.out ass6_14CS30006_test3.c
	./a.out < ass6_14CS30006_test3.c 3
	gcc -c ass6_14CS30006_3.s
	gcc  ass6_14CS30006_3.o -L. -g -lmyl -o test3.out
	./test3.out

test4: a.out ass6_14CS30006_test4.c
	./a.out < ass6_14CS30006_test4.c 4
	gcc -c ass6_14CS30006_4.s
	gcc  ass6_14CS30006_4.o -L. -g -lmyl -o test4.out
	./test4.out


test5: a.out ass6_14CS30006_test5.c
	./a.out < ass6_14CS30006_test5.c 5
	gcc -c ass6_14CS30006_5.s
	gcc  ass6_14CS30006_5.o -L. -g -lmyl -o test5.out
	./test5.out

