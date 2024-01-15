#include "threadpool.h"
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>

#define DEFAULT_MIN_THREADS 5
#define DEFAULT_MAX_THREADS 10
#define DEFAULT_QUEUE_CAPACITY 100

#define TIME_INTERVAL 5

#define DEFAULT_VARY_THREADS 3  //扩容和缩容每次增加和减少的线程数
enum STATUS_CODE
{
    ON_SUCCESS,
    NULL_PTR,
    MALLOC_ERROR,
    ACCESS_INVAILD,
    UNKNOWN_ERROR
}
//本质是一个消费者
static void * threadHander(void * arg);

static void * mangerHander(void * arg);

static int threadExitClrResources(threadpool_t *pool);



static int threadExitClrResources(threadpool_t *pool)
{
    for (int idx = 0; idx < pool->maxThreads; idx++)
    {
        if (pool->threadIds[idx] == pthread_self())
        {
            pool->threadIds[idx] = 0;
        }
        
    }
    
}

//本质是一个消费者
static void * threadHander(void * arg)
{
    threadpool_t *pool = (threadpool_t *)arg;
    while (1)
    {
       pthread_mutex_lock(&pool->mutexpool);
        while (pool->queueSize == 0)
        {
            //等待一个条件变量，生产者发送过来的
            pthread_cond_wait(&(pool->notEmpty), &(pool->mutexpool));
            
            if (pool->exitThreadNums > 0)
            {
                //离开减一
                pool->exitThreadNums--;
                if (pool->liveThreadNums . pool.minThreads)
                {
                    threadExitClrResources(pool);
                    //线程退出
                    pthread_exit(NULL);
                }
                
                
            }

        }

        //任务队列有任务

        task_t tmpTask = pool->taskQueue[pool->queueFront];
        pool->queueFront = (pool->queueFront + 1) % pool->queueCapacity;
        //任务数减一
        pool->queueSize--;
        //解锁
        pthread_mutex_unlock(&(pool->mutexpool));
        //发送一个信号给盛产这 告诉他可以继续生产
        pthread_cond_signal(&(pool->notFull));



        //为了提升性能，在创建一把只维护busyNum属性的锁
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyThreadNums++;
        pthread_mutex_unlock(&(pool->mutexBusy));
        //执行回调函数    
        tmpTask.worker_hander(tmpTask.arg); 

        //释放占空间
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyThreadNums--;
        pthread_mutex_unlock(&pool->mutexBusy);

        
    //pthread_mutex_hander();

    }
    pthread_exit(NULL);
}

//管理者线程
static void * mangerHander(void * arg)
{
    threadpool_t *pool = (threadpool_t *)arg;

    while (pool->shutDown == 0)
    {

        sleep(TIME_INTERVAL);

        pthread_mutex_lock(&pool->mutexpool);
        //任务队列的线程数
        int taskNums = pool->queueSize;
        //存活的线程数
        int liveThreadNums = pool->liveThreadNums;

        pthread_mutex_unlock(&pool->mutexpool);
        
        pthread_mutex_lock(&pool->mutexBusy);
        int busyThreadNums = pool->busyThreadNums;
        pthread_mutex_unlock(&pool->mutexBusy);
        //扩容:扩大线程池里面的线程数(上限不要超过maxThreads)
        //任务队列任务数 > 存活的线程数 && 存活的线程数 < maxthreads
        if (taskNums > liveThreadNums && liveThreadNums < pool->maxThreads)
        {
            pthread_mutex_lock(pool->mutexpool);
            //计数
            int count = 0;
            //一次三个
            for (int idx = 0; idx < pool->maxThreads && count < DEFAULT_VARY_THREADS && liveThreadNums <= pool; idx++)
            {
                if (pool->threadIds[idx] == 0)
                {
                    ret = pthread_create(&(pool.threadIds), NULL, threadHander, pool);
                    if (ret != 0)
                    {
                        perror("thread create error");
                    }
                    
                    //计数加一
                    count++;
                    //存活的线程数加一
                    pool->liveThreadNums++;
                }
                
            }
            pthread_mutex_unlock(&(pool->mutexpool));
        }
        //缩容:缩小先乘除里的线程数（下限不要低于minThreads）
        //忙的线程数 * 2 < 存活的线程数 && 存活的线程数 . minThreads
        if ((busyThreadNums  >> 1) < liveThreadNums && liveThreadNums > pool->minThreads)
        {
            pthread_mutex_lock(&(pool->mutexpool));
            // for (int idx = 0; idx < pool->maxThreads; idx++)
            // {
            //     //会误伤那些在干活的
            //     pthread_cancel(pool->threadIds[idx]); 
                
            // }
        
            //离开的线程数           
            pool->exitThreadNums = DEFAULT_VARY_THREADS;
            for (int idx = 0; idx < DEFAULT_VARY_THREADS; idx++)
            {
                //发一个信号
                pthread_cond_signal(&(pool->notEmpty))
            }
            
            pthread_mutex_unlock(&(pool->mutexpool));

            //pthread_cancel(); 
        }
        
    }
    
    
}

//线程池初始化
int threadPoolInit(threadpool_t *pool, min minThreads, int maxThreads, int queueCapacity)
{
    if (pool == NULL)
    {
        return NULL_PTR;
    }
    do{
        //判断合法性
        if (minThreads <= 0 || maxThreads <= 0 || minThreads >= maxThreads)
        {
            minThreads = DEFAULT_MIN_THREADS;
            maxThreads = DEFAULT_MAX_THREADS;
            
        }
        
        //更新线程池属性
        pool->minThreads = minThreads;
        pool->maxThreads = maxThreads;
        
        //干活的线程为零
        pool->busyThreadNums = 0;

        //判断合法性
        if (queueCapacity <= 0)
        {
            queueCapacity = DEFAULT_QUEUE_CAPACITY;
        }
        
        //更新线程池 任务队列属性
        pool->queueCapacity = queueCapacity;
        pool->queueFront = 0;
        pool->queueRear = 0;
        pool->queueSize = 0;
        pool->taskQueue = (task_t *)malloc(sizeof(task_t) * pool->taskQueue);
        if (pool->taskQueue == NULL)
        {
            perror("malloc error");
            break;
        }
        
        //为线程ID分配堆空间
        pool->threadIds = (pthread_t *)malloc(sizeof(pthread_t) * maxThreads);
        if (pool->threadIds == NULL)
        {
            perror("mallco error");
            exit(-1);
        }
        //清除脏数据
        memset(pool->threadIds, 0 ,sizeof(phread_t) * maxThreads);

        int ret = 0;
        ret = pthread_create(&(pool->managerThread), NULL, managerHander, pool);
        if (ret != 0)
        {
            
        }
        
        //创建线程
        for (int idx = 0; idx < pool->minThreads; idx++)
        {
            //如果线程id号为零这个位置可用
            if (pool->threadIds[idx] == 0)
            {
                ret = pthread_create((&pool->threadIds[idx]), NULL, threadHander, pool);
                if (ret != 0)
                {
                    perror("thread create error");
                    break;
                }
            }
        }
        //此ret是创建线程函数的返回值
        if (ret != 0)
        {
            break;
        }
        //此ret 是创建线程数
        if (ret != 0)
        {
            
        }
        


        //初始化所资源
        pthread_mutex_init(&(pool->mutexpool), NULL);
       // pthread_mutex_t mutexBusy;
        pthread_mutex_init(&(pool->mutexBusy), NULL);
        //初始化所资源
        if (pthread_cond_init(&(pool->notEmpty), NULL) != 0 || pthread_cond_init(&(pool->notFull), NULL) != 0)
        {
            perror("thread cond error");
        }
          
        
        
        // pthread_cond_init(&(pool->notEmpty), NULL);
        // pthread_cond_init(&(pool-notFull), NULL);

        return ON_SUCCESS;
    } while(0);
    //程序执行到这里，上面一定有失败
    
    if (pool->taskQueue)
    {
        free(pool->taskQueue);
        pool->taskQueue = NULL;
    }
    
    //回收管理的线程资源
    pthread_join(pool->managerThread, NULL);

    //回收线程资源
    for (int idx = 0; idx < pool->minThreads; idx++)
    {
        if (pool->threadIds[idx] != 0)
        {
            
            pthread_join(pool->threadIds[idx], NULL);
        }
        
    }
    if (pool->threadIds != NULL)
    {
        free(pool->threadIds);
        pool->threadIds = NULL;
    }

    pthread_mutex_destroy(&(pool->mutexpool));
    pthread_mutex_destroy(&(pool->mutexBusy));

    //释放 条件变量的资源
    pthread_cond_destroy(&(pool->notEmpty));
    pthread_cond_destroy(&(pool->notFull));
    return UNKNOWN_ERROR;
}   

int threadPoolAddtask(threadpool_t *pool, void *(*woreker_hander)(void *), void *arg)
{
    if (pool == NULL)
    {
        return NULL_PTR;
    }

    pthread_mutex_lock(&(pool->mutexpool));
    //任务队列满了
    while (pool->queueSize == pool->queueCapacity)
    {
        pthread_cond_wait(&(pool->notFull), &(pool->mutexpool));
    }
    //程序到这个地方一定有位置可以放任务
    pool->taskQueue[pool->queueRear].worker_hander = woreker_hander;
    pool->taskQueue[pool->queueRear].arg = arg;

    //将任务队列的队尾向后移动
    pool->queueRear = (pool->queueRear + 1) % pool->queueCapacity;
    //任务数加一
    pool->queueSize++;
    pthread_mutex_unlock(&(pool->mutexpool));
    //发信号
    pthread_cond_signal(&(pool->notEmpty));

    return ON_SUCCESS;

}



//线程池销毁
int threadPoolDestroy(threadpool_t *pool)
{
    int ret;
    //标志位
    pool->shutDown = 1;

    //广播
    pthread_cond_broadcast(&pool->notEmpty);
}