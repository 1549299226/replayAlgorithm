#include <stdio.h>

//选择排序：相较于冒泡排序，减少了交换次数
#define BUFFER_SIZE 6

int printArray(int *array, int size)
{
    for (int idx = 0; idx < size; idx++)
    {
        printf("array[%d] = %d\n", idx, array[idx]);
    }
}

int selectSort01(int *array, int len)
{
    int minVal = 0;
    int minIndex = 0;
    for (int pos = 0; pos <len; pos++)
    {
        minVal = array[pos];
        for (int idx = pos + 1; idx < len; idx++)
        {
            if (minVal > array[idx])
            {
                minVal = array[idx];
                minIndex = idx;
            }
            
        }

        if (array[pos] > minVal)
        {
            int temp = array[pos];
            array[pos] = minVal;
            array[minIndex] = temp;
        }
    }

}


int main()
{
    int array[BUFFER_SIZE] = {11,36,24,107,23,65};
    int len = sizeof(array) / sizeof(array[0]);

    selectSort01(array, len);
    printArray(array, len);

}