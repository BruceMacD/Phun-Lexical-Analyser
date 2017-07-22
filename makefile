all: phun

clean:
	rm phun *.o

test: phun sanityCheck.dat
	./phun sanityCheck.dat

phun: main.o lexer.o parser.o eval.o
	gcc main.o lexer.o parser.o eval.o -o phun
    
main.o: main.c phun.h
	gcc -c main.c
eval.o: eval.c phun.h
	gcc -c eval.c
parser.o: parser.c phun.h
	gcc -c parser.c
lexer.o: lexer.c phun.h
	gcc -c lexer.c
    
phun.h:
	touch *.c
