#include<stdio.h>  // ��׼�������
#include<stdlib.h> // ��׼��ͷ�ļ�,size_t, malloc()��
#include<sys/socket.h>  // �ṩsocket���������ݽṹ
#include<netinet/in.h>  // �������ݽṹsockaddr_in

int make_socket (uint16_t port) {
    int sock; // socket ������
    sock = socket(AF_INET, SOCK_STREAM, 0); // IPv4 TCP socket
    if(sock < 0) {
        perror("socket fail"); // �൱��cerr
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in ipv4_sock; // ����IPv4 socket ����

    ipv4_sock.sin_family = AF_INET;
    ipv4_sock.sin_port = htons(port);

    // sin_addr��in_addr���͵Ķ��� 
    // ����ͨ���ַ
    ipv4_sock.sin_addr.s_addr = htonl(INADDR_ANY); 

    if(bind(sock, (struct sockaddr*) &ipv4_sock, sizeof(ipv4_sock)) < 0) { // ��Ҫ��socket��ʽת��Ϊͨ��socket��ʽ
        perror("bind fail");
        exit(EXIT_FAILURE);
    }

    return sock;
}

int main() {
    int sockfd = make_socket(9819);
    printf("success!");
    exit(0);
}