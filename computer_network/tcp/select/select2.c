#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>

#define BUFSIZE 30

void error_handling(char *message);

int main(int argc, char **argv)
{
    int serv_sock;
    struct sockaddr_in serv_addr;
    fd_set reads, temps;
    int fd_max;
    char message[BUFSIZE];
    int str_len;
    struct timeval timeout;
    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    
    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
        error_handling("bind() error");
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");
    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;
    while (1)
    {
        int fd, str_len;
        int clnt_sock;
        unsigned int clnt_len;
        struct sockaddr_in clnt_addr;
        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        if (select(fd_max + 1, &temps, 0, 0, &timeout) == -1)
            error_handling("select() error");
        for (fd = 0; fd < fd_max + 1; fd++)
        {
            if (FD_ISSET(fd, &temps))
            {
                if (fd == serv_sock)
                { /* connect request from a client*/
                    clnt_len = sizeof(clnt_addr);
                    clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr, &clnt_len);
                    FD_SET(clnt_sock, &reads);
                    if (fd_max < clnt_sock)
                        fd_max = clnt_sock;
                    printf("connected client : %d \n", clnt_sock);
                }
                else
                {
                    str_len = read(fd, message, BUFSIZE);
                    if (str_len == 0)
                    { /* connection close */
                        FD_CLR(fd, &reads);
                        close(fd);
                        printf("closed client: %d \n", fd);
                    }
                    else
                    {
                        write(fd, message, str_len);
                    }
                }
            } // if(FD_ISSET(fd, &temps))
        }     // for(fd=0; fd<fd_max+1; fd++)
    }         // while(1)
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}