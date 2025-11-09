import ctypes
import os
import random
import time
import math
import matplotlib.pyplot as plt

# ==============================================================================
# SCRIPT DE TIM SORT
# ==============================================================================
MIN_MERGE = 32

def calcMinRun(n):
    r = 0
    while n >= MIN_MERGE:
        r |= n & 1
        n >>= 1
    return n + r

def insertionSort(arr, left, right):
    for i in range(left + 1, right + 1):
        j = i
        while j > left and arr[j] < arr[j - 1]:
            arr[j], arr[j - 1] = arr[j - 1], arr[j]
            j -= 1

def merge(arr, l, m, r):
    len1, len2 = m - l + 1, r - m
    left, right = [], []
    for i in range(0, len1):
        left.append(arr[l + i])
    for i in range(0, len2):
        right.append(arr[m + 1 + i])
    i, j, k = 0, 0, l
    while i < len1 and j < len2:
        if left[i] <= right[j]:
            arr[k] = left[i]
            i += 1
        else:
            arr[k] = right[j]
            j += 1
        k += 1
    while i < len1:
        arr[k] = left[i]
        k += 1
        i += 1
    while j < len2:
        arr[k] = right[j]
        k += 1
        j += 1

def timSort(arr):
    n = len(arr)
    minRun = calcMinRun(n)
    for start in range(0, n, minRun):
        end = min(start + minRun - 1, n - 1)
        insertionSort(arr, start, end)
    size = minRun
    while size < n:
        for left in range(0, n, 2 * size):
            mid = min(n - 1, left + size - 1)
            right = min((left + 2 * size - 1), (n - 1))
            if mid < right:
                merge(arr, left, mid, right)
        size = 2 * size

# ==============================================================================
# CARGA Y CONFIGURACION DE LAS BIBLIOTECAS C
# ==============================================================================
# Determina la extension de la biblioteca segun el SO
lib_ext = '.dll' if os.name == 'nt' else '.so'

lib_path = os.path.dirname(os.path.abspath(__file__))
# --- Counting Sort ---
path1 = os.path.join(lib_path, f"counting_sort{lib_ext}")
counting_lib = ctypes.CDLL(path1)
counting_sort_c = counting_lib.sort_array
counting_sort_c.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int]
counting_sort_c.restype = None

# --- Radix Sort ---
path2 = os.path.join(lib_path, f"radix_queue{lib_ext}")
radix_lib = ctypes.CDLL(path2)
radix_sort_c = radix_lib.radix_sort_queue
radix_sort_c.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int]
radix_sort_c.restype = None

# --- Bucket Sort ---
path3 = os.path.join(lib_path, f"bucket_sort{lib_ext}")
bucket_lib = ctypes.CDLL(path3)
bucket_sort_c = bucket_lib.bucket_sort
bucket_sort_c.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.c_int]
bucket_sort_c.restype = None

# ==============================================================================
# FUNCIONES "WRAPPERS" PARA SIMPLIFICAR LAS LLAMADAS
# ==============================================================================
# Estas funciones manejan la conversion de tipos para que el bucle principal sea mas limpio.

def run_counting_sort(arr):
    n = len(arr)
    k = max(arr) if n > 0 else 0
    CArray = ctypes.c_int * n
    input_arr = CArray(*arr)
    output_arr = CArray()
    counting_sort_c(input_arr, output_arr, n, k)
    return list(output_arr)

def run_radix_sort(arr):
    n = len(arr)
    max_num = max(arr) if n > 0 else 0
    d = math.ceil(math.log10(max_num + 1)) if max_num >= 0 else 1
    if max_num == 0: d = 1
    CArray = ctypes.c_int * n
    c_arr = CArray(*arr)
    radix_sort_c(c_arr, n, d)
    return list(c_arr)

def run_bucket_sort(arr):
    n = len(arr)
    CArray = ctypes.c_double * n
    c_arr = CArray(*arr)
    bucket_sort_c(c_arr, n)
    return list(c_arr)

# ==============================================================================
# BUCLE PRINCIPAL DE PRUEBAS Y RECOLECCION DE DATOS
# ==============================================================================

def run_performance_tests():
    # Tamaños de arreglos a probar
    array_sizes = [1000, 5000, 10000, 25000, 50000, 100000]
    
    # --- EXPERIMENTO 1: DATOS ENTEROS ---
    print("--- Experimento 1: Ordenamiento de Enteros ---")
    int_results = {'TimSort (Python)': [], 'Counting Sort (C)': [], 'Radix Sort (C)': []}
    
    for size in array_sizes:
        # Generar datos aleatorios de enteros (0 a 10000)
        int_data = [random.randint(0, 10000) for _ in range(size)]
        print(f"\nProbando con {size} enteros...")

        # TimSort
        arr_copy = int_data.copy()
        start_time = time.perf_counter()
        timSort(arr_copy)
        duration = time.perf_counter() - start_time
        int_results['TimSort (Python)'].append(duration)
        print(f"  TimSort: {duration:.6f} segundos")

        # Counting Sort
        arr_copy = int_data.copy()
        start_time = time.perf_counter()
        run_counting_sort(arr_copy)
        duration = time.perf_counter() - start_time
        int_results['Counting Sort (C)'].append(duration)
        print(f"  Counting Sort: {duration:.6f} segundos")

        # Radix Sort
        arr_copy = int_data.copy()
        start_time = time.perf_counter()
        run_radix_sort(arr_copy)
        duration = time.perf_counter() - start_time
        int_results['Radix Sort (C)'].append(duration)
        print(f"  Radix Sort: {duration:.6f} segundos")

    # --- EXPERIMENTO 2: DATOS FLOTANTES ---
    print("\n--- Experimento 2: Ordenamiento de Flotantes [0,1) ---")
    float_results = {'TimSort (Python)': [], 'Bucket Sort (C)': []}
    
    for size in array_sizes:
        # Generar datos aleatorios de flotantes [0.0, 1.0)
        float_data = [random.random() for _ in range(size)]
        print(f"\nProbando con {size} flotantes...")
        
        # TimSort
        arr_copy = float_data.copy()
        start_time = time.perf_counter()
        timSort(arr_copy)
        duration = time.perf_counter() - start_time
        float_results['TimSort (Python)'].append(duration)
        print(f"  TimSort: {duration:.6f} segundos")

        # Bucket Sort
        arr_copy = float_data.copy()
        start_time = time.perf_counter()
        run_bucket_sort(arr_copy)
        duration = time.perf_counter() - start_time
        float_results['Bucket Sort (C)'].append(duration)
        print(f"  Bucket Sort: {duration:.6f} segundos")
        
    return array_sizes, int_results, float_results

# ==============================================================================
# SECCIÓN 5: GENERACION DE PLOTS
# ==============================================================================

def plot_results(sizes, results, title):
    plt.figure(figsize=(12, 7))
    for name, times in results.items():
        plt.plot(sizes, times, marker='o', linestyle='-', label=name)
    
    plt.xlabel("Tamaño del Arreglo (n)")
    plt.ylabel("Tiempo de Ejecución (segundos)")
    plt.title(title)
    plt.legend()
    plt.grid(True)
    plt.xscale('log') # Escala logaritmica para ver mejor los tamaños grandes
    plt.yscale('log') # Escala logaritmica para ver mejor las diferencias de tiempo
    plt.show()

# ==============================================================================
# SECCIÓN 6: EJECUCION PRINCIPAL
# ==============================================================================
if __name__ == "__main__":
    sizes, int_data, float_data = run_performance_tests()
    
    # Graficar resultados del experimento 1
    plot_results(sizes, int_data, "Rendimiento de Algoritmos de Ordenamiento (Enteros)")
    
    # Graficar resultados del experimento 2
    plot_results(sizes, float_data, "Rendimiento de Algoritmos de Ordenamiento (Flotantes [0,1))")