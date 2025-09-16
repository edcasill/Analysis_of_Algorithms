#include <stdio.h>
#include <stdlib.h>

#define MASK 0xFFFFFFFF

unsigned long long multiplication(unsigned int a, unsigned int b, int n, unsigned int in_mask)
{
     if (n == 1) {
        return a*b;
    } 
    int size = n / 2;
    unsigned int mask = in_mask >> size;
    unsigned int a_r = a & mask;
    unsigned int a_l = a >> size;
    unsigned int b_r = b & mask;
    unsigned int b_l = b >> size;
    unsigned long long temp1 = multiplication(a_l, b_l, size, mask);
    unsigned long long temp2 = multiplication(a_r, b_r, size, mask);
    unsigned long long temp3 = multiplication(a_l, b_r, size, mask);
    unsigned long long temp4 = multiplication(a_r, b_l, size, mask);
    return temp2 + ((temp3 + temp4) << size) + (temp1 << n);
}

int main() {
    unsigned int mask = MASK;
    printf("mask is: %d ", mask);
    int n = 32;
    unsigned int a = 4;
    unsigned int b = 5;
    unsigned long long result = multiplication(a, b, n, mask);
    printf("result a=%u, b=%u is %llu\n", a, b, result);
    return 0;
}
