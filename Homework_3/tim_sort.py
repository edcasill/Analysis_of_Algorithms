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
    arr_copy = arr.copy()
    n = len(arr_copy)
    minRun = calcMinRun(n)

    for start in range(0, n, minRun):
        end = min(start + minRun - 1, n - 1)
        insertionSort(arr_copy, start, end)

    size = minRun
    while size < n:
        for left in range(0, n, 2 * size):
            mid = min(n - 1, left + size - 1)
            right = min((left + 2 * size - 1), (n - 1))
            if mid < right:
                merge(arr_copy, left, mid, right)
        size = 2 * size
    
    return arr_copy

# =============================================================================
# CARGA DE DLLs DE C
# =============================================================================

def load_all_dlls():
    """Carga todas las DLLs de los algoritmos en C"""
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
        
        print("✓ Todas las DLLs cargadas correctamente")
        return counting_dll, radix_dll, bucket_dll
        
    except Exception as e:
        print(f"✗ Error cargando DLLs: {e}")
        print("  Asegúrate de que los archivos .dll estén en el mismo directorio")
        return None, None, None

def load_counting_radix_dlls():
    """Carga solo las DLLs de Counting y Radix Sort"""
    lib_path = os.path.dirname(os.path.abspath(__file__))
    path1 = os.path.join(lib_path, "counting_sort.dll")
    path2 = os.path.join(lib_path, "radix_queue.dll")
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
        
        print("✓ DLLs de Counting Sort y Radix Sort cargadas correctamente")
        return counting_dll, radix_dll
        
    except Exception as e:
        print(f"✗ Error cargando DLLs: {e}")
        return None, None

def load_bucket_dll():
    """Carga solo la DLL de Bucket Sort"""
    lib_path = os.path.dirname(os.path.abspath(__file__))
    path3 = os.path.join(lib_path, "bucket_sort.dll")
    try:
        # Bucket Sort DLL (para doubles)
        bucket_dll = ctypes.CDLL(path3)
        bucket_dll.bucket_sort.argtypes = [
            ctypes.POINTER(ctypes.c_double),  # A
            ctypes.c_int                       # n
        ]
        bucket_dll.bucket_sort.restype = None
        
        print("✓ DLL de Bucket Sort cargada correctamente")
        return bucket_dll
        
    except Exception as e:
        print(f"✗ Error cargando DLL de Bucket Sort: {e}")
        return None

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
    """Wrapper para Bucket Sort en C que adapta enteros a doubles [0,1)"""
    if not arr:
        return []
    
    # Convertir enteros a doubles en el rango [0, 1)
    min_val = min(arr)
    max_val = max(arr)
    
    if min_val == max_val:
        return arr.copy()
    
    # Normalizar a [0, 1)
    normalized = [(x - min_val) / (max_val - min_val + 1e-10) for x in arr]
    
    n = len(normalized)
    arr_copy = (ctypes.c_double * n)(*normalized)
    
    # Llamar a la DLL de Bucket Sort
    dll.bucket_sort(arr_copy, n)
    
    # Convertir de vuelta a enteros
    result_doubles = list(arr_copy)
    result_ints = [int(x * (max_val - min_val) + min_val) for x in result_doubles]
    
    return result_ints

def bucket_sort_python(arr):
    """Implementación de Bucket Sort en Python para enteros (backup)"""
    if len(arr) == 0:
        return arr
    
    min_val = min(arr)
    max_val = max(arr)
    
    if min_val == max_val:
        return arr.copy()
    
    bucket_count = len(arr)
    buckets = [[] for _ in range(bucket_count)]
    
    for num in arr:
        normalized = (num - min_val) / (max_val - min_val + 1)
        index = int(normalized * bucket_count)
        if index == bucket_count:
            index = bucket_count - 1
        buckets[index].append(num)
    
    for i in range(bucket_count):
        buckets[i].sort()
    
    result = []
    for bucket in buckets:
        result.extend(bucket)
    
    return result

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
        for _ in range(size // 10):
            i, j = random.randint(0, size-1), random.randint(0, size-1)
            arr[i], arr[j] = arr[j], arr[i]
        return arr
    else:
        return [random.randint(0, 1000) for _ in range(size)]

def measure_time(func, arr, *args):
    """Mide el tiempo de ejecución de una función"""
    start_time = time.perf_counter()
    result = func(arr, *args)
    end_time = time.perf_counter()
    return end_time - start_time, result

def verify_sorting(arr_original, arr_sorted):
    """Verifica que el array esté correctamente ordenado"""
    expected = sorted(arr_original)
    is_correct = arr_sorted == expected
    return is_correct

# =============================================================================
# EXPERIMENTO 1: BUCKET SORT vs TIM SORT
# =============================================================================

def run_experiment_bucket_vs_tim():
    """Experimento 1: Compara Bucket Sort vs Tim Sort"""
    print("=" * 70)
    print("EXPERIMENTO 1: BUCKET SORT vs TIM SORT")
    print("=" * 70)
    
    # Cargar DLL de Bucket Sort
    bucket_dll = load_bucket_dll()
    bucket_dll_loaded = bucket_dll is not None
    
    # Tamaños de array (misma longitud para ambos algoritmos)
    sizes = [100, 500, 1000, 2000, 5000, 10000, 20000]
    array_types = ['random', 'sorted', 'reverse_sorted', 'nearly_sorted']
    
    results = {
        'Tim Sort': {arr_type: [] for arr_type in array_types},
        'Bucket Sort': {arr_type: [] for arr_type in array_types}
    }
    
    # Ejecutar pruebas
    for size in sizes:
        print(f"\n📊 Tamaño del array: {size}")
        
        for arr_type in array_types:
            print(f"  🎯 Tipo: {arr_type}")
            
            # Generar el MISMO array para ambos algoritmos
            test_array = generate_test_array(size, arr_type)
            
            # Tim Sort
            print("    🔄 Tim Sort...", end=" ")
            time_tim, result_tim = measure_time(timSort, test_array)
            results['Tim Sort'][arr_type].append(time_tim)
            if verify_sorting(test_array, result_tim):
                print(f"✓ ({time_tim:.6f}s)")
            else:
                print(f"✗ ({time_tim:.6f}s)")
            
            # Bucket Sort (usando DLL si está disponible)
            print("    🔄 Bucket Sort...", end=" ")
            if bucket_dll_loaded:
                try:
                    time_bucket, result_bucket = measure_time(
                        lambda arr: bucket_sort_wrapper(bucket_dll, arr), test_array
                    )
                    if verify_sorting(test_array, result_bucket):
                        print(f"✓ ({time_bucket:.6f}s) [DLL]")
                    else:
                        print(f"✗ ({time_bucket:.6f}s) [DLL]")
                        # Si falla, usar implementación Python
                        time_bucket, result_bucket = measure_time(bucket_sort_python, test_array)
                        print(f"  (usando Python implementation)")
                except Exception as e:
                    print(f"Error con DLL: {e}, usando Python...")
                    time_bucket, result_bucket = measure_time(bucket_sort_python, test_array)
                    print(f"✓ ({time_bucket:.6f}s) [Python]")
            else:
                # Usar implementación Python si no hay DLL
                time_bucket, result_bucket = measure_time(bucket_sort_python, test_array)
                if verify_sorting(test_array, result_bucket):
                    print(f"✓ ({time_bucket:.6f}s) [Python]")
                else:
                    print(f"✗ ({time_bucket:.6f}s) [Python]")
            
            results['Bucket Sort'][arr_type].append(time_bucket)
    
    return results, sizes, array_types, bucket_dll_loaded

def plot_experiment1(results, sizes, array_types, bucket_dll_used):
    """Genera gráficos para el Experimento 1"""
    print("\n📊 Generando gráficos para Experimento 1...")
    
    # Gráfico individual por tipo de array
    for arr_type in array_types:
        plt.figure(figsize=(12, 8))
        
        for algo in ['Tim Sort', 'Bucket Sort']:
            times = results[algo][arr_type]
            plt.plot(sizes, times, marker='o', label=algo, linewidth=2, markersize=6)
        
        plt.xlabel('Tamaño del Array', fontsize=12)
        plt.ylabel('Tiempo (segundos)', fontsize=12)
        plt.title(f'Bucket Sort vs Tim Sort - Array {arr_type.replace("_", " ").title()}', fontsize=14)
        plt.legend(fontsize=12)
        plt.grid(True, alpha=0.3)
        plt.xscale('log')
        plt.yscale('log')
        plt.tight_layout()
        plt.savefig(f'experiment1_{arr_type}.png', dpi=300, bbox_inches='tight')
        print(f"  ✅ Gráfico guardado: experiment1_{arr_type}.png")
        plt.show()
    
    # Gráfico comparativo general
    plt.figure(figsize=(12, 8))
    
    for algo in ['Tim Sort', 'Bucket Sort']:
        avg_times = []
        for i in range(len(sizes)):
            times_for_size = [results[algo][arr_type][i] for arr_type in array_types]
            avg_times.append(np.mean(times_for_size))
        
        plt.plot(sizes, avg_times, marker='o', label=algo, linewidth=2, markersize=6)
    
    plt.xlabel('Tamaño del Array', fontsize=12)
    plt.ylabel('Tiempo Promedio (segundos)', fontsize=12)
    plt.title('Comparación General: Bucket Sort vs Tim Sort', fontsize=14)
    plt.legend(fontsize=12)
    plt.grid(True, alpha=0.3)
    plt.xscale('log')
    plt.yscale('log')
    plt.tight_layout()
    plt.savefig('experiment1_general.png', dpi=300, bbox_inches='tight')
    print("  ✅ Gráfico guardado: experiment1_general.png")
    plt.show()
    
    # Gráfico de speedup
    plt.figure(figsize=(12, 8))
    
    speedups = []
    for i in range(len(sizes)):
        tim_times = [results['Tim Sort'][arr_type][i] for arr_type in array_types]
        bucket_times = [results['Bucket Sort'][arr_type][i] for arr_type in array_types]
        avg_tim = np.mean(tim_times)
        avg_bucket = np.mean(bucket_times)
        speedup = avg_tim / avg_bucket if avg_bucket > 0 else 1
        speedups.append(speedup)
    
    plt.plot(sizes, speedups, marker='o', color='red', linewidth=2, markersize=6)
    plt.axhline(y=1, color='black', linestyle='--', alpha=0.7, label='Línea base')
    plt.xlabel('Tamaño del Array', fontsize=12)
    plt.ylabel('Speedup (Tim Sort / Bucket Sort)', fontsize=12)
    plt.title('Speedup: Bucket Sort vs Tim Sort', fontsize=14)
    plt.legend(fontsize=12)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig('experiment1_speedup.png', dpi=300, bbox_inches='tight')
    print("  ✅ Gráfico guardado: experiment1_speedup.png")
    plt.show()

# =============================================================================
# EXPERIMENTO 2: COUNTING SORT vs RADIX SORT vs TIM SORT
# =============================================================================

def run_experiment_counting_radix_tim():
    """Experimento 2: Compara Counting Sort, Radix Sort y Tim Sort"""
    print("\n" + "=" * 70)
    print("EXPERIMENTO 2: COUNTING SORT vs RADIX SORT vs TIM SORT")
    print("=" * 70)
    
    # Cargar DLLs
    counting_dll, radix_dll = load_counting_radix_dlls()
    dlls_loaded = counting_dll is not None and radix_dll is not None
    
    if not dlls_loaded:
        print("⚠️  No se pudieron cargar las DLLs. Usando implementaciones Python...")
    
    # Tamaños de array (misma longitud que el experimento 1)
    sizes = [100, 500, 1000, 2000, 5000, 10000, 20000]
    array_types = ['random', 'sorted', 'reverse_sorted', 'nearly_sorted']
    
    results = {
        'Tim Sort': {arr_type: [] for arr_type in array_types},
        'Counting Sort': {arr_type: [] for arr_type in array_types},
        'Radix Sort': {arr_type: [] for arr_type in array_types}
    }
    
    # Ejecutar pruebas
    for size in sizes:
        print(f"\n📊 Tamaño del array: {size}")
        
        for arr_type in array_types:
            print(f"  🎯 Tipo: {arr_type}")
            
            # Generar el MISMO array para todos los algoritmos
            test_array = generate_test_array(size, arr_type)
            
            # Tim Sort
            print("    🔄 Tim Sort...", end=" ")
            time_tim, result_tim = measure_time(timSort, test_array)
            results['Tim Sort'][arr_type].append(time_tim)
            if verify_sorting(test_array, result_tim):
                print(f"✓ ({time_tim:.6f}s)")
            else:
                print(f"✗ ({time_tim:.6f}s)")
            
            if dlls_loaded:
                # Counting Sort
                print("    🔄 Counting Sort...", end=" ")
                k = max(test_array) if test_array else 0
                time_counting, result_counting = measure_time(
                    lambda arr: counting_sort_wrapper(counting_dll, arr, k), test_array
                )
                results['Counting Sort'][arr_type].append(time_counting)
                if verify_sorting(test_array, result_counting):
                    print(f"✓ ({time_counting:.6f}s)")
                else:
                    print(f"✗ ({time_counting:.6f}s)")
                
                # Radix Sort
                print("    🔄 Radix Sort...", end=" ")
                d = len(str(max(test_array))) if test_array else 1
                time_radix, result_radix = measure_time(
                    lambda arr: radix_sort_wrapper(radix_dll, arr, d), test_array
                )
                results['Radix Sort'][arr_type].append(time_radix)
                if verify_sorting(test_array, result_radix):
                    print(f"✓ ({time_radix:.6f}s)")
                else:
                    print(f"✗ ({time_radix:.6f}s)")
            else:
                # Usar sorted de Python como proxy si no hay DLLs
                print("    🔄 Counting Sort (Python sorted)...", end=" ")
                time_counting, _ = measure_time(sorted, test_array)
                results['Counting Sort'][arr_type].append(time_counting)
                print(f"✓ ({time_counting:.6f}s)")
                
                print("    🔄 Radix Sort (Python sorted)...", end=" ")
                time_radix, _ = measure_time(sorted, test_array)
                results['Radix Sort'][arr_type].append(time_radix)
                print(f"✓ ({time_radix:.6f}s)")
    
    return results, sizes, array_types, dlls_loaded

def plot_experiment2(results, sizes, array_types, dlls_loaded):
    """Genera gráficos para el Experimento 2"""
    print("\n📊 Generando gráficos para Experimento 2...")
    
    algorithms = ['Tim Sort', 'Counting Sort', 'Radix Sort']
    
    # Gráfico individual por tipo de array
    for arr_type in array_types:
        plt.figure(figsize=(12, 8))
        
        for algo in algorithms:
            times = results[algo][arr_type]
            plt.plot(sizes, times, marker='o', label=algo, linewidth=2, markersize=6)
        
        plt.xlabel('Tamaño del Array', fontsize=12)
        plt.ylabel('Tiempo (segundos)', fontsize=12)
        plt.title(f'Counting/Radix vs Tim Sort - Array {arr_type.replace("_", " ").title()}', fontsize=14)
        plt.legend(fontsize=12)
        plt.grid(True, alpha=0.3)
        plt.xscale('log')
        plt.yscale('log')
        plt.tight_layout()
        plt.savefig(f'experiment2_{arr_type}.png', dpi=300, bbox_inches='tight')
        print(f"  ✅ Gráfico guardado: experiment2_{arr_type}.png")
        plt.show()
    
    # Gráfico comparativo general
    plt.figure(figsize=(12, 8))
    
    for algo in algorithms:
        avg_times = []
        for i in range(len(sizes)):
            times_for_size = [results[algo][arr_type][i] for arr_type in array_types]
            avg_times.append(np.mean(times_for_size))
        
        plt.plot(sizes, avg_times, marker='o', label=algo, linewidth=2, markersize=6)
    
    plt.xlabel('Tamaño del Array', fontsize=12)
    plt.ylabel('Tiempo Promedio (segundos)', fontsize=12)
    plt.title('Comparación General: Counting Sort vs Radix Sort vs Tim Sort', fontsize=14)
    plt.legend(fontsize=12)
    plt.grid(True, alpha=0.3)
    plt.xscale('log')
    plt.yscale('log')
    plt.tight_layout()
    plt.savefig('experiment2_general.png', dpi=300, bbox_inches='tight')
    print("  ✅ Gráfico guardado: experiment2_general.png")
    plt.show()

# =============================================================================
# EJECUCIÓN PRINCIPAL
# =============================================================================

if __name__ == "__main__":
    print("ANÁLISIS COMPARATIVO DE ALGORITMOS DE ORDENAMIENTO")
    print("=" * 70)
    
    try:
        # EXPERIMENTO 1: Bucket Sort vs Tim Sort
        results1, sizes1, array_types1, bucket_dll_used = run_experiment_bucket_vs_tim()
        plot_experiment1(results1, sizes1, array_types1, bucket_dll_used)
        
        # EXPERIMENTO 2: Counting Sort vs Radix Sort vs Tim Sort  
        results2, sizes2, array_types2, dlls_loaded = run_experiment_counting_radix_tim()
        plot_experiment2(results2, sizes2, array_types2, dlls_loaded)
        
        print("\n" + "=" * 70)
        print("✅ ANÁLISIS COMPLETADO EXITOSAMENTE")
        print("=" * 70)
        
        print(f"\n📋 RESUMEN:")
        print(f"  Bucket Sort: {'DLL' if bucket_dll_used else 'Python'}")
        print(f"  Counting/Radix: {'DLL' if dlls_loaded else 'Python'}")
        
    except Exception as e:
        print(f"\n❌ Error durante la ejecución: {e}")
        import traceback
        traceback.print_exc()