#include <stdio.h>


// #define BUFFER_SIZE 6

// int printArray(int *array, int size)
// {
//     for (int idx = 0; idx < size; idx++)
//     {
//         printf("array[%d] = %d\n", idx, array[idx]);
//     }
    
// }

// int main()
// {
//     int array[BUFFER_SIZE] = {11,36,24,107,23,65};
//     int len = sizeof(array) / sizeof(array[0]);

//     int pos = 0;
//     int copy = 0;

//     for (int jdx = 1; jdx < BUFFER_SIZE; jdx++)
//     {
//         for (int idx = 0; idx < BUFFER_SIZE; idx++)
//         {
//             pos = jdx;
//             copy = array[pos];
//             if (array[pos] > array[pos - 1])
//             {
//                 array[pos] = array[pos - 1];
//                 pos--;
//             }
//             array[pos - 1] = copy;
//         }
         
//     }
//     printArray(array, len);
// }





int printArray(int *array, int arraySize)
{
    int ret = 0;
    for (int idx = 0; idx < arraySize; idx++)
    {
        printf("array[%d]:%d\n", idx, array[idx]);
    }
    return 0;
}



int insertSort01(int *array, int length)
{
    int ret = 0;

    int num = 0;
    for (int pos = 1; pos < length; pos++)
    {
        num = array[pos];
        for (int idx = pos; idx >= 0; idx--)
        {
            if (array[idx - 1] > num)
            {
                array[idx] = array[idx - 1];
            }
            else
            {
                array[idx] = num;
                break;
            }
        }
    }
    return ret;
}

int main()
{
    int array[] = {11, 36, 24, 107, 23, 65, 1};
    int len = sizeof(array) / sizeof(array[0]);
    insertSort01(array, len);
    //insertSort(array, len);
    printArray(array, len);
    return 0;
}