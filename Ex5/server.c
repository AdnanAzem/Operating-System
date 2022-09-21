#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/mman.h>
#include <pthread.h>
#include "stack.h"
#include <fcntl.h>

#define PORT "3512" // the port users will be connecting to
#define S 10124
#define BACKLOG 10 // how many pending connections queue will hold
#define MAXP 11
char client_message[2000];
char buffer[1024];
int new_fd[MAXP];
struct stack_tag
{
    char data[S];
    int tos;
};
struct flock flock;
int sockfd;
stack st = NULL;
int fd;

int tolock()
{
    fd = open("lock.txt", O_RDWR);
    if (fd < 0)
    {
        perror("Error!");
    }
    memset(&flock, 0, sizeof(flock)); // size
}
void *socketThread(void *arg)
{

    if (st == NULL)
    {
        perror("Not correct\n");
    }
    /// printf("%s","hgjh");
    int newSocket = *((int *)arg);
    // free(arg);
    sleep(1);
    char buff[1024], *strin;

    char *message;
    int nb;
    (flock).l_type = F_WRLCK; // lock
    fcntl(fd, F_SETLKW, &flock);
    //loking
    (flock).l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &flock);
    while (1)
    {

        nb = recv(newSocket, buff, 1024, 0);

        int c = 0;
        buff[nb] = '\0';
        // printf("server%sserver",buff);
        if (!strncmp(buff, "POP", 3))
        {
            c = 1;
        }
        if (!strncmp(buff, "PUSH ", 4))
        {
            // printf("ENTER bb\n");
            c = 2;
            /// printf("the client %d has left the server ",c);
        }
        if (!strncmp(buff, "TOP ", 3))
        {
            c = 3;
        }
        if (!strncmp(buff, "EXIT ", 4))
        {
            c = 4;
        }

        switch (c)
        {
        case 1:
            char *s;
            s = stack_pop(st);
            send(newSocket, s, strlen(s), 0);
            // printf("the client %d has left the server ",i);
            break;

        case 2:
            /// printf("ENTER kk\n");
            strin = buff + 5;
            stack_push(st, strin);
            // puts("client ");
            //  printf("the client %d has left the server ",i);
            break;
        case 3:
            char *s1;
            s1 = stack_top(st);
            // printf("%s",peek(&stack_s));
            send(newSocket, s1, strlen(s1), 0);
            // printf("the client %d has left the server ",i);
            break;

        case 4:
            // printf("the client %d has left the server ",i);
            break;
        default:
            // printf("the client %d has left the server ",i);
            break;
        }
        bzero(buffer, 1024);
    }

    close(newSocket);
    // pthread_exit(NULL);
}
void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;

    errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(void)
{
    tolock();

    st = (stack)mmap(NULL, sizeof(struct stack_tag), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    st->tos = 0;
    int fd2, *new_sock; // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    pid_t childpid;

    struct stat sb;
    //    fd = open ("stack_file.exe", O_RDWR);
    // stack_create(st);

    //     stack_s = (struct StackNode*)mmap(NULL,  sizeof(stack_s)*100333, PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS, fd, 0);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }
    pthread_t tid[60];

    printf("server: waiting for connections...\n");
    int i = 0;
    while (1)
    { // main accept() loop
        sin_size = sizeof their_addr;
        new_fd[i] = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd[i] == -1)
        {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);
        // printf("server: got connection from %s\n", s);
        // pid_t pidid=

        // Creates a child process
        if (fork() == 0)
        {
            close(sockfd);
            // Closing the server socket id
            printf("client  %d  in server  \n", i);
            socketThread(&new_fd[i]);

            return 1;
        }

        i++; /// close(*new_sock);
    }
    close(sockfd);
    // free(i);
    return 0;
}