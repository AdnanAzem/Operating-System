
#include "Queue.h" 
#include <pthread.h>
typedef struct active_object {
    void* (*f1)(void*);
    void* (*f2)(void*);
    queue* Q;
    pthread_t *p;
    int run;

}AO;
void* runAO(void* temp);
void destroyAO(AO *active_Obj);
AO *newAO(queue* Q, void* f1, void* f2);
