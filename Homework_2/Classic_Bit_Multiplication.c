#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MASK 0xFFFFFFFF

unsigned long long XOR(unsigned long long a, unsigned long long b)
{
    unsigned long long carry;
    while (b != 0){
            carry = a & b;
            a = a ^ b;
            b = carry << 1;
        }
    return a;
}

unsigned long long multiplication(unsigned int a, unsigned int b, int n, unsigned int in_mask)
{
    if (n == 1) {
        unsigned long long result = 0;
        while (b > 0)
        {
            if (b & 1)
            {
                result = XOR(result, a);
            }
            a = a<<1;
            b = b>>1;
        }
        //return a*b;
        return result;
        
    }

    int size = n / 2;
    unsigned int mask = in_mask >> size;
    unsigned int a_r = a & mask;
    unsigned int a_l = a >> size;
    unsigned int b_r = b & mask;
    unsigned int b_l = b >> size;
    unsigned long long temp1 = multiplication(a_l, b_l, size, mask);
    unsigned long long temp2 = multiplication(a_r, b_r, size, mask);
    //unsigned long long temp3 = multiplication(a_l, b_r, size, mask);
    //unsigned long long temp4 = multiplication(a_r, b_l, size, mask);
    unsigned long long new_temp = multiplication(a_l + a_r, b_l + b_r, size, mask);
    unsigned int op = new_temp - temp1 - temp2;
    //return temp2 + ((temp3 + temp4) << size) + (temp1 << n);
    return temp2 + ((op) << size) + (temp1 << n);
}

int main() {
    struct timeval tv0, tv1;
    long elapsed_time;
    gettimeofday(&tv0, NULL);

    unsigned int mask = MASK;
    int n = 32;
    unsigned int a = 4;
    unsigned int b = 5;
    unsigned long long result = multiplication(a, b, n, mask);
    printf("result a=%u, b=%u is %llu\n", a, b, result);

    gettimeofday(&tv1, NULL);
    elapsed_time = (tv1.tv_sec - tv0.tv_sec)*1000000 + (tv1.tv_usec - tv0.tv_usec);
    printf("\n");
    printf("execution time: %ld microseconds\n", elapsed_time);
    printf("\n");
    return 0;
}
