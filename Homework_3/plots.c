#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

// ==================== ESTRUCTURAS Y FUNCIONES AUXILIARES ====================

// Estructuras para Queue (Radix Sort)
typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
} Queue;

Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue* q, int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = NULL;
    
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
        return;
    }
    
    q->rear->next = newNode;
    q->rear = newNode;
}

int dequeue(Queue* q) {
    if (q->front == NULL) return -1;
    
    Node* temp = q->front;
    int value = temp->data;
    q->front = q->front->next;
    
    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    free(temp);
    return value;
}

int isQueueEmpty(Queue* q) {
    return q->front == NULL;
}

// Estructuras para List (Bucket Sort)
typedef struct ListNode {
    double data;
    struct ListNode* next;
} ListNode;

typedef struct {
    ListNode* head;
} List;

List* createList() {
    List* list = (List*)malloc(sizeof(List));
    list->head = NULL;
    return list;
}

void insertSorted(List* list, double value) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->data = value;
    newNode->next = NULL;
    
    if (list->head == NULL || value < list->head->data) {
        newNode->next = list->head;
        list->head = newNode;
        return;
    }
    
    ListNode* current = list->head;
    while (current->next != NULL && current->next->data < value) {
        current = current->next;
    }
    
    newNode->next = current->next;
    current->next = newNode;
}

void freeList(List* list) {
    ListNode* current = list->head;
    while (current != NULL) {
        ListNode* temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
}

// ==================== IMPLEMENTACIONES DE ALGORITMOS ====================

// Counting Sort
void counting_sort(int A[], int n, int k) {
    int *B = (int*)malloc(n * sizeof(int));
    int *C = (int*)malloc((k + 1) * sizeof(int));
    
    for (int i = 0; i <= k; i++) C[i] = 0;
    for (int j = 0; j < n; j++) C[A[j]]++;
    for (int i = 1; i <= k; i++) C[i] += C[i - 1];
    for (int j = n - 1; j >= 0; j--) {
        B[C[A[j]] - 1] = A[j];
        C[A[j]]--;
    }
    for (int i = 0; i < n; i++) A[i] = B[i];
    
    free(C);
    free(B);
}

// Radix Sort con Queues
void radix_sort_queue(int A[], int n, int d) {
    Queue* buckets[10];
    for (int i = 0; i < 10; i++) buckets[i] = createQueue();
    
    for (int digit = 0; digit < d; digit++) {
        for (int i = 0; i < n; i++) {
            int currentDigit = (A[i] / (int)pow(10, digit)) % 10;
            enqueue(buckets[currentDigit], A[i]);
        }
        
        int index = 0;
        for (int i = 0; i < 10; i++) {
            while (!isQueueEmpty(buckets[i])) {
                A[index++] = dequeue(buckets[i]);
            }
        }
    }
    
    for (int i = 0; i < 10; i++) free(buckets[i]);
}

// Bucket Sort
void bucket_sort(double A[], int n) {
    List** B = (List**)malloc(n * sizeof(List*));
    for (int i = 0; i < n; i++) B[i] = createList();
    
    for (int i = 0; i < n; i++) {
        int bucket_index = (int)(n * A[i]);
        insertSorted(B[bucket_index], A[i]);
    }
    
    int index = 0;
    for (int i = 0; i < n; i++) {
        ListNode* current = B[i]->head;
        while (current != NULL) {
            A[index++] = current->data;
            current = current->next;
        }
    }
    
    for (int i = 0; i < n; i++) freeList(B[i]);
    free(B);
}

// ==================== GENERACIÓN DE DATOS PARA PLOTS ====================

void generate_complexity_data() {
    printf("Generando datos de complejidad...\n");
    
    FILE *fp_counting = fopen("counting_sort_data.txt", "w");
    FILE *fp_radix = fopen("radix_sort_data.txt", "w");
    FILE *fp_bucket = fopen("bucket_sort_data.txt", "w");
    FILE *fp_theoretical = fopen("theoretical_data.txt", "w");
    
    fprintf(fp_counting, "# n tiempo k\n");
    fprintf(fp_radix, "# n tiempo d\n");
    fprintf(fp_bucket, "# n tiempo\n");
    fprintf(fp_theoretical, "# n O(n) O(nk) O(nd) O(n^2)\n");
    
    int sizes[] = {100, 500, 1000, 2000, 5000, 10000, 20000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    
    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        
        // === COUNTING SORT ===
        int *arr_counting = (int*)malloc(n * sizeof(int));
        int k_counting = n / 2; // rango de valores
        for (int i = 0; i < n; i++) {
            arr_counting[i] = rand() % k_counting;
        }
        
        clock_t start = clock();
        counting_sort(arr_counting, n, k_counting);
        clock_t end = clock();
        double time_counting = ((double)(end - start)) / CLOCKS_PER_SEC;
        fprintf(fp_counting, "%d %.6f %d\n", n, time_counting, k_counting);
        
        // === RADIX SORT ===
        int *arr_radix = (int*)malloc(n * sizeof(int));
        int d_radix = 4; // dígitos fijos
        for (int i = 0; i < n; i++) {
            arr_radix[i] = 1000 + rand() % 9000; // 4 dígitos
        }
        
        start = clock();
        radix_sort_queue(arr_radix, n, d_radix);
        end = clock();
        double time_radix = ((double)(end - start)) / CLOCKS_PER_SEC;
        fprintf(fp_radix, "%d %.6f %d\n", n, time_radix, d_radix);
        
        // === BUCKET SORT ===
        double *arr_bucket = (double*)malloc(n * sizeof(double));
        for (int i = 0; i < n; i++) {
            arr_bucket[i] = (double)rand() / RAND_MAX; // [0,1)
        }
        
        start = clock();
        bucket_sort(arr_bucket, n);
        end = clock();
        double time_bucket = ((double)(end - start)) / CLOCKS_PER_SEC;
        fprintf(fp_bucket, "%d %.6f\n", n, time_bucket);
        
        // === DATOS TEÓRICOS ===
        double theoretical_n = n;
        double theoretical_nk = n * k_counting / 1000.0; // escalado
        double theoretical_nd = n * d_radix / 1000.0; // escalado
        double theoretical_n2 = (n * n) / 1000000.0; // escalado
        
        fprintf(fp_theoretical, "%d %.6f %.6f %.6f %.6f\n", 
                n, theoretical_n/1000, theoretical_nk/100, theoretical_nd/100, theoretical_n2);
        
        free(arr_counting);
        free(arr_radix);
        free(arr_bucket);
        
        printf("Completado n = %d\n", n);
    }
    
    fclose(fp_counting);
    fclose(fp_radix);
    fclose(fp_bucket);
    fclose(fp_theoretical);
    
    printf("Datos generados exitosamente!\n");
}

// ==================== GENERACIÓN DE SCRIPTS GNUPLOT ====================

void generate_gnuplot_scripts() {
    printf("Generando scripts de GNUplot...\n");
    
    // Script para gráfica comparativa
    FILE *fp_script = fopen("plot_complexity.gnu", "w");
    fprintf(fp_script, 
        "set terminal pngcairo size 1200,800 enhanced font 'Arial,12'\n"
        "set output 'complexity_comparison.png'\n\n"
        "set title 'Complejidad Asintótica - Counting Sort vs Radix Sort vs Bucket Sort'\n"
        "set xlabel 'Tamaño del Array (n)'\n"
        "set ylabel 'Tiempo (segundos)'\n"
        "set logscale xy\n"
        "set grid\n"
        "set key left top\n\n"
        "plot 'counting_sort_data.txt' using 1:2 with linespoints title 'Counting Sort O(n+k)', \\\n"
        "     'radix_sort_data.txt' using 1:2 with linespoints title 'Radix Sort O(dn)', \\\n"
        "     'bucket_sort_data.txt' using 1:2 with linespoints title 'Bucket Sort O(n)', \\\n"
        "     'theoretical_data.txt' using 1:2 with lines title 'O(n) teórico' linewidth 2, \\\n"
        "     'theoretical_data.txt' using 1:3 with lines title 'O(nk) teórico' linewidth 2, \\\n"
        "     'theoretical_data.txt' using 1:4 with lines title 'O(nd) teórico' linewidth 2, \\\n"
        "     'theoretical_data.txt' using 1:5 with lines title 'O(n^2) teórico' linewidth 2\n"
    );
    fclose(fp_script);
    
    // Script individual para Counting Sort
    fp_script = fopen("plot_counting.gnu", "w");
    fprintf(fp_script,
        "set terminal pngcairo size 800,600 enhanced font 'Arial,12'\n"
        "set output 'counting_sort_complexity.png'\n\n"
        "set title 'Counting Sort - Complejidad O(n + k)'\n"
        "set xlabel 'Tamaño del Array (n)'\n"
        "set ylabel 'Tiempo (segundos)'\n"
        "set grid\n"
        "set key left top\n\n"
        "plot 'counting_sort_data.txt' using 1:2 with linespoints title 'Tiempo experimental', \\\n"
        "     'theoretical_data.txt' using 1:3 with lines title 'O(n + k) teórico' linewidth 2\n"
    );
    fclose(fp_script);
    
    // Script individual para Radix Sort
    fp_script = fopen("plot_radix.gnu", "w");
    fprintf(fp_script,
        "set terminal pngcairo size 800,600 enhanced font 'Arial,12'\n"
        "set output 'radix_sort_complexity.png'\n\n"
        "set title 'Radix Sort - Complejidad O(d × n)'\n"
        "set xlabel 'Tamaño del Array (n)'\n"
        "set ylabel 'Tiempo (segundos)'\n"
        "set grid\n"
        "set key left top\n\n"
        "plot 'radix_sort_data.txt' using 1:2 with linespoints title 'Tiempo experimental', \\\n"
        "     'theoretical_data.txt' using 1:4 with lines title 'O(d × n) teórico' linewidth 2\n"
    );
    fclose(fp_script);
    
    // Script individual para Bucket Sort
    fp_script = fopen("plot_bucket.gnu", "w");
    fprintf(fp_script,
        "set terminal pngcairo size 800,600 enhanced font 'Arial,12'\n"
        "set output 'bucket_sort_complexity.png'\n\n"
        "set title 'Bucket Sort - Complejidad O(n)'\n"
        "set xlabel 'Tamaño del Array (n)'\n"
        "set ylabel 'Tiempo (segundos)'\n"
        "set grid\n"
        "set key left top\n\n"
        "plot 'bucket_sort_data.txt' using 1:2 with linespoints title 'Tiempo experimental', \\\n"
        "     'theoretical_data.txt' using 1:2 with lines title 'O(n) teórico' linewidth 2\n"
    );
    fclose(fp_script);
    
    printf("Scripts de GNUplot generados!\n");
}

// ==================== EJECUCIÓN AUTOMÁTICA ====================

void run_gnuplot_scripts() {
    printf("Ejecutando GNUplot...\n");
    system("gnuplot plot_complexity.gnu");
    system("gnuplot plot_counting.gnu");
    system("gnuplot plot_radix.gnu");
    system("gnuplot plot_bucket.gnu");
    printf("Gráficas generadas como PNG files!\n");
}

// ==================== INSTRUCCIONES ====================

void print_instructions() {
    printf("\n===================================================\n");
    printf("INSTRUCCIONES PARA USAR EL SISTEMA DE PLOTTING\n");
    printf("===================================================\n\n");
    
    printf("ARCHIVOS GENERADOS:\n");
    printf("1. counting_sort_data.txt    - Datos experimentales Counting Sort\n");
    printf("2. radix_sort_data.txt       - Datos experimentales Radix Sort\n");
    printf("3. bucket_sort_data.txt      - Datos experimentales Bucket Sort\n");
    printf("4. theoretical_data.txt      - Datos teóricos de complejidad\n");
    printf("5. plot_complexity.gnu       - Script gráfica comparativa\n");
    printf("6. plot_counting.gnu         - Script Counting Sort individual\n");
    printf("7. plot_radix.gnu            - Script Radix Sort individual\n");
    printf("8. plot_bucket.gnu           - Script Bucket Sort individual\n\n");
    
    printf("GRÁFICAS GENERADAS:\n");
    printf("1. complexity_comparison.png - Comparación de los 3 algoritmos\n");
    printf("2. counting_sort_complexity.png - Counting Sort individual\n");
    printf("3. radix_sort_complexity.png    - Radix Sort individual\n");
    printf("4. bucket_sort_complexity.png   - Bucket Sort individual\n\n");
    
    printf("COMPLEJIDADES TEÓRICAS INCLUIDAS:\n");
    printf("✓ Counting Sort: O(n + k)\n");
    printf("✓ Radix Sort: O(d × n)\n");
    printf("✓ Bucket Sort: O(n) (caso promedio)\n");
    printf("✓ También se incluye O(n²) como referencia\n\n");
}

// ==================== PROGRAMA PRINCIPAL ====================

int main() {
    srand(time(NULL));
    
    printf("===================================================\n");
    printf("SISTEMA DE ANÁLISIS DE COMPLEJIDAD ASINTÓTICA\n");
    printf("Counting Sort vs Radix Sort vs Bucket Sort\n");
    printf("===================================================\n\n");
    
    // Generar datos y scripts
    generate_complexity_data();
    generate_gnuplot_scripts();
    
    // Ejecutar GNUplot (comentar si no tienes GNUplot instalado)
    run_gnuplot_scripts();
    
    // Mostrar instrucciones
    print_instructions();
    
    printf("¡Análisis completado! Revisa los archivos PNG generados.\n");
    
    return 0;
}