all: phun

clean:
	rm phun *.o

phun: main.o lexer.o parser.o eval.o
	gcc main.o eval.o lexer.o parser.o -o phun
    
main.o: main.c phun.h
	gcc -c main.c
lexer.o: lexer.c phun.h
	gcc -c lexer.c
parser.o: parser.c phun.h
	gcc -c parser.c
eval.o: eval.c phun.h
	gcc -c eval.c
    
phun.h:
	touch *.c