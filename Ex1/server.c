#include "server.h"

int main(int argc, char *argv[]) {
    int sockfd, ret;
    struct sockaddr_in serverAddr;

    int newSocket;
    struct sockaddr_in newAddr;

    socklen_t addr_size;

    char buffer[SOCKET_BUFFER_SIZE];
    bzero(buffer, sizeof(buffer));
    pid_t childpid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("[-] Error in connection.\n");
        exit(1);
    }
    printf("[+] Server Socket is created.\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SOCKET_PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (ret < 0) {
        printf("[-] Error in binding.\n");
        exit(1);
    }
    printf("[+] Bind to port %d\n", SOCKET_PORT);

    if (listen(sockfd, 10) == 0) {
        printf("[+] Listening....\n");
    } else {
        printf("[-] Error in binding.\n");
    }

    while (1) {
        newSocket = accept(sockfd, (struct sockaddr *)&newAddr, &addr_size);
        if (newSocket < 0) {
            exit(1);
        }
        printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

        if ((childpid = fork()) == 0) {
            close(sockfd);

            while ((ret = recv(newSocket, buffer, SOCKET_BUFFER_SIZE - 1, 0)) > 0) {
                if (buffer[ret - 1] == '\n')
                    buffer[ret - 1] = '\0';
                fprintf(stdout, "%s\n", buffer);
                fflush(stdout);
                bzero(buffer, sizeof(buffer));
            }

            if (ret == 0) {
                printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
                fflush(stdout);
            } else if (ret == -1) {
                perror("recv failed");
            }
        }
    }

    close(newSocket);

    return 0;
}
