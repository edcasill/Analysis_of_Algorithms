#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

void matrix_chain_order(int p[], int n, int **m, int **s) {
    int i, l, j, k, q;

    for (i = 1; i <= n; i++) {
        m[i][i] = 0;
    }
    for (l = 2; l <= n; l++) {
        for (i = 1; i <= n - l + 1; i++) {
            j = i + l - 1;
            m[i][j] = INT_MAX;
            for (k = i; k <= j - 1; k++) {
                q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
                if (q < m[i][j]) {
                    m[i][j] = q;
                    s[i][j] = k;
                }
            }
        }
    }
}

void generate_random_dimensions(int p[], int n, int max_dim) {
    for (int i = 0; i <= n; i++) {
        p[i] = rand() % max_dim + 1;
    }
}

// Function to avoid compiler optimizations
volatile int dummy_result = 0;

int main() {
    srand(time(NULL));
    
    // MUCH larger sizes and fewer stitches
    int sizes[] = {100, 200, 300, 400, 500, 600, 700, 800};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int max_dim = 100;
    int repetitions = 3; // Fewer repeats but larger sizes
    
    printf("n,Time(s)\n");
    
    for (int idx = 0; idx < num_sizes; idx++) {
        int n = sizes[idx];
        
        int *p = (int *)malloc((n + 1) * sizeof(int));
        generate_random_dimensions(p, n, max_dim);
        
        int **m = (int **)malloc((n + 1) * sizeof(int *));
        int **s = (int **)malloc((n + 1) * sizeof(int *));
        for (int i = 0; i <= n; i++) {
            m[i] = (int *)malloc((n + 1) * sizeof(int));
            s[i] = (int *)malloc((n + 1) * sizeof(int));
        }
        
        // Measure time with clock()
        clock_t total_ticks = 0;
        for (int rep = 0; rep < repetitions; rep++) {
            clock_t start = clock();
            matrix_chain_order(p, n, m, s);
            clock_t end = clock();
            total_ticks += (end - start);
            
            // Use the result to avoid optimizations
            dummy_result += m[1][n];
        }
        double avg_time = ((double)total_ticks / CLOCKS_PER_SEC) / repetitions;
        
        printf("%d,%.6f\n", n, avg_time);
        fflush(stdout); // Force immediate printing
        
        // free memory
        free(p);
        for (int i = 0; i <= n; i++) {
            free(m[i]);
            free(s[i]);
        }
        free(m);
        free(s);
    }
    
    printf("Dummy result: %d\n", dummy_result); // Prevents optimization from compiler
    
    return 0;
}