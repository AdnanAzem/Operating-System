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
#include <netdb.h>
#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT "4200" 
#define nullptr ((void*)0)
typedef void *(*pfunc)(void *arg);

typedef struct Reactor
{
    int fileID;
    pthread_t threadID;
    pfunc func;
} reactor, *preactor;

typedef struct reqests
{
    int fileID;
    preactor reac;
} reqests, *preqests;

preactor newReactor();
void RemoveHandler(preactor reac, int fd_free);
void InstallHandler(preactor rc, pfunc newFunc, int file_des);


int fd_count,l;
struct pollfd *pfds;
char buf[1024];
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}
preactor newReactor()
{
    preactor res = (preactor)(malloc(sizeof(reactor)));
    return res;
}
void InstallHandler(preactor rc, pfunc newFunc, int file_des)
{
    rc->func = newFunc;
    rc->fileID = file_des;
    preqests rq = (preqests)(malloc(sizeof(reqests)));
    rq->fileID = file_des;
    rq->reac = rc;
    pthread_create(&rc->threadID, NULL, newFunc, rq);
}
void RemoveHandler(preactor reac, int fd_free)
{
    pthread_join(reac->threadID, NULL);
    reac->fileID = -1;
    reac->func = NULL;
}
int get_l_socket(void)
{
    int l;
    int yes = 1;
    int rv;
    struct addrinfo hints, *ai, *p;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0)
    {
        exit(1);
    }

    for (p = ai; p != NULL; p = p->ai_next)
    {
        l = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (l < 0)
        {
            continue;
        }
        setsockopt(l, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if (bind(l, p->ai_addr, p->ai_addrlen) < 0)
        {
            close(l);
            continue;
        }
        break;
    }
    freeaddrinfo(ai);
    if (p == NULL)
    {
        return -1;
    }
    if (listen(l, 10) == -1)
    {
        return -1;
    }
    return l;
}
void add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size)
{
    if (*fd_count == *fd_size)
    {
        *fd_size *= 2; // Double it

        *pfds = realloc(*pfds, sizeof(**pfds) * (*fd_size));
    }
    (*pfds)[*fd_count].fd = newfd;
    (*pfds)[*fd_count].events = POLLIN;
    (*fd_count)++;
}
void del_from_pfds(struct pollfd pfds[], int i, int *fd_count)
{
    pfds[i] = pfds[*fd_count - 1];
    (*fd_count)--;
}
void *myTreadFunc(void *arg)
{
    preqests req = &((preqests)arg)[0];
    int  newFd = req->fileID;
    char buf[1024]; // Buffer for client data
    for(;;)
    {
        int bytes = recv(newFd, buf, sizeof(buf), 0);
        if (bytes <= 0)
        {
            close(newFd);
            return NULL;
        }
        else
        {
            for (int i = 0; i < fd_count + 1; i++)
            {
                int client_fd = pfds[i].fd;
                if (client_fd != l && client_fd != newFd)
                {
                    send(client_fd, buf, bytes, 0);
                }
            }
            bzero(buf, 1024);
        }
    }
}
int main(void)
{
    int newfd;
    struct sockaddr_storage remoteaddr;
    socklen_t addrlen;
    char remoteIP[INET6_ADDRSTRLEN];
    fd_count = 0;
    int fd_size = 5;

    pfds = (struct pollfd *)malloc(sizeof(*pfds) * fd_size);
    l = get_l_socket();
    if (l == -1)
    {
        exit(1);
    }
    pfds[0].fd = l;
    pfds[0].events = POLLIN;

    fd_count = 1;

    printf("ready:\n");
    for (;;)
    {
        int poll_count = poll(pfds, fd_count, -1);
        if (poll_count == -1)
        {
            perror("poll");
            exit(1);
        }
        for (int i = 0; i < fd_count; i++)
        {
            if (pfds[i].revents & POLLIN)
            { 

                if (pfds[i].fd == l)
                {
                    addrlen = sizeof remoteaddr;
                    newfd = accept(l,
                                   (struct sockaddr *)&remoteaddr,
                                   &addrlen);

                    if (newfd == -1)
                    {
                        perror("accept");
                    }
                    else
                    {
                        add_to_pfds(&pfds, newfd, &fd_count, &fd_size);
                        preactor reac = (preactor)newReactor();
                        InstallHandler(reac, &myTreadFunc, newfd);
                    }
                }
                else
                {
                    int nbytes = recv(pfds[i].fd, buf, sizeof buf, 0);
                    int sender_fd = pfds[i].fd;
                    if (nbytes <= 0)
                    {
                        if (nbytes == 0)
                        {
                            printf(" socket %d closed\n", sender_fd);
                        }
                        else
                        {
                            perror("recv");
                        }
                        close(pfds[i].fd); // Bye!
                        del_from_pfds(pfds, i, &fd_count);
                    }
                    else
                    {
                        for (int j = 0; j < fd_count; j++)
                        {
                            int dest_fd = pfds[j].fd;
                            if (dest_fd != l && dest_fd != sender_fd)
                            {
                                if (send(dest_fd, buf, nbytes, 0) == -1)
                                {
                                    perror("send");
                                }
                            }
                        }
                    }
                } 
            }   
        }    
    }            
    return 0;
}