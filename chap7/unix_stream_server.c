/*
实现功能：
1. 接收客户端发送的字节流
2. 对接收到的字节流加上前缀 "GO!" 并发回给客户端
*/
#include "Common.h"

// 需要通过命令行输入，本地地址(绝对地址)，必须是一个本地文件的路径
// 字节流形式的本地socket 类似 TCP socket
int main (int argc, char** argv)
{
    if (argc != 2)
    {
        perror("path error");
        exit(EXIT_FAILURE);
    }

    int listenfd, connectfd;
    listenfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }
    
    char* localpath = argv[1];
    // 为了保持幂等性，需要把本地地址存在的文件删除
    // (这样每次运行服务器程序时，都会先把文件删除，然后由bind创建新文件，不管做多少次操作，都是一样的结果)
    unlink(localpath);

    struct sockaddr_un ser_addr, cli_addr;
    socklen_t cli_len;
    cli_len = sizeof(cli_addr);

    bzero(&ser_addr, sizeof(ser_addr));
    ser_addr.sun_family = AF_LOCAL;
    strcpy(ser_addr.sun_path, localpath);

    if (bind(listenfd, (struct sockaddr*) & ser_addr, sizeof(ser_addr)) < 0)
    {
        perror("bind fail");
        exit(EXIT_FAILURE);
    }   

    if (listen(listenfd, LISTEN_BACKLOG) < 0)
    {
        perror("listen fail");
        exit(EXIT_FAILURE);
    }
    
    connectfd = accept(listenfd, (struct sockaddr*) &cli_addr, &cli_len);
    if (connectfd < 0)
    {
        perror("accept fail");
        exit(EXIT_FAILURE);
    }
    

    char read_buf[BUFFER_SIZE];

    for (;;)
    {
        // 每次进行一轮循环前，需要将read_buf清空
        bzero(read_buf, sizeof(read_buf));
        if (read(connectfd, read_buf, BUFFER_SIZE) == 0)
        {
            printf("client quit");
            break;
        }
        printf("Received: %s \n", read_buf);

        char send_line[MAXLINE];
        sprintf(send_line, "GO!  %s", read_buf);

        int n = sizeof(send_line);
        if (write(connectfd, send_line, n) != n)
        {
            perror("write error");
            exit(EXIT_FAILURE);
        }
        
    }

    close(connectfd);
    close(listenfd);

    exit(0);
    

}   
