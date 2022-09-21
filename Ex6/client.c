
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#define PORT "4200"
#define MAXDATASIZE 1024
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
int main(int argc, char *argv[])
{
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
    int sockfd, length;
    char buf[1024];
    if (argc != 2) {
        exit(1);
    }
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        return 1;
    }
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("socket");
            continue;
        }
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("connect");
            continue;
        }
        break;
    }
    if (p == NULL) {
        return 2;
    }
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),s ,sizeof s);
    freeaddrinfo(servinfo);
    char str[2048] = {'\0'};
    char temp[2048] = {'\0'};
    scanf("%s", str);
    while (1)
    {
        printf("->");
        if(send(sockfd, str, strlen(str), 0) == -1){
            exit(1);
        } 
        bzero(str, 2048);
        if ((length = recv(sockfd, str,2048, 0)) == -1) {
            exit(1);
        }
        str[length] = '\0';
        for (int i = 0; i < strlen(str); i++)
        {
            printf("%c", str[i]);
        }
        printf("\n");
        bzero(str, 2048);
        scanf("%s", str);
    }
    close(sockfd);
    return 0;
}
