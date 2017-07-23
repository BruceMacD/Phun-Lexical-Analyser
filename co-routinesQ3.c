#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

/* Function Declarations */
void evens();
void odds();

/* Variables */
jmp_buf evenBuffer, oddBuffer;
int numbers[10] = {0};
int n = 0;
int e, o;

/* Main function reads 10 integers from user and calls evens() function to start the co-routines which print the numbers */
int main(int argc, char *argv) {
    int i;
    printf("Enter a sequence of ten integers: ");
    for (i = 0; i < 10; i++) {
        scanf("%d", &numbers[i]);
    }
    evens();
    return 0;
}

/* Function servers as a co-routine for printing all even numbers encountered in the numbers array */
void evens(){
    //Initially pass control to odds()
    e = setjmp(evenBuffer);
    if (e == 0) odds();
    //Loop over all possible values in numbers array checking for even numbers
    while(n < 10){
	//If number is odd pass control to evens()
	if(numbers[n]%2 !=0){
	    e = setjmp(evenBuffer);
            if (e == 0)longjmp(oddBuffer, n);
	}
	//If even print the number and update global counter
	else{
	    printf("Even Number: %d\n", numbers[n]);
	    n++;
	}
    }
    //Exit program when all numbers have been printed
    exit(0);
}

/* Function serves as a co-routine for printing all odd numbers encountered in the numbers array */
void odds(){
    //Loop over all possible values in numbers array checking for odd numbers
    while(n < 10){
	//If number is even pass control to odds()
	if(numbers[n]%2 ==0){
	    o = setjmp(oddBuffer);
    	    if (o == 0) longjmp(evenBuffer, n);
	}
	//If odd print the number and update the global counter
	else{
	    printf("Odd  Number: %d\n", numbers[n]);
	    n++;
	}
    }
    //Exit program when all numbers have been printed
    exit(0);
}
