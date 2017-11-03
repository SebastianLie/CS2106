/*************************************
* Lab 5 Exercise 2
* Name: LE TRUNG HIEU
* Student No: A0161308M
* Lab Group: 6
*************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int fdIn, option, nItems, nBytes, fileSize, bytesRead;
    int* intPtr;
    char* charPtr;
    char fileName[40], intBuffer[4], charBuffer[1];

    //Checking 32-bit vs 64-bit. Do not remove!
    printf("Integer size is %d bytes. Should be 4 bytes (32-bit).\n",
            sizeof(int));

    if (sizeof(int) != 4){
        printf("64-bit environment detected!\n");
        printf("Please recompile with \"-m32\" flag\n");
        exit(1);
    }


    //Program proper
    printf("File Name: ");
    scanf("%s", fileName);

    fdIn = open(  fileName, O_RDONLY );

    if(fdIn < 0) {
	   printf("Cannot Open \n");
	   return 0;
    }

    //TODO:Calculate the file size

    size_t data = lseek(fdIn, (size_t)0, SEEK_END);  
    fileSize = (int)(data);   //change this line to reflect actual file size
    printf("Size = %i bytes\n", fileSize);

    lseek(fdIn, (size_t)0, SEEK_SET);


	// int bytesRead = read(fdIn, buffer, 4);
 //    while(bytesRead != 0) {
 //    	//This is how you can "convert" bytes into a single integer
 //    	ip = (int*) &buffer;
 //    	printf("%d\n", *ip);
	// 	bytesRead = read(fdIn, buffer, 4); 
 //    }
    // Read and perform the specified options
    while( scanf("%i %i %i", &option, &nItems, &nBytes) == 3){
        //TODO: Handle the options here
    	if(option == 1) {
    		while(nItems > 0) {
    			if(nBytes == 4) {
    				bytesRead = read(fdIn, intBuffer, 4);
    				if(bytesRead <= 0) break;
    				intPtr = (int*) &intBuffer;
    				printf("%d ", *intPtr);
    			} else if(nBytes == 1) {
					bytesRead = read(fdIn, charBuffer, 1);
    				if(bytesRead <= 0) break;
    				charPtr = (char*) &charBuffer;
    				printf("%c ", *charPtr);
    			}
    			nItems--;
    		}
    		printf("\n");
    	} else if(option == 2) {
    		// printf("Request 2 \n");
    		off_t result = lseek(fdIn, (size_t)nBytes*nItems, SEEK_CUR);
    		// printf("%d \n", (int)result);
    		if((int)result >= fileSize) {
    			lseek(fdIn, (size_t)-nBytes*nItems, SEEK_CUR);
    			printf("not allowed\n"); 
    		} else {
    			printf("%d\n", (int)result);
    		}
    	} else if(option == 3) {
    		// printf("Request 3 \n");
    		off_t result = lseek(fdIn, (size_t)-nBytes*nItems, SEEK_CUR);
    		// printf("%d \n", (int)result);
    		if((int)result < 0) {
    			lseek(fdIn, (size_t)nBytes*nItems, SEEK_CUR);
    			printf("not allowed\n"); 
    		} else {
    			printf("%d\n", (int)result);
    		}
    	}
    }
    
    close( fdIn );  //good practice

    return 0;
}
