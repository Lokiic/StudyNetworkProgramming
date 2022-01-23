#include"Common.h"

/* 服务器端读取数据的程序 */

// ssize_t read(int fd, void * buf, size_t count);
// read()会把参数fd 所指的文件传送count 个字节到buf 指针所指的内存中.
// 如果返回值为0，表示EOF，表示对端发送了FIN包，需要断开连接


// 从socket描述字中读取size个字节
size_t read_n(int socketfd, void* buffer, size_t sz) {
    char* buffer_p = buffer;
    int length = sz;

    while(length > 0) {
        int result = read(socketfd, buffer_p, length);
        
        if(result < 0) {
            if(errno == EINTR)  // errno 用来保存最后的错误代码，它是一个宏,被展开后是一个 int 类型的数据
                continue;  /* 考虑非阻塞的情况，这里需要再次调用read */
            else
                return -1; // 表示出现错误
        }
        else if(result == 0)
            break;

        length -= result;
        buffer_p += result; // 需要移动指针，指向下次读取的位置
    }

    return (sz - length); // 读取到EOF，跳出循环后，返回实际读取到的数据
}


// 客户端 不断地 发送数据，服务器端每读取一段数据之后进行休眠，以模拟实际业务处理所需要的时间
void read_data(int fd) {
    ssize_t n;
    char buffer[1024];

    int time = 0; 
    for(;;) {
        fprintf(stdout, "read 1K data\n");
        // 每次读取1024个字节，即1K数据，直到将数据全部读完 
        // 从客户端socket中读取1024个字节到buffer中，然后服务端休眠（表示正在处理这些数据），之后继续读取
        // n = read_n(fd, buffer, 1024); 
        if((n = read_n(fd, buffer, 1024)) == 0)
            return;
        
        ++time;
        fprintf(stdout, "1K data read for %d \n", time);
        usleep(10000000);
    }

}

int main(int argc, char** argv) {
    // socket->bind->listen->accept
    int listenfd, connectfd; // listenfd是传给accept的参数，connectfd是accept的返回值
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0) {
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }

    socklen_t clientlen;
    struct sockaddr_in client_addr, server_addr; // IPv4 socket 对象
    // void bzero(void *s, int n);
    // bzero()会将参数 s 所指的内存区域前 n 个字节，全部设为零值。
    bzero(&server_addr, sizeof(server_addr)); 
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9819);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind函数将socket和地址关联
    bind(listenfd, (struct sockaddr*) &server_addr, sizeof(server_addr));

    // listen函数的backlog设置为1024
    listen(listenfd, 1024);

    // 一直循环处理不同用户请求
    for(;;) {
        clientlen = sizeof(client_addr);
        connectfd = accept(listenfd, (struct sockaddr*) &client_addr, &clientlen); // 注意最后一个参数是socklen_t*
        read_data(connectfd); 
        close(connectfd); // 关闭连接socket 而不是监听socket
    }
}