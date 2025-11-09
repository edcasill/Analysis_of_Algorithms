import matplotlib.pyplot as plt
import numpy as np
import os

def read_data(filename):
    """Leer datos de los archivos generados"""
    n, time, extra = [], [], []
    try:
        with open(filename, 'r') as f:
            for line in f:
                if line.startswith('#'):
                    continue
                parts = line.strip().split()
                if len(parts) >= 2:
                    n.append(int(parts[0]))
                    time.append(float(parts[1]))
                    if len(parts) >= 3:
                        extra.append(int(parts[2]))
        return np.array(n), np.array(time), np.array(extra) if extra else None
    except FileNotFoundError:
        print(f"❌ Archivo {filename} no encontrado")
        return None, None, None

print("=== VISUALIZADOR DE COMPLEJIDAD EN WINDOWS ===\n")

# Leer todos los datos
n_counting, t_counting, k_counting = read_data('counting_sort_data.txt')
n_radix, t_radix, d_radix = read_data('radix_sort_data.txt')
n_bucket, t_bucket, _ = read_data('bucket_sort_data.txt')
n_theoretical, t_theoretical_n, t_theoretical_nk, t_theoretical_nd, t_theoretical_n2 = [], [], [], [], []

try:
    with open('theoretical_data.txt', 'r') as f:
        for line in f:
            if line.startswith('#'):
                continue
            parts = line.strip().split()
            if len(parts) >= 5:
                n_theoretical.append(int(parts[0]))
                t_theoretical_n.append(float(parts[1]))
                t_theoretical_nk.append(float(parts[2]))
                t_theoretical_nd.append(float(parts[3]))
                t_theoretical_n2.append(float(parts[4]))
except FileNotFoundError:
    print("❌ Archivo theoretical_data.txt no encontrado")

# Crear gráfica comparativa
plt.figure(figsize=(14, 10))

# Gráfica 1: Comparación general
plt.subplot(2, 2, 1)
if n_counting is not None:
    plt.plot(n_counting, t_counting, 'o-', label='Counting Sort O(n+k)', linewidth=2, markersize=4)
if n_radix is not None:
    plt.plot(n_radix, t_radix, 's-', label='Radix Sort O(dn)', linewidth=2, markersize=4)
if n_bucket is not None:
    plt.plot(n_bucket, t_bucket, '^-', label='Bucket Sort O(n)', linewidth=2, markersize=4)

plt.xlabel('Tamaño del Array (n)')
plt.ylabel('Tiempo (segundos)')
plt.title('Comparación de Algoritmos')
plt.grid(True, alpha=0.3)
plt.legend()
plt.xscale('log')
plt.yscale('log')

# Gráfica 2: Counting Sort
plt.subplot(2, 2, 2)
if n_counting is not None:
    plt.plot(n_counting, t_counting, 'o-', color='blue', label='Experimental', linewidth=2)
if n_theoretical:
    plt.plot(n_theoretical, t_theoretical_nk, '--', color='red', label='O(n+k) teórico', linewidth=2)
plt.xlabel('Tamaño del Array (n)')
plt.ylabel('Tiempo (segundos)')
plt.title('Counting Sort - O(n + k)')
plt.grid(True, alpha=0.3)
plt.legend()

# Gráfica 3: Radix Sort
plt.subplot(2, 2, 3)
if n_radix is not None:
    plt.plot(n_radix, t_radix, 's-', color='green', label='Experimental', linewidth=2)
if n_theoretical:
    plt.plot(n_theoretical, t_theoretical_nd, '--', color='red', label='O(dn) teórico', linewidth=2)
plt.xlabel('Tamaño del Array (n)')
plt.ylabel('Tiempo (segundos)')
plt.title('Radix Sort - O(d × n)')
plt.grid(True, alpha=0.3)
plt.legend()

# Gráfica 4: Bucket Sort
plt.subplot(2, 2, 4)
if n_bucket is not None:
    plt.plot(n_bucket, t_bucket, '^-', color='purple', label='Experimental', linewidth=2)
if n_theoretical:
    plt.plot(n_theoretical, t_theoretical_n, '--', color='red', label='O(n) teórico', linewidth=2)
plt.xlabel('Tamaño del Array (n)')
plt.ylabel('Tiempo (segundos)')
plt.title('Bucket Sort - O(n)')
plt.grid(True, alpha=0.3)
plt.legend()

plt.tight_layout()
plt.savefig('windows_complexity_analysis.png', dpi=300, bbox_inches='tight')
print("✅ Gráfica guardada como: windows_complexity_analysis.png")

# Mostrar estadísticas
print("\n=== ESTADÍSTICAS ===")
if n_counting is not None:
    print(f"Counting Sort: {len(n_counting)} puntos de datos")
if n_radix is not None:
    print(f"Radix Sort: {len(n_radix)} puntos de datos")  
if n_bucket is not None:
    print(f"Bucket Sort: {len(n_bucket)} puntos de datos")

# Mostrar tabla de datos
print("\n=== DATOS COUNTING SORT ===")
if n_counting is not None:
    print("n      | Tiempo(s) | k")
    print("-------|-----------|----")
    for i in range(min(5, len(n_counting))):
        print(f"{n_counting[i]:6} | {t_counting[i]:9.6f} | {k_counting[i]}")

plt.show()