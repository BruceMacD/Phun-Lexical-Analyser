all: phun

clean:
	rm phun *.o

phun: main.o lexer.o  
	gcc main.o lexer.o -o phun
    
main.o: main.c phun.h
	gcc -c main.c
lexer.o: lexer.c phun.h
	gcc -c lexer.c
    
phun.h:
	touch *.c
