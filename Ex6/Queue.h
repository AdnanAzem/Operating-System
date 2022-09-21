#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include "Queue.h"
#define nullptr ((void*)0)
struct node {
    void* value;
    struct node *next;
    struct node *prev;
};
typedef struct queue {
    struct node *front;
    struct node *rear;
    int size;
    pthread_mutex_t mut;
    pthread_cond_t deQ_Wait;
    void* lastVal;
}queue;
queue* createQ();
bool enQ(void*, queue*);
void* deQ(queue*);
void destroyQ(queue*);
