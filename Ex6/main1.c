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
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include "Queue.h"
#include "Active_Object.h"
#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
pthread_t thread_id[BACKLOG];
bool on;
int new_fd;
int sockfd; 
queue* q1;
queue* q2;
queue* q3;
AO *first;
AO *second; 
AO *third;
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
void *clientThread(void *newfd) {
    int new_fd = *(int*)newfd;
    pthread_detach(pthread_self());
    char buf[1024];
    bool co = true;
    unic_fd *res = nullptr;
    while (co&&on) 
    {
        int length = recv(new_fd, buf, sizeof(buf), 0);
        if (length <=0) {
            perror("recv");
            co = 0;
            pthread_exit(NULL);
        }
        buf[length] = '\0';
        res = (unic_fd*)malloc(sizeof(unic_fd));
        res->fd = new_fd;
        memcpy(res->txt, buf, sizeof(buf));
        res ->stage = 0;
        insertQ1(res);
    }
    free(res);
    close(new_fd);
}
void sig_handler(int signum)
{
    if (signum == SIGINT) {
        on = 0;
        for (size_t i = 0; i < BACKLOG; i++)
        {
            close(new_fd);
        }
        close(sockfd);
        for (size_t i = 0; i < BACKLOG; i++)
        {
            pthread_cancel(thread_id[i]);
        }
        destroyAO(first);
        destroyAO(second);
        destroyAO(third);
        destroyQ(q1);
        destroyQ(q2);
        destroyQ(q3);
        exit(1);
    }
}
void* insertQ3(void* temp) 
{
    enQ(temp, q3);
    return temp;
}
void* insertQ2(void* temp) 
{
    enQ(temp, q2);
    return temp;
}
void* insertQ1(void* temp) 
{
    enQ(temp, q1);
    return temp;
}
void* nopPoent(void* temp) 
{
    return temp;
}
void* f_func(void* temp) 
{
    unic_fd* res = (unic_fd* )temp;
    int size = strlen(res->txt);
    for (size_t j = 0; j < size; j++)
    {
        if (res->txt[j]>='a'&&res->txt[j]<='z') {
            res->txt[j] = (res->txt[j]-96)%26+97;
        } 
        else if (res->txt[j]>='A'&&res->txt[j]<='Z') {
            res->txt[j] = (res->txt[j]-64)%26+65;
        }
    }
    return res;
}
void* s_func(void* temp) 
{
    unic_fd *res = (unic_fd*)temp;
    int size = strlen(res->txt);
    for (size_t j = 0; j < size; j++)
    {
        if (res->txt[j]>='a'&&res->txt[j]<='z') {
            res->txt[j] -= 32;
        } 
        else if (res->txt[j]>='A'&&res->txt[j]<='Z')
        {
            res->txt[j] += 32;
        }
    }
    return temp;
}
void* answer(void* temp) 
{
    unic_fd* res = (unic_fd*)temp;
    int new_fd = res->fd;
    if (send(new_fd, res->txt, 2048, 0) == -1)  
    {
        perror("error by send");
    }
    return temp;
}

int main(void)
{
    q1 = createQ();
    q2 = createQ();
    q3 = createQ();
    first = newAO(q1, f_func,insertQ2);
    second = newAO(q2, s_func , insertQ3);
    third = newAO(q3, nopPoent, answer);
    struct sockaddr_storage their_addr;
    struct addrinfo hints, *servinfo, *p;
    struct sigaction sa;
    socklen_t sin_size;
    int rv;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        return 1;
    }
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            continue;
        }
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1) {
            exit(1);
        }
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            continue;
        }
        break;
    }
    freeaddrinfo(servinfo);
    if (p == NULL)  {
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        exit(1);
    }
    int j = 0;
    signal (SIGINT,sig_handler);
    on = true;
        while(1) {
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            continue;
        }
        inet_ntop(their_addr.ss_family,
        get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        pthread_create(&thread_id[j%BACKLOG], NULL, clientThread, &new_fd);
        j++;
    }
    return 0;
}