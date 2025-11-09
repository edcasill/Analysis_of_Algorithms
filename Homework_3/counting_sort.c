#include <stdio.h>
#include <sys/time.h>
# include <windows.h>

// Hemos modificado la función para que reciba un arreglo de entrada (in_arr)
// y escriba el resultado en un arreglo de salida (out_arr).
// Esto es mucho más útil para una biblioteca.
void sort_array(int in_arr[], int out_arr[], int n, int k)
{
    int t_len = k + 1;
    int temp_array[t_len];

    // Inicializar el arreglo temporal
    for (int i = 0; i <= k; i++)
    {
        temp_array[i] = 0;
    }

    // Contar ocurrencias
    for (int j = 0; j < n; j++)
    {
        temp_array[in_arr[j]] = temp_array[in_arr[j]] + 1;
    }

    // Calcular posiciones acumuladas
    for (int i = 1; i <= k; i++)
    {
        temp_array[i] = temp_array[i] + temp_array[i-1];
    }

    // Construir el arreglo de salida ordenado
    for (int j = n - 1; j >= 0; j--)
    {
        out_arr[temp_array[in_arr[j]] - 1] = in_arr[j];
        temp_array[in_arr[j]] = temp_array[in_arr[j]] - 1;
    }
}
// Export Windows library (DLL)
__declspec(dllexport) double counting_sort_c(int *A, int n, int k) {
    int B[n];
    struct timeval tv_start, tv_end;
    gettimeofday(&tv_start, NULL);
    sort_array(A, B, n, k);
    gettimeofday(&tv_end, NULL);
    long ms = ((tv_end.tv_sec - tv_start.tv_sec) * 1000000L + (tv_end.tv_usec - tv_start.tv_usec)) / 1000;
    return ms;
}