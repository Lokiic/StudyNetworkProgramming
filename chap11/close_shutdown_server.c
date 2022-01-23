/*

实现功能：
1. 打印出从客户端收到的数据
2. 将收到的数据进行格式化后，再发回给客户端
3. 对SIGPIPE信号进行处理（如果对端已经关闭了连接，此时再写入数据，会收到RST包，如果向已经收到RST的socket执行写操作，内核会发出一个SIGPIPE信号给进程）
4. 发送数据前，休眠一段时间，模拟服务器端的处理时间

*/

#include"Common.h"

static int count; // 记录客户端发送的报文数量

static void sig_handler(int para)
{
    printf("\n received %d datagram \n", count);
    exit(0);
}

int main(int argc, char** argv)
{
    int listenfd, connectfd;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in ser_addr;
    bzero(&ser_addr, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(SERVER_PORT);
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenfd, (struct sockaddr*) &ser_addr, sizeof(ser_addr)) < 0)
    {
        perror("bind fail");
        exit(EXIT_FAILURE);
    }

    if (listen(listenfd, 1024) < 0)
    {
        perror("listen fail");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    connectfd = accept(listenfd, (struct sockaddr*) &cli_addr, &cli_len);
    if (connectfd < 0)
    {
        perror("accept fail");
        exit(EXIT_FAILURE);
    }
    
    char recv_buf[BUFFER_SIZE];

    count = 0;

    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, sig_handler);
    for (;;)
    {
        bzero(recv_buf, BUFFER_SIZE);
        int n = read(connectfd, recv_buf, BUFFER_SIZE);
        if (n < 0)
        {
            perror("read fail");
            exit(EXIT_FAILURE);
        }
        else if (n == 0)
        {
            perror("client close");
            exit(EXIT_FAILURE);
        }

        recv_buf[n] = 0;
        ++count;
        printf("received %d bytes: %s \n", n, recv_buf);

        char send_line[MAXLINE];
        sprintf(send_line, "GO! %s", recv_buf);

        sleep(5); // 休眠一段时间，模拟服务器端的处理时间
        size_t wt = write(connectfd, send_line, strlen(send_line));
        if (wt < 0)
        {
            perror("write fail");
            exit(EXIT_FAILURE);
        }
        printf("send bytes: %zu \n", wt);
    }
    
}