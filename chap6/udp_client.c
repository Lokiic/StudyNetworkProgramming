#include"Common.h"
#define MAXLINE  4096

/*

实现功能：
1. 从标准输入中读取输入的字符串，并发送给服务器端
2. 把服务端经过处理的报文打印到标准输出上

*/

// socket->sendto->recvfrom

int main (int argc, char** argv)
{
    if (argc != 2)
    {
        perror("ip error");
        exit(EXIT_FAILURE);
    }

    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serv_adr;
    bzero(&serv_adr, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, argv[1], &serv_adr.sin_addr);

    char send_line[MAXLINE];
    char recv_line[MAXLINE + 1];

    // 为了避免recvfrom函数得到的对端地址信息覆盖了原serv_addr(实际上没有改变)，
    // 所以应该构建一个指向通用类型sockaddr的指针，存放对端的地址信息
    struct sockaddr* temp_addr;
    temp_addr = malloc(sizeof(serv_adr));
    socklen_t temp_len;

    while (fgets(send_line, MAXLINE, stdin) != NULL)
    {
        int n = strlen(send_line); // 由于fgets只能读取一行数据，所以需要判断读到数据的最后一个字符是否是换行符
        if (send_line[n-1] == '\n')
        {
            send_line[n-1] = 0;
        }
        
        printf("now sending %s \n", send_line);
        size_t send_t = sendto(sockfd, send_line, n, 0, (struct sockaddr*) &serv_adr, sizeof(serv_adr));
        if (send_t < 0)
        {
            perror("send fail");
            exit(EXIT_FAILURE);
        } 
        printf("send %zu bytes \n", send_t);

        temp_len = sizeof(serv_adr);
        size_t recv_t = recvfrom(sockfd, recv_line, MAXLINE, 0, (struct sockaddr*) &temp_addr, &temp_len);
        if (recv_t < 0)
        {
            perror("recvfrom fail");
            exit(EXIT_FAILURE);
        }

        recv_line[recv_t] = 0;
        fputs(recv_line, stdout);
        fputs("\n", stdout);
        
    }    

    exit(0);
}