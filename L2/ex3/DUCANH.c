/**b 2 Exercise 3
* Name: Huynh Thanh Duc Anh
* Student No: A0161312X
* Lab Group: 6
*************************************
Warning: Make sure your code works on
lab machine (Linux on x86)
*************************************
Some parts are code snippets copied from given stringTokenizer.c
*************************************/

#include <stdio.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()

/*************************************
Copied from stringTokenizer.c
*************************************/
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
/*************************************
*************************************/
char** copyTokens(char** tokens, int nTokens, int maxTokenNum, int maxTokenSize)
{
    char** tokenStrArr;
    int i;

    //allocate token array, each element is a char*
    tokenStrArr = (char**) malloc(sizeof(char*) * maxTokenNum);

    //Nullify all entries
    for (i = 0; i < maxTokenNum; i++){
        tokenStrArr[i] = NULL;
    }

    i = 0;
    while (i < nTokens){
        //Allocate space for token string
        tokenStrArr[i] = (char*) malloc(sizeof(char) * maxTokenSize );
        strcpy(tokenStrArr[i], tokens[i]);
        i++;
    }

    return tokenStrArr;
}



//return the child PID if it's a background process, return 0 otherwise
int command_path(char **args, int bg) {
    struct stat sb;
    if (stat(args[0], &sb) >= 0) { // the command is valid
        int pid = fork();
        if (pid == 0) {
            execv(args[0], args);
        }
        if (bg == 0) { //wait for the child process if it isn't a background process
        	wait(NULL);
       	} else {
       		printf("Child %d in background\n", pid);
       		return pid;
       	}
    } else {
        printf("%s not found\n", args[0]);
    }
    return 0;
}

int main()
{
    char **argsArr;
    char **old_argsArr;
    int i, nToken, old_nToken = 0, done, status;
    int cPID[11], cActive[11]; //there're most 10 background jobs in a single test session
    memset(cActive, 0, sizeof(cActive));

    //read user input
    printf("YWIMC > ");
    /*
    Assume the number of input arguments is always less than or equal to 4
	maxTokenNum = 7 including command, up-to-4 args, & (background process), '\n' (if any)
    */
    argsArr = readTokens(7, 20, &nToken);

    while (strcmp(argsArr[0], "quit") != 0 ){
        done = 1; //determine if path can be handled in current iteration
        if (strcmp(argsArr[0], "last") == 0) {
            if (old_nToken > 0) { //not empty
                done = 0;
                argsArr = copyTokens(old_argsArr, old_nToken, 7, 20);
                nToken = old_nToken;
            }
        } else if (strcmp(argsArr[0], "wait") == 0) {
        	i = 0;
        	int childPid = atoi(argsArr[1]);
        	for (i = 0; i<10; i++)
        	 	if (cActive[i] == 1 && cPID[i] == childPid) { break; }
        	if (i == 10) {
        		printf("%d not a valid child pid\n", childPid);
        	} else {
        		cActive[i] = 0;
        		waitpid(childPid, &status, 0);
        	}
        } else if (strcmp(argsArr[0], "printchild") == 0) {
        	//The order of child processes in the result can be random
        	printf("Unwaited Child Processes:\n");
        	for (i = 0; i<10; i++)
        		if (cActive[i] == 1) {
        			printf("%d\n", cPID[i]);
        		}
        } else if (strcmp(argsArr[nToken-1], "&") == 0) {
        	i = 0;
        	while (cActive[i] == 1) i++;
        	char **temp_argsArr;
        	temp_argsArr = copyTokens(argsArr, nToken, 7, 20);
        	temp_argsArr[nToken-1] = NULL;
        	cPID[i] = command_path(temp_argsArr, 1);
        	cActive[i] = 1;
        } else {
            command_path(argsArr, 0);
        }

        if (done == 1) {
            old_argsArr = copyTokens(argsArr, nToken, 7, 20);
            old_nToken = nToken;
            printf("\n");
            printf("YWIMC > ");
            argsArr = readTokens(7, 20, &nToken);
        }
    }

    //Free the memory space
    freeTokenArray(argsArr, 5);
    argsArr = NULL;
    printf("Goodbye!\n");
    return 0;
}
