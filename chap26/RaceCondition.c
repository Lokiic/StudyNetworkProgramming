/*

实现功能：
创建两个子线程，对两个共享变量进行计算，观察结果值和预期的相比大小如何

*/

#include "Common.h"

int shared_count = 0;

void* thread_run(void* arg) {
    int* m_count = (int*) arg;
    printf("hello, world, tid == %ld \n", pthread_self());
    for(int i = 0; i < 1000; ++i) {
        // *m_count++; 注意这里的后置++和解引用*的优先级问题
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