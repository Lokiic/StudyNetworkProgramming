
typedef struct
{
    u_int32_t type; // 消息类型
    char data[1024]; // 数据
}message_object;

#define PING 1
#define PONG 2
#define TYPE1 11
#define TYPE2 22
