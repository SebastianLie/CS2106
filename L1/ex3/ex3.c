/*************************************
* Lab 1 Exercise 3
* Name     : Le Trung Hieu
* Matric No: A0161308M
* Lab Group: 06
*************************************/

#include <stdio.h>

//Datatype Declarations
typedef void (*instructionPtr)(int*, int*, int);

void up(int* X, int* Y, int disp) {
	*Y = *Y + disp;
}

void down(int* X, int* Y, int disp) {
	*Y = *Y - disp;
}

void left(int* X, int* Y, int disp) {
	*X = *X - disp;
}

void right(int* X, int* Y, int disp) {
	*X = *X + disp;
}

int main()
{
	instructionPtr arr[4] = {&up, &down, &left, &right};
	int command, disp;
	int X = 0; int Y = 0;
	while(scanf("%i %i", &command, &disp) == 2) {
		(*(arr[command - 1]))(&X, &Y, disp);
		printf("%i %i\n", X, Y); 
	}
}

