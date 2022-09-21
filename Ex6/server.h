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
#define nullptr ((void*)0)
#define PORT "4200"
#define BACKLOG 10
typedef struct unic_fd {
    int stage;
    char txt[1024];
    int fd;
}unic_fd;
void* nopPoent(void* temp);
void* f_func(void* temp);
void* s_func(void* temp);
void* answer(void* temp); 
void* insertQ1(void* temp);
void* insertQ2(void* temp);
void* insertQ3(void* temp);