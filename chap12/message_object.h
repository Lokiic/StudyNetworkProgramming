
typedef struct
{
    u_int32_t type; // ��Ϣ����
    char data[1024]; // ����
}message_object;

#define PING 1
#define PONG 2
#define TYPE1 11
#define TYPE2 22
