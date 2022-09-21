#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define S 10124
struct stack_tag
{
    char data[S];
    int tos;
};
//int fd = open("mmap.txt", O_RDWR);
stack stack_create(stack s)
{
   // s = (void*)mmap( NULL, sizeof(struct stack_tag), PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1, 0);
       
       
       
        
        
       
    
    if (s == NULL)
    {
        perror("Not correct\n");
    }
    s->tos = -1;
   // printf("si %d\n", s->tos);
}

void stack_destroy(stack *sptr)
{
    if (*sptr != NULL)
        free(*sptr);
    *sptr = NULL;
}

element_type stack_top(stack s)
{
    if (s->tos == 0)
    {
        return "Empty";
    };
    int i;
   
    for (i = (s->tos) - 2; s->data[i] != '\0' ;i--){
     if(i==0){
         break;
     }
     
     printf("i= %d\n", i);
     
    }
    if(i!=0){
        i++; 
     }
         
         // return to begin of top string
   
  
    char *top = (char *)malloc(sizeof(char) * 1024);
    strcpy(top, &(s->data[i]));
    return top;
}

element_type stack_pop(stack s)
{
    if (s->tos == 0)
    {
        //perror("Error! stack is empty\n");
       return "Empty";
    }
    int i;
    for (i = (s->tos) - 2; s->data[i] != '\0'; i--)
    {
       s->tos--; 
       if(i==0){
           s->tos--;
           break;
       }
       
    } // return to begin of top string
    if(i!=0){
         s->tos--; 
     }
     printf("si %d\n", s->tos); 
    char *pop = (char *)malloc(sizeof(char) * 1024);
    strcpy(pop, &(s->data[i]));
    return pop;
}

void stack_push(stack s, element_type e)
{

    int si = (int)strlen(e);
   // printf("si %d\n", si);
    for (int i = 0; i < si; i++)
    {
        s->data[s->tos] = e[i];
         //printf("s->tos %d\n", s->tos);
        // printf("tos  %d\n", s->tos);
       // printf("s->data[s->tos]=%c\n",  s->data[s->tos]);
        s->tos++;
        // int p=s->tos;
        
        
         ///printf("s->data[s->tos]=%c\n",  s->data[s->tos]);
    }

    s->data[s->tos++] = '\0';
     //printf("s->tos %d\n", s->tos);
    printf("%s pushed to stack\n", s->data);
}

int stack_size(stack s)
{
    return s->tos + 1;
}

int stack_is_empty(stack s)
{
    return s->tos == -1;
}