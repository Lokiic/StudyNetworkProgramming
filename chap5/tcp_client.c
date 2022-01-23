#include"Common.h"

#define MESSAGE_SIZE 10240000

// 客户端需要向服务器端发送数据
// 这里客户端发送的是一个长为 MESSAGE_SIZE 的字符串流
void send_data (int fd) 
{
    // 初始化生成一个长为 MESSAGE_SIZE 的字符串流
    char* mess;
    mess = malloc(MESSAGE_SIZE + 1);
    for (int i = 0; i < MESSAGE_SIZE; ++i) 
    {
        mess[i] = 'z';
    }
    mess[MESSAGE_SIZE] = '\0';

    const char* p;
    p = mess;
    size_t remain = strlen(mess); // 记录剩余数据的长度，长度==0时，结束发送
    while (remain)
    {
        int result = send(fd, p, remain, 0);  // 发送时，仅仅是将数据发送到发送缓冲区中，并不意味着连接对端已经收到所有的数据
        fprintf(stdout, "send to buffer %ld \n", result);
        if (result <= 0)
        {
            perror("send fail");
            exit(EXIT_FAILURE);
        }

        remain -= result;
        p += result;
    }
    
    return;
}


// ip地址通过参数的形式传给 main 函数
int main (int argc, char **argv) 
{
    if (argc != 2) // 没有传入ip地址，或传入参数过多
    {
        perror("ip error");
        exit(EXIT_FAILURE);
    }
    
    int client_sock;
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0)
    {
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }
    
    
    struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(9819); // 绑定服务器端口
    inet_pton(AF_INET, argv[1], &server.sin_addr);  // 将字符串类型的ip地址，转换成用于网络传输的数值格式

    int c = connect(client_sock, (struct sockaddr*) &server, sizeof(server));
    if (c < 0)
    {
        perror("connect fail");
        exit(EXIT_FAILURE);
    }
    
    send_data(client_sock);
    exit(0);

}