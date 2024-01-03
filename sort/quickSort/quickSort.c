#include <stdio.h>
#define BUFFER_SIZE 6


int printArray(int *array, int size)
{
    for (int left = 0; left < size; left++)
    {
        printf("array[%d] = %d\n", left, array[left]);
    }
    
}
#if 0
int quickSort(int *array, int start, int end)
{
    if (start >= end)
    {
        return 0;
    }
    
    int left = start;
    int right = end;
    // 基准值
    int temp = array[start];
    while (left < right)
    {
        // 右侧
        while (left < right)
        {
            if (temp <= array[right])
            {
                right--;
            }
            else
            {
                array[left++] = array[right];
                break;
            }
        }

        // 左侧
        while (left < right)
        {
            if (temp >= array[left])
            {
                left++;
            }
            else 
            {
                array[right--] = array[left];
                break;
            }
        }
      
    }

    array[left] = temp;
    // 左侧
    quickSort(array, start, left - 1);
    // 右侧
    quickSort(array, right + 1, end);

    return 0;
    
}
#else
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
#endif
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
    quickSort02(array, start, baseValPos - 1);
    /* 对基准值右侧排序 */
    quickSort02(array, baseValPos + 1, end);

    return ret;
}

int main()
{
    int array[BUFFER_SIZE] = {11, 36, 24, 107, 23, 65};
    int len = sizeof(array) / sizeof(array[0]);

    quickSort02(array, 0, len );

    printArray(array, len);

    return 0;

}