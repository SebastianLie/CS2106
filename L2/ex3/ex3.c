/*************************************
* Lab 2 Exercise 3
* Name: Le Trung Hieu
* Student No: A0161308M
* Lab Group: Lab 6
*************************************
Warning: Make sure your code works on
lab machine (Linux on x86)
*************************************/

#include <stdio.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>   
#include <sys/stat.h>
#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()


// readTokens and freeTokenArray is from stringTokenizer.c

char** readTokens( int maxTokenNum, int maxTokenSize, int *readTokenNum )
//Assumptions:
//  - the input line is < 120 characters
//  - the tokens are separated by " " and ended by newline
//Return: Tokenized substrings as array of strings
//        readTokenNum stores the total number of tokens
//Note:
//  - should use the freeTokenArray to free memory after use!
{
    char input[120];
    char** tokenStrArr;
    char* tStart;   //start of token
    int i;

    //allocate token array, each element is a char*
    tokenStrArr = (char**) malloc(sizeof(char*) * maxTokenNum);

    //Nullify all entries
    for (i = 0; i < maxTokenNum; i++){
        tokenStrArr[i] = NULL;
    }

    fgets(input, 120, stdin);

    tStart = strtok(input, " \n");

    i = 0;
    while (i < maxTokenNum && tStart){
        //Allocate space for token string
        tokenStrArr[i] = (char*) malloc(sizeof(char) * maxTokenSize );

        //Ensure at most 19 + null characters are copied
        strncpy(tokenStrArr[i], tStart, maxTokenSize);

        //Add NULL terminator in the worst case
        tokenStrArr[i][maxTokenSize-1] = '\0';

        i++;
        tStart = strtok(NULL, " \n");
    }

    //return number of token read
    *readTokenNum = i;

    return tokenStrArr;
}

void freeTokenArray(char** strArr, int size)
{
    int i;

    //Free each string first
    for (i = 0; i < size; i++){
        if (strArr[i])
            free( strArr[i] );
        strArr[i] = NULL;
    }

    //Free the entire array
    free( strArr );

    //Note: Caller still need to set the strArr parameter to NULL
    //      afterwards
}


char** clone(char** source, int argNum) {
    char** cloneInput = (char **) malloc(sizeof(char*) * 100);
    for(int i = 0; i < argNum; i ++) {
        cloneInput[i] = (char*) malloc(sizeof(char*) * 20);
        strcpy(cloneInput[i], source[i]);
    }
    return cloneInput;
}


int commandPath(char** input, int argNum, int* childPidArr, int numProcess) {
     struct stat fileStat;
     if(stat(input[0], &fileStat) < 0) {
	printf("%s not found \n", input[0]);
     } else {	
	int pid = fork();
     	if (pid == 0) {
		// if last arg is "&" then make this a background process
		if(strcmp(input[argNum - 1], "&") == 0) {
			char** tempInput = clone(input, argNum - 1);
			execv(input[0], tempInput);
		} else {
	        	execv(input[0], input);
		}
     	} else {
		// if its children is a background process then add its pid to our table
		if(strcmp(input[argNum - 1], "&") == 0) {
			childPidArr[numProcess] = pid;
			printf("Child %d in background \n", pid);
			return numProcess + 1;
		} else {
			wait(NULL);
		}
	}
     }
     return numProcess;
}

int waitChild(char **input, int numProcess, int* childPidArr) {
    int status;
    int childPid = atoi(input[1]);
    int i;
    for(i = 0; i < numProcess; i ++) {
             if(childPidArr[i] == childPid) {
                    waitpid(childPidArr[i], &status, 0);
                    childPidArr[i] = childPidArr[numProcess - 1];
                    return numProcess - 1;
             }
    }

    printf("%d not a valid child pid\n", childPid);
    return numProcess;

}

void printChild(int* childPidArr, int numProcess) {
     printf("Unwaited Child Processes: \n");
     for(int i = 0; i < numProcess; i ++) {
            printf("%d \n", childPidArr[i]);
     }
}

int main()
{

    // char userInput[120];
    char **input;
    char **lastInput; 
    char* token;
    char* command;
    int argNum = 0;
    int lastArgNum;
    //assume we have at most 100 unwaited background children
    int* childPidArr = (int*) malloc(sizeof(int*) * 100);
    int numProcess, status;
    int maxToken = 7; 
    int maxTokenLength = 20;

    printf("YWIMC > "); 
    
    input = readTokens(maxToken, maxTokenLength, &argNum);	
    command = input[0]; 
 
    while (strcmp(command, "quit") != 0 ){ 
	if(strcmp(command, "last") == 0) {
		// In question b, we need to clone a copy of lastInput and then assign to input.
		// However, here it's not necessary because in c/ we do allocate a heap memory region to store input 
		// and lastInput, resulting in a valid swapping of the pointers.
		input = clone(lastInput, lastArgNum);
		command = input[0];
		continue;
	} else if (strcmp(command, "wait") == 0) {
		numProcess = waitChild(input, numProcess, childPidArr);
	} else if(strcmp(command, "printchild") == 0) {
		printChild(childPidArr, numProcess);
	} else {
		 numProcess = commandPath(input, argNum, childPidArr, numProcess);
	}
 
        printf("YWIMC > ");
	lastInput = clone(input, argNum);
	lastArgNum = argNum;
	input = readTokens(maxToken, maxTokenLength, &argNum);
	command = input[0];
    }
   
    freeTokenArray(input, maxToken); 
    freeTokenArray(lastInput, maxToken);
    input = NULL;
    lastInput = NULL;
    printf("Goodbye!\n");
    return 0;

}
