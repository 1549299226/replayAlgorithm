#include <stdio.h>

#define BUFFER_SIZE 6

int swap(int *val1, int *val2)
{
    int temp = *val1;
    *val1 = *val2;
    *val2 = temp;
}

int printArray(int *array, int size)
{
    if (array == NULL)
    {
        return 0;
    }
    
    for (int idx = 0; idx < size; idx++)
    {
        printf("array[%d] = %d\n", idx, array[idx]);
    }
    
}


void bubbleSort01(int *array, int len)
{
     if (array == NULL)
    {
        return ;
    }

    for (int jdx = len; jdx > 0 ; jdx--)
    {
        for (int idx = 1; idx < jdx; idx++)
        {
            //后面的数小于前面的数交换
            if (array[idx] < array[idx - 1])
            {
                swap(&(array[idx]), &(array[idx - 1]));
            }
        
        }
    }
}

//优化版本一的冒泡排序
void bubbleSort02(int *array, int len)
{
     if (array == NULL)
    {
        return ;
    }
    //已经排好序了
    int sorted = 1;
    for (int jdx = len; jdx > 0 ; jdx--)
    {
        //数据排好序为一
        sorted = 1;
        for (int idx = 1; idx < jdx; idx++)
        {
            //后面的数小于前面的数交换
            if (array[idx] < array[idx - 1])
            {
                swap(&(array[idx]), &(array[idx - 1]));
                //数据没有排好序
                sorted = 0;
            }            
        }
        if (sorted == 1)
        {
            break;
        }
        
    }
}

//优化版本二的冒泡排序
void bubbleSort03(int *array, int len)
{
     if (array == NULL)
    {
        return ;
    }
    //已经排好序了
    int sorted = 1;
    int sortedIndex = 0;
    for (int jdx = len; jdx > 0 ; jdx--)
    {
        //数据排好序为一
        // sorted = 1;
        sortedIndex = 1;
        for (int idx = 1; idx < jdx; idx++)
        {
            //后面的数小于前面的数交换
            if (array[idx] < array[idx - 1])
            {
                swap(&(array[idx]), &(array[idx - 1]));
                //数据没有排好序
                // sorted = 0;
                sortedIndex = idx;
            }            
        }
        jdx = sortedIndex;
        // if (sorted == 1)
        // {
        //     break;
        // }
        
    }
}


int main()
{
    int array[BUFFER_SIZE] = {11,36,24,107,23,65};
    int len = sizeof(array) / sizeof(array[0]);

    bubbleSort01(array, len);
    printArray(array, len);
}