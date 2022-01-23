#include "Common.h"

char ROT13(char c) {
    if((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
        return c + 13;
    else if((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
        return c - 13;
    return c;
}

void echo(int fd) {
    char recv_buf[BUFFER_SIZE];
    bzero(recv_buf, BUFFER_SIZE);

    size_t readIndex = 0;
    while(1) {
        char ch;
        int rt = recv(fd, &ch, 1, 0);
        if(rt == 0) {
            break;
        }
        else if(rt < 0) {
            error(1, errno, "recv fail");
        }

        // ���� '\n' �����ͻ��˵�һ�������Ѿ����꣬��ʱ���Խ����������ݷ��͸��ͻ���
        if(ch == '\n') {
            int st = send(fd, recv_buf, readIndex, 0);
            if(st < 0) {
                error(1, errno, "send fail");
            }
            readIndex = 0; // ����recv_buf
            continue;
        }
        // �������'\n'����ô��ch���벢����recv_buf
        if(readIndex < BUFFER_SIZE)
            recv_buf[readIndex++] = ROT13(ch);  
    }
}