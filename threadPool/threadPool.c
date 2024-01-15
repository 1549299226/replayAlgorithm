#include "threadpool.h"
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>

#define DEFAULT_MIN_THREADS 5
#define DEFAULT_MAX_THREADS 10

enum STATUS_CODE
{
    ON_SUCCESS,
    NULL_PTR,
    MALLOC_ERROR,
    ACCESS_INVAILD,
    UNKOWNN_ERROR
}

void 

int threadPoolInit(threadpool_t *pool, min minThreads, int maxThreads)
{
    if (pool == NULL)
    {
        return NULL_PTR;
    }
    do{
        //判断合法性
        if (minThreads < 0 || maxThreads < 0 || minThreads >= maxThreads)
        {
            minThreads = DEFAULT_MIN_THREADS;
            maxThreads = DEFAULT_MAX_THREADS;
            
        }
        
        //更新线程池属性
        pool->minThreads = minThreads;
        pool->maxThreads = maxThreads;

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
        //创建线程
        for (int idx = 0; idx < pool->minThreads; idx++)
        {
            //如果线程id号为零这个位置可用
            if (pool->threadIds[idx] == 0)
            {
                ret = pthread_create((&pool->threadIds[idx]), NULL, threadHander, NULL);
                if (ret != 0)
                {
                    perror("thread create error");
                    break;
                }
            }
        }
        return ON_SUCCESS;
    }while(0);
    //程序执行到这里，上面一定有失败
    
    //回收线程资源
    for (int idx = 0; idx < pool->minThreads; idx++)
    {
        if (pool->threadIds[idx] != 0)
        {
            
            pthread_join(pool->threadIds[idx]);
        }
        
    }
    if (pool->threadIds != NULL)
    {
        free(pool->threadIds);
        pool->threadIds = NULL;
    }
    return UNKOWNN_ERROR
}   



//线程池销毁
int threadPoolDestroy(threadpool_t *pool)
{

}