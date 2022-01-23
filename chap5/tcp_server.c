#include"Common.h"

/* �������˶�ȡ���ݵĳ��� */

// ssize_t read(int fd, void * buf, size_t count);
// read()��Ѳ���fd ��ָ���ļ�����count ���ֽڵ�buf ָ����ָ���ڴ���.
// �������ֵΪ0����ʾEOF����ʾ�Զ˷�����FIN������Ҫ�Ͽ�����


// ��socket�������ж�ȡsize���ֽ�
size_t read_n(int socketfd, void* buffer, size_t sz) {
    char* buffer_p = buffer;
    int length = sz;

    while(length > 0) {
        int result = read(socketfd, buffer_p, length);
        
        if(result < 0) {
            if(errno == EINTR)  // errno �����������Ĵ�����룬����һ����,��չ������һ�� int ���͵�����
                continue;  /* ���Ƿ������������������Ҫ�ٴε���read */
            else
                return -1; // ��ʾ���ִ���
        }
        else if(result == 0)
            break;

        length -= result;
        buffer_p += result; // ��Ҫ�ƶ�ָ�룬ָ���´ζ�ȡ��λ��
    }

    return (sz - length); // ��ȡ��EOF������ѭ���󣬷���ʵ�ʶ�ȡ��������
}


// �ͻ��� ���ϵ� �������ݣ���������ÿ��ȡһ������֮��������ߣ���ģ��ʵ��ҵ��������Ҫ��ʱ��
void read_data(int fd) {
    ssize_t n;
    char buffer[1024];

    int time = 0; 
    for(;;) {
        fprintf(stdout, "read 1K data\n");
        // ÿ�ζ�ȡ1024���ֽڣ���1K���ݣ�ֱ��������ȫ������ 
        // �ӿͻ���socket�ж�ȡ1024���ֽڵ�buffer�У�Ȼ���������ߣ���ʾ���ڴ�����Щ���ݣ���֮�������ȡ
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
    int listenfd, connectfd; // listenfd�Ǵ���accept�Ĳ�����connectfd��accept�ķ���ֵ
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0) {
        perror("make socket fail");
        exit(EXIT_FAILURE);
    }

    socklen_t clientlen;
    struct sockaddr_in client_addr, server_addr; // IPv4 socket ����
    // void bzero(void *s, int n);
    // bzero()�Ὣ���� s ��ָ���ڴ�����ǰ n ���ֽڣ�ȫ����Ϊ��ֵ��
    bzero(&server_addr, sizeof(server_addr)); 
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9819);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind������socket�͵�ַ����
    bind(listenfd, (struct sockaddr*) &server_addr, sizeof(server_addr));

    // listen������backlog����Ϊ1024
    listen(listenfd, 1024);

    // һֱѭ������ͬ�û�����
    for(;;) {
        clientlen = sizeof(client_addr);
        connectfd = accept(listenfd, (struct sockaddr*) &client_addr, &clientlen); // ע�����һ��������socklen_t*
        read_data(connectfd); 
        close(connectfd); // �ر�����socket �����Ǽ���socket
    }
}