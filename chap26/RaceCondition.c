/*

ʵ�ֹ��ܣ�
�����������̣߳�����������������м��㣬�۲���ֵ��Ԥ�ڵ���ȴ�С���

*/

#include "Common.h"

int shared_count = 0;

void* thread_run(void* arg) {
    int* m_count = (int*) arg;
    printf("hello, world, tid == %ld \n", pthread_self());
    for(int i = 0; i < 1000; ++i) {
        // *m_count++; ע������ĺ���++�ͽ�����*�����ȼ�����
        (*m_count)++;
        shared_count++;
    }
}

int main(int argc, char** argv) {
    int main_count;

    pthread_t t1;
    pthread_t t2;

    pthread_create(&t1, NULL, &thread_run, &main_count);
    pthread_create(&t2, NULL, &thread_run, &main_count);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("shared_count == %d \n", shared_count);
    printf("main_count == %d \n", main_count);
}