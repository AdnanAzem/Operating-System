#ifndef COMMANDS_H_
#define COMMANDS_H_
#include <stdio.h>
#include <stdlib.h>



struct stack_tag;
typedef char* element_type;
typedef struct stack_tag *stack;

stack stack_create(stack s);

void stack_destroy(stack* s);

element_type stack_top(stack s);

element_type stack_pop(stack s);

void stack_push(stack s, element_type e);

int stack_size(stack s);

int stack_is_empty(stack s);

#endif





