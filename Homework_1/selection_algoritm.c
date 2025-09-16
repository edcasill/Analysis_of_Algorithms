#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    //int unsorted[] = {3,1,5,2,4};
    int unsorted[] = {9,1,5,2,4,7,3}; //O(1)
    int data_length = sizeof(unsorted)/sizeof(unsorted[0]);//O(1)
    int minimum=0, mid_data, i, j;//O(4)

    printf("Your array is:");//O(0)/O(1)
    for (i = 0; i < data_length; i++)//O(n)
    {
        printf(" %d ", unsorted[i]);//O(0)/O(1)
    }
    printf("\n");//O(0)/O(1)
    printf("Your array has: %d, elements\n", data_length);//O(0)/O(1)
    
    for (i = 0; i < data_length; i++)//O(n)
    {
        minimum = i;
        for (j = i+1; j < data_length; j++)//O((n)) -> i*j
        {
            if (unsorted[j] < unsorted[minimum])// O(1)
            {
                minimum=j;//O(1)
            }
        }
        if (minimum != i)// O(1)
        {
            mid_data = unsorted[i];// O(1)
            unsorted[i] = unsorted[minimum];// O(1)
            unsorted[minimum] = mid_data;// O(1)
        }
    }
    printf("Your oredered array is:");// O(1)
    for (int i = 0; i < data_length; i++)// O(n)
    {
        printf(" %d ", unsorted[i]);// O(1)
    }
    printf("\n");// O(1)

    system("pause");// O(1)
}
