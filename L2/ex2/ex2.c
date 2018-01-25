/*************************************
* Lab 2 Exercise 2
* Name: Le Trung Hieu
* Student No: A0161308M
* Lab Group: Lab 6
*************************************
Warning: Make sure your code works on
lab machine (Linux on x86)
*************************************/

#include <stdio.h>
#include <string.h>     //For string comparison, string copy
#include <fcntl.h>      //For stat()
#include <sys/types.h>   
#include <sys/stat.h>
#include <unistd.h>     //for fork()
#include <sys/wait.h>   //for wait()

void commandPath(char* path) {
    struct stat fileStat;
    if(stat(path, &fileStat) < 0) {
    	printf("%s not found \n", path);
    } else {
    	int pid = fork();
        if (pid == 0){
	    // child exec command
       	     execl("/bin/sh", "sh","-c", path, NULL);
     	} else {
	    //wait for the child
             wait(NULL);
     	}
    }	
}

int main()
{
    char path[20];
    char lastPath[20];

    //read user input
    printf("YWIMC > ");
    scanf("%s", path);
    while (strcmp( path, "quit") != 0){ 
        // Check whether file exist
        // In real interpreter, a lot more checks are needed
        // E.g. check for file type, execution right etc

		
	if(strcmp( path, "last") == 0) {
		strcpy(path, lastPath);
		continue;
	}

	commandPath(path);

	strcpy(lastPath, path);
        printf("YWIMC > ");
        scanf("%s", path);
    }
   
    printf("Goodbye!\n");
    return 0;
}
