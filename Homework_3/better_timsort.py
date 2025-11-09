import time
import random
import ctypes
import matplotlib.pyplot as plt
import numpy as np
from typing import List, Callable
import sys
import os

# =============================================================================
# TIM SORT (Tu implementación)
# =============================================================================

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

# =============================================================================
# CARGA DE DLLs DE C
# =============================================================================

def load_c_dlls():
    """Carga las DLLs de los algoritmos en C"""
    lib_path = os.path.dirname(os.path.abspath(__file__))
    path1 = os.path.join(lib_path, "counting_sort.dll")
    path2 = os.path.join(lib_path, "radix_queue.dll")
    path3 = os.path.join(lib_path, "bucket_sort.dll")
    try:
        # Counting Sort DLL
        counting_dll = ctypes.CDLL(path1)
        counting_dll.sort_array.argtypes = [
            ctypes.POINTER(ctypes.c_int),  # in_arr
            ctypes.POINTER(ctypes.c_int),  # out_arr  
            ctypes.c_int,                   # n
            ctypes.c_int                    # k
        ]
        counting_dll.sort_array.restype = None
        
        # Radix Sort DLL
        radix_dll = ctypes.CDLL(path2)
        radix_dll.radix_sort_queue.argtypes = [
            ctypes.POINTER(ctypes.c_int),  # A
            ctypes.c_int,                   # n
            ctypes.c_int                    # d
        ]
        radix_dll.radix_sort_queue.restype = None
        
        # Bucket Sort DLL (para doubles)
        bucket_dll = ctypes.CDLL(path3)
        bucket_dll.bucket_sort.argtypes = [
            ctypes.POINTER(ctypes.c_double),  # A
            ctypes.c_int                       # n
        ]
        bucket_dll.bucket_sort.restype = None
        
        return counting_dll, radix_dll, bucket_dll
        
    except Exception as e:
        print(f"Error cargando DLLs: {e}")
        print("Asegúrate de que los archivos .dll estén en el mismo directorio")
        return None, None, None

# =============================================================================
# WRAPPERS PARA LOS ALGORITMOS EN C
# =============================================================================

def counting_sort_wrapper(dll, arr, k=None):
    """Wrapper para Counting Sort en C"""
    if k is None:
        k = max(arr) if arr else 0
    
    n = len(arr)
    in_arr = (ctypes.c_int * n)(*arr)
    out_arr = (ctypes.c_int * n)()
    
    dll.sort_array(in_arr, out_arr, n, k)
    
    return list(out_arr)

def radix_sort_wrapper(dll, arr, d=None):
    """Wrapper para Radix Sort en C"""
    if d is None:
        max_val = max(abs(x) for x in arr) if arr else 0
        d = len(str(max_val))
    
    n = len(arr)
    arr_copy = (ctypes.c_int * n)(*arr)
    
    dll.radix_sort_queue(arr_copy, n, d)
    
    return list(arr_copy)

def bucket_sort_wrapper(dll, arr):
    """Wrapper para Bucket Sort en C"""
    # Convertir a doubles en rango [0, 1) para bucket sort
    if not arr:
        return []
    
    min_val = min(arr)
    max_val = max(arr)
    
    # Normalizar a [0, 1)
    normalized = [(x - min_val) / (max_val - min_val + 1e-10) for x in arr]
    
    n = len(normalized)
    arr_copy = (ctypes.c_double * n)(*normalized)
    
    dll.bucket_sort(arr_copy, n)
    
    # Desnormalizar
    result = [x * (max_val - min_val) + min_val for x in arr_copy]
    
    return [int(x) for x in result]  # Convertir de vuelta a enteros

# =============================================================================
# FUNCIONES DE PRUEBA Y MEDICIÓN
# =============================================================================

def generate_test_array(size, array_type='random'):
    """Genera arrays de prueba de diferentes tipos"""
    if array_type == 'random':
        return [random.randint(0, 1000) for _ in range(size)]
    elif array_type == 'sorted':
        return list(range(size))
    elif array_type == 'reverse_sorted':
        return list(range(size, 0, -1))
    elif array_type == 'nearly_sorted':
        arr = list(range(size))
        # Intercambiar algunos elementos
        for _ in range(size // 10):
            i, j = random.randint(0, size-1), random.randint(0, size-1)
            arr[i], arr[j] = arr[j], arr[i]
        return arr
    else:
        return [random.randint(0, 1000) for _ in range(size)]

def measure_time(func, arr, *args):
    """Mide el tiempo de ejecución de una función"""
    start_time = time.perf_counter()
    result = func(arr.copy(), *args)  # Usar copia para no modificar el original
    end_time = time.perf_counter()
    return end_time - start_time, result

def verify_sorting(arr_original, arr_sorted):
    """Verifica que el array esté correctamente ordenado"""
    return arr_sorted == sorted(arr_original)

# =============================================================================
# ANÁLISIS ASINTÓTICO Y GRÁFICOS
# =============================================================================

def run_comparative_analysis():
    """Ejecuta el análisis comparativo completo"""
    
    # Cargar DLLs
    counting_dll, radix_dll, bucket_dll = load_c_dlls()
    if counting_dll is None:
        print("No se pudieron cargar las DLLs. Ejecutando solo Tim Sort...")
        dlls_loaded = False
    else:
        dlls_loaded = True
    
    # Configuración de pruebas
    sizes = [100, 500, 1000, 2000, 5000, 10000]
    array_types = ['random', 'sorted', 'reverse_sorted', 'nearly_sorted']
    
    results = {algo: {arr_type: [] for arr_type in array_types} 
               for algo in ['Tim Sort', 'Counting Sort', 'Radix Sort', 'Bucket Sort']}
    
    # Ejecutar pruebas
    for size in sizes:
        print(f"\nProbando con tamaño: {size}")
        
        for arr_type in array_types:
            print(f"  Tipo de array: {arr_type}")
            test_array = generate_test_array(size, arr_type)
            
            # Tim Sort (Python)
            time_tim, result_tim = measure_time(lambda arr: timSort(arr) or arr, test_array)
            results['Tim Sort'][arr_type].append(time_tim)
            
            if dlls_loaded:
                # Counting Sort
                k = max(test_array) if test_array else 0
                time_counting, result_counting = measure_time(
                    lambda arr: counting_sort_wrapper(counting_dll, arr, k), test_array
                )
                results['Counting Sort'][arr_type].append(time_counting)
                
                # Radix Sort
                d = len(str(max(test_array))) if test_array else 1
                time_radix, result_radix = measure_time(
                    lambda arr: radix_sort_wrapper(radix_dll, arr, d), test_array
                )
                results['Radix Sort'][arr_type].append(time_radix)
                
                # Bucket Sort
                time_bucket, result_bucket = measure_time(
                    lambda arr: bucket_sort_wrapper(bucket_dll, arr), test_array
                )
                results['Bucket Sort'][arr_type].append(time_bucket)
                
                # Verificar que todos producen el mismo resultado
                assert verify_sorting(test_array, result_tim), "Tim Sort falló"
                assert verify_sorting(test_array, result_counting), "Counting Sort falló"
                assert verify_sorting(test_array, result_radix), "Radix Sort falló"
                assert verify_sorting(test_array, result_bucket), "Bucket Sort falló"
    
    return results, sizes, array_types, dlls_loaded

def plot_results(results, sizes, array_types, dlls_loaded):
    """Genera gráficos comparativos"""
    
    algorithms = ['Tim Sort']
    if dlls_loaded:
        algorithms.extend(['Counting Sort', 'Radix Sort', 'Bucket Sort'])
    
    # Gráfico para cada tipo de array
    for arr_type in array_types:
        plt.figure(figsize=(12, 8))
        
        for algo in algorithms:
            times = results[algo][arr_type]
            plt.plot(sizes, times, marker='o', label=algo, linewidth=2)
        
        plt.xlabel('Tamaño del Array')
        plt.ylabel('Tiempo (segundos)')
        plt.title(f'Comparación de Algoritmos - Array {arr_type.replace("_", " ").title()}')
        plt.legend()
        plt.grid(True, alpha=0.3)
        plt.xscale('log')
        plt.yscale('log')
        plt.tight_layout()
        plt.savefig(f'comparison_{arr_type}.png', dpi=300, bbox_inches='tight')
        plt.show()
    
    # Gráfico comparativo general (promedio de todos los tipos)
    plt.figure(figsize=(12, 8))
    
    for algo in algorithms:
        avg_times = []
        for i in range(len(sizes)):
            avg_time = np.mean([results[algo][arr_type][i] for arr_type in array_types])
            avg_times.append(avg_time)
        
        plt.plot(sizes, avg_times, marker='o', label=algo, linewidth=2)
    
    plt.xlabel('Tamaño del Array')
    plt.ylabel('Tiempo Promedio (segundos)')
    plt.title('Comparación General de Algoritmos de Ordenamiento')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.xscale('log')
    plt.yscale('log')
    plt.tight_layout()
    plt.savefig('comparison_general.png', dpi=300, bbox_inches='tight')
    plt.show()
    
    # Gráfico de speedup relativo
    if dlls_loaded:
        plt.figure(figsize=(12, 8))
        
        c_algorithms = ['Counting Sort', 'Radix Sort', 'Bucket Sort']
        for algo in c_algorithms:
            speedups = []
            for i in range(len(sizes)):
                tim_time = np.mean([results['Tim Sort'][arr_type][i] for arr_type in array_types])
                c_time = np.mean([results[algo][arr_type][i] for arr_type in array_types])
                speedup = tim_time / c_time if c_time > 0 else 1
                speedups.append(speedup)
            
            plt.plot(sizes, speedups, marker='o', label=algo, linewidth=2)
        
        plt.axhline(y=1, color='red', linestyle='--', label='Línea base (Tim Sort)')
        plt.xlabel('Tamaño del Array')
        plt.ylabel('Speedup (Tim Sort / Algoritmo C)')
        plt.title('Speedup Relativo: Algoritmos en C vs Tim Sort en Python')
        plt.legend()
        plt.grid(True, alpha=0.3)
        plt.tight_layout()
        plt.savefig('speedup_comparison.png', dpi=300, bbox_inches='tight')
        plt.show()

def print_statistical_analysis(results, sizes, array_types, dlls_loaded):
    """Imprime análisis estadístico detallado"""
    
    print("\n" + "="*80)
    print("ANÁLISIS ESTADÍSTICO COMPARATIVO")
    print("="*80)
    
    algorithms = ['Tim Sort']
    if dlls_loaded:
        algorithms.extend(['Counting Sort', 'Radix Sort', 'Bucket Sort'])
    
    for algo in algorithms:
        print(f"\n{algo}:")
        print("-" * 40)
        
        for arr_type in array_types:
            times = results[algo][arr_type]
            avg_time = np.mean(times)
            std_time = np.std(times)
            
            print(f"  {arr_type.replace('_', ' ').title():<15}: {avg_time:.6f}s ± {std_time:.6f}s")
        
        # Tiempo promedio general
        all_times = [t for arr_type in array_types for t in results[algo][arr_type]]
        overall_avg = np.mean(all_times)
        print(f"  {'Promedio General':<15}: {overall_avg:.6f}s")

# =============================================================================
# EJECUCIÓN PRINCIPAL
# =============================================================================

if __name__ == "__main__":
    print("INICIANDO ANÁLISIS COMPARATIVO DE ALGORITMOS DE ORDENAMIENTO")
    print("="*60)
    
    # Ejecutar análisis
    results, sizes, array_types, dlls_loaded = run_comparative_analysis()
    
    # Generar gráficos
    print("\nGenerando gráficos...")
    plot_results(results, sizes, array_types, dlls_loaded)
    
    # Mostrar análisis estadístico
    print_statistical_analysis(results, sizes, array_types, dlls_loaded)
    
    print("\n" + "="*60)
    print("ANÁLISIS COMPLETADO")
    print("="*60)
    
    if dlls_loaded:
        print("\nCONCLUSIONES:")
        print("• Los algoritmos en C generalmente serán más rápidos que Tim Sort en Python")
        print("• Tim Sort es robusto para diferentes tipos de datos")
        print("• Counting Sort es muy eficiente para rangos limitados")
        print("• Radix Sort es excelente para números con muchos dígitos")
        print("• Bucket Sort es ideal para distribuciones uniformes")
    else:
        print("\nNota: Solo se ejecutó Tim Sort. Para comparación completa, compila los scripts C como DLLs.")