#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void int_to_bin(int data, int bin_data[])
{
    /*
    This function create a mask to do a bit-bit operation, in order to get the binary representation
    of the given number.
    :return:
    bin_data -> binary number, storage as an integer on an array
    O(n+6)
    */
    int counter=0;
    unsigned int mask=1<<((sizeof(int)*8)-1);

    while(mask > 0) {
        bin_data[counter] = data&mask ? 1 : 0; //bit-bit operation to calculate de binary number
        counter ++;
        mask >>= 1; //move the 1 one position to the rigth in order to do the next operation
    }
}


int bin_to_dec(int a2_comp[], int dec_num){
    /*
    This function converts the binary number obtained after the a2 complement,
    to use the decimal number for next bit-bit opertion.
    i: position on a2_comp
    bin_count: weight of binary position, from 31 to 0
    a2_comp: absolute value of inserted number. 0 element is MSB
    dec_num: decimal number
    O(n+6)
    */
    int bin_count=1, mid=0;
    for (int i = 31; i >= 0; i--)//bin to dec converter
    {
        mid = a2_comp[i]*bin_count;
        dec_num = dec_num + mid;
        bin_count = bin_count*2;
    }
    return dec_num;
}


int main(int argc, char const *argv[])
{
    int in_number, bin_number[32], flag=0, a2_comp[32], dec_num=0, inv_dec=0, res, i;

    printf("Insert a positive integer number: ");
    scanf("%d", &in_number);
    
    int_to_bin(in_number, bin_number);//get the binary representation of the number
    //copy original data on new array to use it for a2 complement
    for (i = 0; i < 32; i++)
    {
        a2_comp[i] = bin_number[i];
    }
    //a2 complement
    if (bin_number[0] == 1)//check if the number is positive or negative
    {
        printf("your number is negative, taking absolute value\n");
        for (i = 31; i >= 0; i--)
        {
            if (flag==0)
            {
                a2_comp[i] = bin_number[i];
                if (bin_number[i] == 1)
                {
                    flag = 1;//take all the 0s from LSB until it founds the firs 1, then, get a1 complement
                }
            }
            else
            {
                a2_comp[i] = flag&bin_number[i] ? 0:1;
            }
        } 
    }
    
    for (i = 0; i < (sizeof(int)*8); i++)
    {
        printf("%d", a2_comp[i]);
    }

    dec_num = bin_to_dec(a2_comp, dec_num);//get decimal for bit-bit operation
    //printf("\n");
    //printf("integer number is: %d",dec_num);
    //convert the bin num to dec, in order to do bit-bit operation to invert the order

    while (dec_num>0)
    {
        /*
        Multiplies the inverted number by 10 (initializaed on 0), then add the module.
        Once it is done, take the int result from the 10 divition
        */
        res = dec_num%10;//get the last digit
        inv_dec = (inv_dec<<3) + (inv_dec<<1);//equivalent of *10
        inv_dec = inv_dec + res;//add last digit
        dec_num=dec_num/10;//get the rest of the number
    }
    printf("\n");
    printf("inverted num is: %d", inv_dec);

    printf("\n");
    system("pause");
}
