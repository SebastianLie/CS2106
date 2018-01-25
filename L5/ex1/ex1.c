/*************************************
* Lab 5 Exercise 1
* Name:
* Student No:
* Lab Group:
*************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fdIn,  fileSize;
    int *ip;
    char fileName[40], buffer[4];

    scanf("%s", fileName);

    //Open file for read only
    fdIn = open(  fileName, O_RDONLY );

    //TODO:Check for valid file
    if(fdIn < 0) {
	printf("File Name: Cannot Open\n");
	return 0;
    }

   //Program proper
    //Checking 32-bit vs 64-bit. Do not remove!
    printf("Integer size is %d bytes. Should be 4 bytes (32-bit).\n",
            sizeof(int));

    printf("File Name: ");

    if (sizeof(int) != 4){
        printf("64-bit environment detected!\n");
        printf("Please recompile with \"-m32\" flag\n");
        exit(1);
    }

 

    //TODO:Calculate the file size

    size_t data = lseek(fdIn, (size_t)0, SEEK_END);  
    fileSize = (int)(data);   //change this line to reflect actual file size
    printf("Size = %i bytes\n", fileSize);

    lseek(fdIn, (size_t)0, SEEK_SET);

    //TODO: Change the code below so that it reads until the end of file
    // Right now, this only read the first item.
    int bytesRead = read(fdIn, buffer, 4);
    while(bytesRead != 0) {
    	//This is how you can "convert" bytes into a single integer
    	ip = (int*) &buffer;
    	printf("%d\n", *ip);
	bytesRead = read(fdIn, buffer, 4); 
    }
    close( fdIn );  //good practice

    return 0;
}
