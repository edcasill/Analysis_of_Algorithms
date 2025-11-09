#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>


void print_array(int arr[], int n)
{
    printf("Your array is:");
    for (int i = 0; i < n; i++)
    {
        printf(" %d ", arr[i]);
    }
    printf("\n");
}


// *** ESTRUCTURAS DE NODO PARA QUEUES ***//
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
    if (q->front == NULL) {
        return -1;
    }
    
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


void radix_sort_queue(int A[], int n, int d)
{
    //Crea 10 queues (0-9) para cada digito
    // O(10) = 0(1)
    Queue* buckets[10];
    for (int i = 0; i < 10; i++) {
        buckets[i] = createQueue();
    }

    // Para cada digito (del menos significativo al mas significativo)
    //O(d * (n + 10)), donde d=numero max de digitos
    for (int digit = 0; digit < d; digit++) {  //O(d) iteraciones
        // Distribuye los elementos en las queues segun el digito actual
        //O(n)
        for (int i = 0; i < n; i++) {
            int currentDigit = (A[i] / (int)pow(10, digit)) % 10;
            enqueue(buckets[currentDigit], A[i]);  //O(1) por cada operacion
        }

        // Recolecta los elementos de las queues en orden
        //O(n + 10)
        int index = 0;
        for (int i = 0; i < 10; i++) {
            while (!isQueueEmpty(buckets[i])) {
                A[index++] = dequeue(buckets[i]);
            }
        }
    }

    // Libera la memoria de las queues
    // O(10) = O(1)
    for (int i = 0; i < 10; i++) {
        free(buckets[i]);
    }

    /*
    T(n, d) = O(1) + d × [O(n) + O(n + 10)]
        = O(1) + d × [O(2n + 10)]
        = O(1) + O(2dn + 10d)
        = O(dn + d)
    */
}


void test_complexity() {
    printf("=== VERIFICACION DE COMPLEJIDAD ===\n\n");

    printf("Comprobando que T(n,d) es proporcional a d * n:\n\n");
    
    int test_cases[][2] = {
        {1000, 2}, {1000, 4}, {1000, 6},
        {5000, 2}, {5000, 4}, {5000, 6},
        {10000, 2}, {10000, 4}, {10000, 6}
    };
    int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
    
    printf("%-8s %-8s %-12s %-15s %s\n", "n", "d", "Tiempo(s)", "d * n", "T/(d*n)");
    printf("------------------------------------------------------------\n");
    
    for (int i = 0; i < num_cases; i++) {
        int n = test_cases[i][0];
        int d = test_cases[i][1];
        int *array = (int*)malloc(n * sizeof(int));
        
        int max_value = (int)pow(10, d) - 1;
        int min_value = (d > 1) ? (int)pow(10, d-1) : 0;
        
        // Llenar array
        for (int j = 0; j < n; j++) {
            array[j] = min_value + rand() % (max_value - min_value + 1);
        }
        
        clock_t start = clock();
        radix_sort_queue(array, n, d); // Tu función directa
        clock_t end = clock();
        
        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        double product = d * n;
        double ratio = time_taken / product;
        
        printf("%-8d %-8d %-12.6f %-15.0f %.10f\n", 
               n, d, time_taken, product, ratio);
        
        free(array);
    }
}


int main() {
    int arr_queue[] = {170, 45, 75, 90, 2, 802, 24, 66};
    int data_radix = sizeof(arr_queue)/sizeof(arr_queue[0]);

    printf("===RADIX SORT===\n");
    srand(time(NULL));
    print_array(arr_queue, data_radix);

    struct timeval tv0, tv1;
    long elapsed_time;
    gettimeofday(&tv0, NULL);

    radix_sort_queue(arr_queue,data_radix, 3);
    printf("Ordered array\n");
    print_array(arr_queue, data_radix);
    test_complexity();
    gettimeofday(&tv1, NULL);
    elapsed_time = (tv1.tv_sec - tv0.tv_sec)*1000000 + (tv1.tv_usec - tv0.tv_usec);
    printf("execution time: %ld microseconds\n", elapsed_time);

    return 0;
}
