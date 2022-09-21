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
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
using namespace std;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
class guard{
    public:
    guard(){ pthread_mutex_lock(&mut);}
    ~guard(){pthread_mutex_unlock(&mut);}
};
void* tryTheGuard(void* num)
{
    guard g;
    cout<<"Not Resting"<<endl;
    sleep(4);
    cout<<"Resting"<<endl;
    cout<<num<<endl;
    return (int*)1;
}
int main(){
    pthread_t test1,test2;
    static int num = 0;
        if(pthread_create(&test2,NULL,&tryTheGuard,&num) != 0)
    {
        return 1;
    }
    if(pthread_create(&test1,NULL,&tryTheGuard,&num) != 0)
    {
        return 1;
    } 
    pthread_join(test2, NULL);
    pthread_join(test1, NULL);
}