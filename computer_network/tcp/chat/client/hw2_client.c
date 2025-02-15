#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
#define NAME_SIZE 100

void error_handling(char *message);
void read_routine(int sock, char *buf);
void write_routine(int sock, char *buf);

char name[NAME_SIZE] = "[NULL]";
char buf[BUF_SIZE];
int name_send = 0;

int main(int argc, char *argv[])
{
    int sock;
    pid_t pid;
    char buf[BUF_SIZE];
    struct sockaddr_in serv_adr;

    if (argc != 4)
    {
        printf("Usage : %s <Server IP> <port> <name>\n", argv[0]);
        exit(1);
    }
    sprintf(name, "[%s]", argv[3]);

    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error!");

    pid = fork();
    if (pid == 0)
        read_routine(sock, buf);
    else
        write_routine(sock, buf);

    close(sock);
    return 0;
}
void read_routine(int sock, char *buf)
{
    char total_msg[NAME_SIZE + BUF_SIZE];
    while (1)
    {

        int str_len = read(sock, total_msg, NAME_SIZE + BUF_SIZE);

        if (str_len == 0)
            return;

        total_msg[str_len] = '\0';
        printf("%s\n", total_msg);
    }
}
void write_routine(int sock, char *buf)
{
    char total_msg[NAME_SIZE + BUF_SIZE];
    while (1)
    {
        if (name_send == 0)
        {
            sprintf(total_msg, "name %s", name);
            write(sock, total_msg, strlen(total_msg));
            bzero(buf, BUF_SIZE);
            name_send = 1;
            int str_len = read(sock, total_msg, NAME_SIZE + BUF_SIZE);
            total_msg[str_len] = '\0';
            printf("%s\n", total_msg);
        }

        fgets(buf, BUF_SIZE, stdin);

        if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))
        {
            shutdown(sock, SHUT_WR);
            return;
        }

        sprintf(total_msg, "%s", buf);

        write(sock, total_msg, strlen(total_msg));
    }
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
