import time
import random
import ctypes
import matplotlib.pyplot as plt
import numpy as np

# =============================================================================
# ALGORITMOS (versiones simplificadas para la demostración)
# =============================================================================

def timSort(arr):
    """Tim Sort - optimizado para datos del mundo real"""
    return sorted(arr)  # Usamos sorted de Python que es Tim Sort

def counting_sort(arr, k=None):
    """Counting Sort - requiere rango limitado"""
    if not arr:
        return []
    
    if k is None:
        k = max(arr)
    
    # Crear array de conteo
    count = [0] * (k + 1)
    
    # Contar ocurrencias
    for num in arr:
        count[num] += 1
    
    # Reconstruir array ordenado
    result = []
    for i in range(k + 1):
        result.extend([i] * count[i])
    
    return result

def radix_sort(arr):
    """Radix Sort - para números con muchos dígitos"""
    if not arr:
        return []
    
    # Encontrar el número máximo para saber el número de dígitos
    max_num = max(arr)
    exp = 1
    
    result = arr.copy()
    
    while max_num // exp > 0:
        # Counting sort para el dígito actual
        output = [0] * len(result)
        count = [0] * 10
        
        # Contar ocurrencias del dígito actual
        for i in range(len(result)):
            index = (result[i] // exp) % 10
            count[index] += 1
        
        # Cambiar count[i] para que contenga la posición actual
        for i in range(1, 10):
            count[i] += count[i - 1]
        
        # Construir el array de salida
        i = len(result) - 1
        while i >= 0:
            index = (result[i] // exp) % 10
            output[count[index] - 1] = result[i]
            count[index] -= 1
            i -= 1
        
        # Copiar el array de salida a result
        for i in range(len(result)):
            result[i] = output[i]
        
        exp *= 10
    
    return result

def bucket_sort(arr):
    """Bucket Sort - para distribuciones uniformes"""
    if not arr:
        return []
    
    # Encontrar mínimo y máximo
    min_val = min(arr)
    max_val = max(arr)
    
    if min_val == max_val:
        return arr.copy()
    
    # Crear buckets
    bucket_count = len(arr)
    buckets = [[] for _ in range(bucket_count)]
    
    # Distribuir elementos en buckets
    for num in arr:
        normalized = (num - min_val) / (max_val - min_val + 1)
        index = int(normalized * bucket_count)
        if index == bucket_count:
            index = bucket_count - 1
        buckets[index].append(num)
    
    # Ordenar cada bucket y concatenar
    result = []
    for bucket in buckets:
        result.extend(sorted(bucket))
    
    return result

# =============================================================================
# ESCENARIOS DONDE TIM SORT GANA
# =============================================================================

def generate_tim_sort_favorable(size, scenario):
    """Genera arrays donde Tim Sort es superior"""
    if scenario == "already_sorted":
        # Array ya ordenado - Tim Sort es O(n) en este caso
        return list(range(size))
    
    elif scenario == "nearly_sorted":
        # Array casi ordenado - Tim Sort es muy eficiente
        arr = list(range(size))
        # Hacer pocos swaps aleatorios
        swaps = size // 100  # Solo 1% de elementos desordenados
        for _ in range(swaps):
            i = random.randint(0, size-1)
            j = random.randint(0, size-1)
            arr[i], arr[j] = arr[j], arr[i]
        return arr
    
    elif scenario == "reverse_sorted":
        # Array en orden inverso - Tim Sort maneja bien los runs descendentes
        return list(range(size, 0, -1))
    
    elif scenario == "real_world_data":
        # Datos del mundo real - típicamente tienen algún patrón
        arr = []
        for i in range(size):
            # Patrón: tendencia creciente + ruido pequeño
            base = i * 10 // size
            noise = random.randint(-5, 5)
            arr.append(max(0, base + noise))
        return arr
    
    elif scenario == "large_range_small_array":
        # Rango grande en array pequeño - Counting Sort sufre
        return [random.randint(0, 1000000) for _ in range(size)]
    
    elif scenario == "mixed_data_patterns":
        # Mezcla de patrones ordenados y aleatorios
        arr = []
        # Bloques ordenados intercalados con bloques aleatorios
        block_size = size // 10
        for block in range(10):
            if block % 2 == 0:
                # Bloque ordenado
                start = block * block_size
                arr.extend(range(start, start + block_size))
            else:
                # Bloque aleatorio
                arr.extend(random.randint(0, 1000) for _ in range(block_size))
        return arr[:size]

def measure_performance():
    """Experimento para mostrar cuándo Tim Sort es mejor"""
    
    scenarios = {
        "already_sorted": "Array Ya Ordenado",
        "nearly_sorted": "Array Casi Ordenado", 
        "reverse_sorted": "Array Orden Inverso",
        "real_world_data": "Datos Mundo Real",
        "large_range_small_array": "Rango Grande/Array Pequeño",
        "mixed_data_patterns": "Patrones Mezclados"
    }
    
    sizes = [1000, 5000, 10000]
    algorithms = {
        "Tim Sort": timSort,
        "Counting Sort": counting_sort,
        "Radix Sort": radix_sort,
        "Bucket Sort": bucket_sort
    }
    
    results = {algo: {scenario: [] for scenario in scenarios} for algo in algorithms}
    
    print("🎯 EXPERIMENTO: ¿CUÁNDO TIM SORT ES MEJOR?")
    print("=" * 70)
    
    for size in sizes:
        print(f"\n📊 Tamaño del array: {size}")
        print("-" * 50)
        
        for scenario_name, scenario_desc in scenarios.items():
            print(f"\n🎯 Escenario: {scenario_desc}")
            
            # Generar array específico para este escenario
            test_array = generate_tim_sort_favorable(size, scenario_name)
            
            for algo_name, algo_func in algorithms.items():
                # Configuración específica para cada algoritmo
                if algo_name == "Counting Sort":
                    if scenario_name == "large_range_small_array":
                        # Counting Sort será muy lento aquí
                        k = 1000000
                    else:
                        k = max(test_array) if test_array else 0
                    algo_with_params = lambda arr: counting_sort(arr, k)
                else:
                    algo_with_params = algo_func
                
                # Medir tiempo
                start_time = time.perf_counter()
                result = algo_with_params(test_array.copy())
                end_time = time.perf_counter()
                
                execution_time = end_time - start_time
                results[algo_name][scenario_name].append(execution_time)
                
                print(f"    {algo_name:<15}: {execution_time:.6f}s")
    
    return results, scenarios, sizes, algorithms

def plot_tim_sort_advantages(results, scenarios, sizes, algorithms):
    """Genera gráficos mostrando las ventajas de Tim Sort"""
    
    print("\n📊 GENERANDO GRÁFICOS DE VENTAJAS DE TIM SORT...")
    
    # Gráfico 1: Comparación por escenario (para tamaño fijo)
    fixed_size_idx = 1  # Usar tamaño medio (5000)
    fixed_size = sizes[fixed_size_idx]
    
    plt.figure(figsize=(14, 8))
    
    scenario_names = list(scenarios.keys())
    scenario_descriptions = [scenarios[scenario] for scenario in scenario_names]
    
    x_pos = np.arange(len(scenario_names))
    width = 0.2
    
    for i, algo_name in enumerate(algorithms.keys()):
        times = []
        for scenario in scenario_names:
            if fixed_size_idx < len(results[algo_name][scenario]):
                times.append(results[algo_name][scenario][fixed_size_idx])
            else:
                times.append(0)
        
        plt.bar(x_pos + i * width, times, width, label=algo_name, alpha=0.8)
    
    plt.xlabel('Escenarios', fontsize=12)
    plt.ylabel('Tiempo (segundos)', fontsize=12)
    plt.title(f'Comparación de Algoritmos - Array de {fixed_size} elementos', fontsize=14)
    plt.xticks(x_pos + width * 1.5, scenario_descriptions, rotation=45, ha='right')
    plt.legend()
    plt.grid(True, alpha=0.3, axis='y')
    plt.tight_layout()
    plt.savefig('tim_sort_advantages_scenarios.png', dpi=300, bbox_inches='tight')
    plt.show()
    
    # Gráfico 2: Speedup de Tim Sort vs otros algoritmos
    plt.figure(figsize=(12, 8))
    
    comparison_algorithms = ["Counting Sort", "Radix Sort", "Bucket Sort"]
    
    for algo_name in comparison_algorithms:
        speedups = []
        for scenario in scenario_names:
            tim_times = results["Tim Sort"][scenario]
            algo_times = results[algo_name][scenario]
            
            # Usar el último tamaño disponible
            if len(tim_times) > 0 and len(algo_times) > 0:
                last_idx = min(len(tim_times) - 1, len(algo_times) - 1)
                speedup = algo_times[last_idx] / tim_times[last_idx] if tim_times[last_idx] > 0 else 1
                speedups.append(speedup)
            else:
                speedups.append(1)
        
        plt.plot(scenario_descriptions, speedups, marker='o', label=algo_name, linewidth=2)
    
    plt.axhline(y=1, color='red', linestyle='--', label='Línea base (Tim Sort)')
    plt.xlabel('Escenarios', fontsize=12)
    plt.ylabel('Speedup (Algoritmo / Tim Sort)', fontsize=12)
    plt.title('Ventaja de Tim Sort: Cuántas veces es más rápido', fontsize=14)
    plt.xticks(rotation=45, ha='right')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig('tim_sort_speedup.png', dpi=300, bbox_inches='tight')
    plt.show()
    
    # Gráfico 3: Comportamiento en arrays casi ordenados
    plt.figure(figsize=(10, 6))
    
    nearly_sorted_scenario = "nearly_sorted"
    for algo_name in algorithms.keys():
        times = results[algo_name][nearly_sorted_scenario]
        plt.plot(sizes[:len(times)], times, marker='o', label=algo_name, linewidth=2)
    
    plt.xlabel('Tamaño del Array', fontsize=12)
    plt.ylabel('Tiempo (segundos)', fontsize=12)
    plt.title('Comportamiento en Arrays Casi Ordenados', fontsize=14)
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.xscale('log')
    plt.yscale('log')
    plt.tight_layout()
    plt.savefig('nearly_sorted_performance.png', dpi=300, bbox_inches='tight')
    plt.show()

def analyze_why_tim_sort_wins():
    """Análisis de por qué Tim Sort gana en ciertos escenarios"""
    
    print("\n" + "=" * 70)
    print("ANÁLISIS: ¿POR QUÉ TIM SORT GANA EN ESTOS ESCENARIOS?")
    print("=" * 70)
    
    advantages = {
        "already_sorted": {
            "reason": "Tim Sort detecta runs ya ordenados en O(n)",
            "others": "Counting/Radix igual hacen todo el proceso completo",
            "complexity": "Tim Sort: O(n), Otros: O(n) pero con constantes mayores"
        },
        "nearly_sorted": {
            "reason": "Tim Sort aprovecha los runs naturales existentes",
            "others": "Algoritmos O(n) ignoran el orden parcial existente", 
            "complexity": "Tim Sort: ~O(n), Otros: O(n) con overhead fijo"
        },
        "reverse_sorted": {
            "reason": "Tim Sort detecta runs descendentes y los invierte eficientemente",
            "others": "Tratan cada elemento independientemente",
            "complexity": "Tim Sort: O(n), Otros: O(n) pero más lento"
        },
        "large_range_small_array": {
            "reason": "Tim Sort no depende del rango de valores",
            "others": "Counting Sort sufre con k grande, Bucket Sort con muchos buckets vacíos",
            "complexity": "Tim Sort: O(n log n), Counting: O(n + k) con k grande"
        },
        "real_world_data": {
            "reason": "Tim Sort está optimizado para patrones comunes en datos reales",
            "others": "Algoritmos especializados asumen distribuciones específicas",
            "complexity": "Tim Sort se adapta al patrón de datos"
        },
        "mixed_data_patterns": {
            "reason": "Tim Sort maneja eficientemente mezclas de patrones",
            "others": "Algoritmos O(n) no se benefician de patrones locales",
            "complexity": "Tim Sort aprovecha la estructura local"
        }
    }
    
    for scenario, info in advantages.items():
        print(f"\n🎯 {scenarios[scenario]}:")
        print(f"   ✅ Tim Sort: {info['reason']}")
        print(f"   ❌ Otros: {info['others']}")
        print(f"   📊 Complejidad: {info['complexity']}")

# =============================================================================
# EJECUCIÓN PRINCIPAL
# =============================================================================

if __name__ == "__main__":
    print("DEMOSTRACIÓN: ESCENARIOS DONDE TIM SORT ES MEJOR")
    print("=" * 70)
    
    # Ejecutar experimento
    results, scenarios, sizes, algorithms = measure_performance()
    
    # Generar gráficos
    plot_tim_sort_advantages(results, scenarios, sizes, algorithms)
    
    # Mostrar análisis
    analyze_why_tim_sort_wins()
