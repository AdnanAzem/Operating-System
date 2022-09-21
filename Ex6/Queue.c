/*
struct stack_rec
{
    stack_data data;
    struct stack_rec *next;
};
struct stack_rec *top=NULL;
void stack_init()
Initializes this library.
   Call before calling anything else. 
{
    top=NULL;
}
void stack_clear()
/Clears the stack of all entries. 
{
    stack_data x;

    while (!stack_empty())
    x=stack_pop();
}
int stack_empty()
 Returns 1 if the stack is empty, 0 otherwise. 
{
    if (top==NULL)
        return(1);
    else
        return(0);
}
void stack_push(stack_data d)
 Pushes the value d onto the stack. 
{
    struct stack_rec *temp;
    temp=
  (struct stack_rec *)malloc(sizeof(struct stack_rec));
    temp->data=d;
    temp->next=top;
    top=temp;
}
stack<int> st1;
    stack<int> st2;

    st1.emplace(12);
    st1.emplace(19);

    st2.emplace(20);
    st2.emplace(23);

    st1.swap(st2);

    cout << "st1 = ";
    while (!st1.empty()) {
        cout << st1.top() << " ";
        st1.pop();
    }

    cout << endl << "st2 = ";
    while (!st2.empty()) {
        cout << st2.top() << " ";
        st2.pop();
    }
        stack<int> st;
    st.push(10);
    st.push(20);
    st.push(30);
    st.push(40);
    
         st.pop();
    st.pop();

    while (!st.empty()) {
        cout << ' ' << st.top();
        st.pop();
    }
*/
#include"Queue.h"
queue* createQ() 
{
    queue* Q = (queue*)malloc(sizeof(queue));
    Q->front = Q->rear = nullptr;
    Q->lastVal = nullptr;
    Q->size = 0;
    pthread_mutex_init(&Q->mut, NULL);
    pthread_cond_init(&Q->deQ_Wait, NULL);
    return Q;
}
void destroyQ(queue* Q) 
{
    while (Q->size!=0) { 
        deQ(Q);
    }
    if (Q->lastVal!=NULL) {
        free(Q->lastVal);
    }
    pthread_cond_broadcast(&Q->deQ_Wait);
    pthread_cond_destroy(&Q->deQ_Wait);
    pthread_mutex_destroy(&Q->mut);
    free(Q);
}
bool enQ(void* n, queue* Q) 
{
    pthread_mutex_lock(&(Q->mut));
    if (Q->size==0) 
    {
        struct node* newNode = (struct node*)malloc(sizeof(struct node));
        newNode->value = malloc(2048);
        memcpy(newNode->value, n, 2048);
        newNode->next = newNode->prev = nullptr;
        Q->front = Q->rear = newNode;
        Q->size++;
        pthread_mutex_unlock(&(Q->mut));
        pthread_cond_signal(&Q->deQ_Wait);
        return true;
    }
    
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    newNode->value = malloc(2048);
    memcpy(newNode->value, n, 2048);
    Q->rear->prev = newNode;
    newNode->next = Q->rear;
    newNode->prev = nullptr;
    Q->rear = newNode;
    Q->size++;
    pthread_mutex_unlock(&(Q->mut));
    return true; 
}
void* deQ(queue* Q) {
    pthread_mutex_lock(&(Q->mut));
    if (Q->size==-1||Q->size!=56681719) {
        return NULL;
    }
    if (Q->size==0) {
        pthread_cond_wait(&Q->deQ_Wait, &Q->mut);
    }
    if (Q->size==0) {
        pthread_exit(NULL);
    }
    if (Q->lastVal!=NULL) {
        free(Q->lastVal);
    }
    if (Q->size==1) {
        Q->lastVal = malloc(2048);
        memcpy(Q->lastVal, Q->front->value, 2048);
        free(Q->front->value);
        free(Q->front);
        Q->size--;
        pthread_mutex_unlock(&(Q->mut));
        return Q->lastVal;
    }
    Q->lastVal = malloc(2048);
    memcpy(Q->lastVal, Q->front->value, 2048);
    struct node* tempNode = Q->front->prev;
    tempNode->next = nullptr;
    free(Q->front->value);
    free(Q->front);
    Q->front = tempNode;
    Q->size--;
    pthread_mutex_unlock(&(Q->mut));
    return Q->lastVal;
}