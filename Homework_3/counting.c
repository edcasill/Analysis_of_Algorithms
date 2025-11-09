#include <stdio.h>
#include <sys/time.h>
#include <windows.h>

void counting_sort(int A[], int n, int k, int B[]) {
    int C[k + 1];
    for (int i = 0; i <= k; i++)
        C[i] = 0;
    for (int j = 0; j < n; j++)
        C[A[j]]++;
    // C[i] now contains the number of elements equal to i.
    for (int i = 1; i <= k; i++)
        C[i] += C[i - 1];
    // C[i] now contains the number of elements less than or equal to i.
    // Copy A to B, starting from the end of A.
    for (int j = n - 1; j >= 0; j--) {
        B[C[A[j]] - 1] = A[j];
        C[A[j]]--; // to handle duplicate values
    }
}

// Export Windows library (DLL)
__declspec(dllexport) double counting_sort_c(int *A, int n, int k) {
    int B[n];
    struct timeval tv_start, tv_end;
    gettimeofday(&tv_start, NULL);
    counting_sort(A, n, k, B);
    gettimeofday(&tv_end, NULL);
    long ms = ((tv_end.tv_sec - tv_start.tv_sec) * 1000000L + (tv_end.tv_usec - tv_start.tv_usec)) / 1000;
    return ms;
}