#ifndef __THREAD_POOL_H_
#define __THREAD_POOL_H_

#include <pthread.h>

typedef struct task_t
{
    //回调函数
    void *(*worker_hander)(void *arg);
    //参数
    void *arg;

} task_t;
//线程池结构体
typedef struct threadpool_t
{
    //任务队列
    task_t * taskQueue;
    //任务队列的容量
    int queueCapacity;
    //任务队列的任务数
    int queueSize;
    //任务队列的队头
    int queueFront;
    //任务队列的队尾
    int queueRear;

    //线程池中的线程
    pthread_t *threadIds;
    //最小的线程数
    int minThreads;
    //最大的线程数
    int maxThreads;

    //锁
    pthread_mutex_t mutexpool;

    //条件变量:任务队列有任务可消费
    pthread_cond_t notEmpty;

    //条件变量:任务队列有空位 可以继续放
    pthread_cond_t notFull;
    
}threadpool_t;

//线程池初始化
int threadPoolInit(threadpool_t *pool, min minThreads, int maxThreads);

//线程池销毁
int threadPoolDestroy(threadpool_t *pool);



#endif //__THREAD_POOL_H_