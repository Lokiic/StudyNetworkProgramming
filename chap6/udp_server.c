#include "Common.h"

/*

实现功能：
1. 服务器端统计收到的UDP数据报的个数 
2. 在响应"Ctrl + "C""时，退出，并打印服务器端收到的报文总数(通过signal函数实现)
3. 对于客户端发来的数据，服务器对每条数据加上 GO！的前缀

*/

/*

socket->bind->recvfrom->sendto

ssize_t recvfrom(int sockfd, void* buff, size_t nbytes, int flags, 
　　　　　　　　　　struct sockaddr* from, socklen_t* addrlen); 

ssize_t sendto(int sockfd, const void* buff, size_t nbytes, int flags,
                const struct sockaddr* to, socklen_t addrlen); 


*/

#define MAXLINE  4096

static int count; // 用于记录接收到的UDP数据报的个数

void handler_function(int para)  // 信号处理函数
{   
    printf("\n received %d datagrams \n", count);
    exit(0);
}

int main (int argc, char** argv)
{
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // UDP IPv4 socket
    if (sockfd < 0)
    {
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in ser_addr, cli_addr;
    bzero(&ser_addr, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ser_addr.sin_port = htons(SERVER_PORT);
    
    bind(sockfd, (struct sockaddr*) &ser_addr, sizeof(ser_addr));

    char message[MAXLINE];
    socklen_t cli_len = sizeof(cli_addr);
    count = 0;

    signal(SIGINT, handler_function);

    for (;;)
    {
        // 通过该函数可以得到对端发送方的地址和端口等信息，然后可以利用该信息，通过sendto函数，发送数据给对端
        int n = recvfrom(sockfd, message, MAXLINE, 0, (struct sockaddr*) &cli_addr, &cli_len); 
        message[n] = 0;
        printf("received %d bytes: %s \n", n, message);

        char send_line[MAXLINE];
        sprintf(send_line, "GO! %s", message); // 给接收到的字符串加上前缀，然后再发送给客户端

        sendto(sockfd, send_line, strlen(send_line), 0, (struct sockaddr *) &cli_addr, cli_len);
        
        ++count; // 表示接收到的UDP数据报个数+1


    }
    
    exit(0);
    
}