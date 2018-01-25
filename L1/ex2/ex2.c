/*************************************
* Lab 1 Exercise 1
* Name: Le Trung Hieu
* Matric No: A0161308M
* Lab Group: 6
*************************************/

#include <stdio.h>
#include <stdlib.h> //for malloc() and free()

//Declaration of a Linked List Node

typedef struct NODE{
    int data;
    struct NODE* next;
} node;

//Function Prototypes
node* insertAt(node*, int, int, int);

void printList(node*);
void destroyList(node*);



int main()
{
    node* myList = NULL;    //Empty List
    int position, input, copies;

    //Fill in code for input and processing
    while(scanf("%i %i %i", &position, &input, &copies) == 3) {
    	myList = insertAt(myList, position, copies, input);
    }
    
    //Output code coded for you
    printf("My List:\n");
    printList(myList);
    
    destroyList(myList);
    myList = NULL;

    
    printf("My List After Destroy:\n");
    printList(myList);
 
    return 0;
}

//Actual Function Implementations
node* insertAt(node* head, int position, int copies, int newValue)
{
    //Fill in your code here
    if (copies == 0) return head;
    node* prev = NULL; node* current = head;
    int cnt = 0;
    while(cnt < position) {
	if (current == NULL) break;    
	cnt++;
        prev = current;
        current = current->next;
        if(cnt == 1) head->next = current;
    }
    node* newNode = (node*)malloc(sizeof(node));
    if (prev) prev->next = newNode;
    newNode->data = newValue;
    newNode->next = insertAt(current, 0, copies - 1, newValue);

    return position > 0 && head ? head : newNode;    //change this!
}
 
void printList(node* head)
//Purpose: Print out the linked list content
//Assumption: the list is properly null terminated
{
    //This function has been implemented for you
    node* ptr = head;

    while (ptr != NULL)  {    //or you can write while(ptr)
        printf("%i ", ptr->data);
        ptr = ptr->next;
    }
    printf("\n");
}

void destroyList(node* head)
{
     
    //Fill in your code here
    //You can use the same implementation as in exercise 1
    node* current = head;
    node* temp = head;

    while(temp) {
        current = temp;
   	temp = current->next;
        free(current);
   }
}
