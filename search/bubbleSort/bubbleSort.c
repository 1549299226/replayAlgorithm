#include <stdio.h>
#include <stdlib.h>



#define BUFFER_SIZE 10

/* 找到基准值的位置 */
static int findBaseValPos(int *array, int start, int end)
{
    int ret = 0;
    end--;
    /* 基准值 */
    int baseValPos = array[start];
    while (start < end)
    {
        while (start < end)
        {
            if (array[end] < baseValPos)
            {
                array[start++] = array[end];
                break;
            }
            else
            {
                end--;
            }
        }

        while (start < end)
        {
            if (array[start] > baseValPos)
            {
                array[end--] = array[start];
                break;
            }
            else
            {
                start++;
            }
        }    
    }   // 退出条件是：start = end.
    array[start] = baseValPos;
    return start;   
}   

/* 快速排序 */
int quickSort02(int *array, int start, int end)
{
    int ret = 0;
    if (array == NULL)
    {
        return ret;
    }
    /* 如果start == end 说明数组只有一个元素. 直接返回. */
    if (start >= end)
    {
        return ret;
    }
    int baseValPos = findBaseValPos(array, start, end);

    /* 对基准值左侧排序 */
    quickSort02(array, start, baseValPos);
    /* 对基准值右侧排序 */
    quickSort02(array, baseValPos + 1, end);

    return ret;
}


int printArray(int *array, int size)
{
    if (array == NULL)
    {
        return 0;
    }
    
    for (int idx = 0; idx < size; idx++)
    {
        printf("buffer[%d] = %d\n", idx, array[idx]);
    }
    
}

int binarySearchIsExist(int *array, int len, int val)
{
    if (array == NULL)
    {
        return -1;
    }    
    int begin = 0;
    int end = len - 1;
    int mid = mid = (begin + end) >> 1;
       
    while (end >= begin)
    {
        if (array[mid] > val)
        {
            end = mid - 1;
            mid = (begin + end) >> 1;
        }
        else if (array[mid] < val)
        {
            begin = mid + 1;
            mid = (begin + end) >> 1;
        }
        else
        {
            return mid;
        }
           
    }  
    return -1;
}

int main()
{
    int buffer[BUFFER_SIZE] = {0};
    int *pos = NULL;
    int val = 100;
    for (int idx = 0; idx < BUFFER_SIZE; idx++)
    {
        buffer[idx] = rand() % 100 + 1;
    }
    int len = sizeof(buffer) / sizeof(buffer[0]);
    printArray(buffer, BUFFER_SIZE);
    printf("\n");
    quickSort02(buffer, 0, len);
    printArray(buffer, BUFFER_SIZE);

    int isExist = binarySearchIsExist(buffer, len, val);
    printf("isExist = %d\n",isExist);
    return 0;
}