import ctypes
import time
import numpy as np
import matplotlib.pyplot as plt

# Load libraries with ctypes
radix = ctypes.CDLL("./radix.dll")
counting = ctypes.CDLL("./counting.dll")
bucket = ctypes.CDLL("./bucket.dll")

# Ctypes arguments
radix.radix_sort_c.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int]
radix.radix_sort_c.restype = ctypes.c_double
counting.counting_sort_c.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.c_int, ctypes.c_int]
counting.counting_sort_c.restype = ctypes.c_double
bucket.bucket_sort_c.argtypes = [ctypes.POINTER(ctypes.c_double), ctypes.c_int]
bucket.bucket_sort_c.restype = ctypes.c_double

# n tests
n_values = [1000, 2500, 5000, 7500, 15000, 30000]
# times for each n
t_radix, t_counting, t_bucket, t_timsort = [], [], [], []

for n in n_values:
    # Radix and counting sorts data
    data_int = np.random.randint(0, 9999, n, dtype=np.int32)
    ptr_int = data_int.ctypes.data_as(ctypes.POINTER(ctypes.c_int))
    # Bucket sort data
    data_float = np.random.random(n).astype(np.double)
    ptr_float = data_float.ctypes.data_as(ctypes.POINTER(ctypes.c_double))

    # Execute c algorithms
    t_radix.append(radix.radix_sort_c(ptr_int, n, 4))
    t_counting.append(counting.counting_sort_c(ptr_int, n, 9999))
    t_bucket.append(bucket.bucket_sort_c(ptr_float, n))

    # Timsort python
    data_py = list(data_int)
    start = time.perf_counter()
    data_py.sort()
    end = time.perf_counter()
    t_timsort.append((end - start) * 1000)

# Plot results
plt.title("Linear sorting algorithms")
plt.plot(n_values, t_radix, 'o-', label="Radix Sort")
plt.plot(n_values, t_counting, 's-', label="Counting Sort")
plt.plot(n_values, t_bucket, '^-', label="Bucket Sort")
plt.plot(n_values, t_timsort, 'x--', label="Tim Sort")
plt.xlabel("n")
plt.ylabel("Time (ms)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()