/*

实现功能：
1. 通过标准输入读取数据，并将该数据发给服务器端
2. 将从服务器端接收到数据，打印在标准输出上

*/

// 注意！与UDP socket 不同的是，本地数据报形式的socket的客户端程序，需要bind自己的本地路径，以便服务器回包时，可以正确找到地址
// 在UDP客户端程序里，服务器可以通过UDP包的本地地址和端口得到客户端的地址信息，所以 UDP 客户端不需要 bind 端口
#include"Common.h"

int main (int argc, char** argv)
{
    if (argc != 2)
    {
        perror("path error");
        exit(EXIT_FAILURE);
    }

    int sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }


    struct sockaddr_un cli_addr;
    bzero(&cli_addr, sizeof(cli_addr));
    cli_addr.sun_family = AF_LOCAL;
    strcpy(cli_addr.sun_path, tmpnam(NULL));

    
    if (bind(sockfd, (struct sockaddr*) &cli_addr, sizeof(cli_addr)) < 0)
    {
        perror("bind fail");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_un ser_addr;
    bzero(&ser_addr, sizeof(ser_addr));
    ser_addr.sun_family = AF_LOCAL;
    strcpy(ser_addr.sun_path, argv[1]);


    char send_line[MAXLINE];
    bzero(send_line, MAXLINE);
    char recv_buf[BUFFER_SIZE];

    while (fgets(send_line, MAXLINE, stdin) != NULL)
    {
        int n = strlen(send_line);
        if (send_line[n-1] == '\n')
        {
            send_line[n-1] = 0;
        }
        size_t nbytes = strlen(send_line);
        printf("now sending %s \n", send_line);

        if (sendto(sockfd, send_line, nbytes, 0, (struct sockaddr*) &ser_addr, sizeof(ser_addr)) != nbytes)
        {
            perror("send fail");
            exit(EXIT_FAILURE);
        }
        
        int l = recvfrom(sockfd, recv_buf, BUFFER_SIZE, 0, NULL, NULL); // 由于已知服务器端的地址信息，所以可以不用在这里捕获其地址
        recv_buf[l] = 0;

        fputs(recv_buf, stdout);
        fputs("\n", stdout);
    }
    
    exit(0);
}