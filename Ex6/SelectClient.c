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
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#define PORT "4200" 
#define SIZEE 30
int sockfd , con;
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
void *rec(void *arg)
{
    char buff[1024] = {0};
    int length = 0;
    while ((length = recv(sockfd, buff, 1024, 0)) != -1)
    {
        if (!length)
        {
            con = 0;
            break;
        }
        if (!strcmp(buff, "exit"))
        {
            con = 0;
            break;
        }
        bzero(buff, 1024);
    }
    return NULL;
}
void *sendFunc(void *arg)
{
    char input[1024] = {0};
    while (con != 0)
    {
        printf("SEND>");
        gets(input);
        if (strncmp(input,"exit",4) == 0)
        {
            send(sockfd,"exit",4,0);
            con = 0;
            break;
        }
        if (send(sockfd, input, strlen(input) + 1, 0) == -1)
        {
            perror("error");
        }
        bzero(input, 1024);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    char buf[SIZEE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr,"Client\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "Info : %s\n", gai_strerror(rv));
        return 1;
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            continue;
        }
        break;
    }
    if (p == NULL) {
        return 2;
    }
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    freeaddrinfo(servinfo);
    pthread_t p1,p2;
    con = 1;
    pthread_create(&p1, NULL, rec, NULL);
    pthread_create(&p2, NULL, sendFunc, NULL);
    pthread_join(p1, NULL);
    pthread_join(p1, NULL);
    pthread_kill(p2, 0);
    pthread_kill(p2, 0);
    close(sockfd);
    return 0;
}