#include <assert.h>
#include "stack.h"
#include <stdio.h>

#include <string.h>


int main() {
    struct StackNode *stack_s = NULL;//my stack_s
    
    char* t1 = "Denis";
    char* t2 = "Kirill";
    char* t3 = "Putin";
    char* t4 = "Zelensky";
    
    push(&stack_s, t1);
    char* t11 = peek(&stack_s);
    push(&stack_s, t2);
    char* t22 = peek(&stack_s);
    push(&stack_s, t3);
    char* t33 = peek(&stack_s);
    push(&stack_s, t4);
    char* t44 = peek(&stack_s);
    
    assert (!strncmp(t1, t11, 5));//compare Denis
    assert (!strncmp(t2, t22, 6));
    assert (!strncmp(t3, t33, 5));
    assert (!strncmp(t4, t44, 8));
} 
    
   
    
    

    
    
    
