/*

实现功能：
1. 从标准输入接收用户输入，并把输入的字符串通过socket发送给服务器端
2. 将服务器端的应答显示在标准输出上
3. 如果用户输入了"close",则会调用close函数关闭连接，此时会休眠一段时间，等待服务器端处理后再退出
4. 如果用户输入了"shutdown"，则会调用shutdown函数关闭连接的写方向，此时不会直接退出，而是继续等待服务器端的应答，直到服务器端完成自己的操作，在另一个方向上完成关闭。

（通过select进行多路复用，同时完成对连接socket和标准输入两个I/O对象的处理）

*/


#include"Common.h"


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        perror("ip error");
        exit(EXIT_FAILURE);
    }

    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in ser_addr;
    bzero(&ser_addr, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, argv[1], &ser_addr.sin_addr);

    if (connect(sockfd, (struct sockaddr*) &ser_addr, sizeof(ser_addr)) < 0)
    {
        perror("connect fail");
        exit(EXIT_FAILURE);
    }
    
    char send_line[MAXLINE];
    char recv_buf[BUFFER_SIZE];

    // fd_set 结构体
    fd_set readmask;
    fd_set allreads;

    FD_ZERO(&allreads);  // 清除fd_set的所有位
    FD_SET(0, &allreads); // 设置fd_set的位fd
    FD_SET(sockfd, &allreads);

    for (;;)
    {
        readmask = allreads;
        int r = select(sockfd + 1, &readmask, NULL, NULL, NULL); // 返回就绪（可读、可写、异常）文件描述符的总数
        if (r <= 0)
        {
            perror("select fail");
            exit(EXIT_FAILURE);
        }
        
        // 对连接socket进行处理
        // 读取服务器端发送的数据，并显示在标准输出上
        if (FD_ISSET(sockfd, &readmask))  // 测试fd_set的位fd是否被设置
        {
            int rt = read(sockfd, recv_buf, BUFFER_SIZE);
            if (rt < 0)
            {
                perror("read fail");
                exit(EXIT_FAILURE);
            }
            else if (rt == 0)  // read == 0 表示EOF，即对端发送了FIN包
            {
                perror("server close");
                exit(EXIT_FAILURE);
            }
            recv_buf[rt] = 0;
            fputs(recv_buf, stdout);
            fputs("\n", stdout);
            
        }

        // 对标准输入进行处理
        // 判断是正常字符串，还是 close 或 shutdown
        if (FD_ISSET(0, &readmask))
        {
            if (fgets(send_line, MAXLINE, stdin) != NULL)
            {
                // 对输入字符串进行判断
                if(strncmp(send_line, "shutdown", 8) == 0) // 如果输入的是"shutdown"，则关闭标准输入的I/O事件感知
                {
                    // 关闭写方向，进入 "半关闭状态"，清除fd_set的位fd
                    FD_CLR(0, &allreads);
                    if (shutdown(sockfd, 1)) // shutdown若成功会返回0，所以成功时不会进入if内部
                    {
                        perror("shutdown error");
                        exit(EXIT_FAILURE);
                    }
                    
                }
                else if(strncmp(send_line, "close", 5) == 0)
                {
                    FD_CLR(0, &allreads);
                    if (close(sockfd))
                    {
                        perror("close error");
                        exit(EXIT_FAILURE);
                    }
                    // 若close成功
                    // 休眠一段时间，等待服务器端处理后再退出
                    sleep(6);
                    exit(0);
                }
                else // 输入的是正常字符串，此时应该把输入的字符串发送给服务器端
                {
                    int st = strlen(send_line);  // strlen会计算换行符
                    if (send_line[st - 1] == '\n')  // 截掉回车符
                        send_line[st - 1] = 0;
                    
                    printf("now sending: %s \n", send_line);
                    size_t wt = write(sockfd, send_line, strlen(send_line));
                    if (wt < 0)
                    {
                        perror("write fail");
                        exit(EXIT_FAILURE);
                    }
                    printf("send bytes: %zu \n", wt);
                    
                }
                
            }
            
        }
        
        
    }
    
}
