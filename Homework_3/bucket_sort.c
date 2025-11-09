#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

// *** ESTRUCTURAS PARA LISTAS ENLAZADAS (BUCKETS) ***
typedef struct Node {
    double data;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
} List;

// *** FUNCIONES PARA MANEJAR LISTAS ***
List* createList() {
    List* list = (List*)malloc(sizeof(List));
    list->head = NULL;
    return list;
}

void insertSorted(List* list, double value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = NULL;

    // Si la lista esta vacia o el valor es menor que el head
    if (list->head == NULL || value < list->head->data) {
        newNode->next = list->head;
        list->head = newNode;
        return;
    }

    // Buscar la posición correcta (Insertion Sort)
    Node* current = list->head;
    while (current->next != NULL && current->next->data < value) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
}

void freeList(List* list) {
    Node* current = list->head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
}

// *** IMPLEMENTACION DE BUCKET SORT ***
void bucket_sort(double A[], int n) {
    // Paso 1: Crear array B de buckets
    List** B = (List**)malloc(n * sizeof(List*));

    // Pasos 2-3: Inicializar buckets como listas vacías
    for (int i = 0; i < n; i++) {
        B[i] = createList();
    }

    // Pasos 4-5: Distribuir elementos en los buckets
    for (int i = 0; i < n; i++) {
        int bucket_index = (int)(n * A[i]); // ⌊n * A[i]⌋
        insertSorted(B[bucket_index], A[i]);
    }

    // Pasos 6-8: Concatenar buckets en orden
    int index = 0;
    for (int i = 0; i < n; i++) {
        Node* current = B[i]->head;
        while (current != NULL) {
            A[index++] = current->data;
            current = current->next;
        }
    }

    // Liberar memoria
    for (int i = 0; i < n; i++) {
        freeList(B[i]);
    }
    free(B);
}


// *** FUNCIONES AUXILIARES ***
void print_array(double arr[], int n) {
    printf("Array: ");
    for (int i = 0; i < n; i++) {
        printf("%.3f ", arr[i]);
    }
    printf("\n");
}


int main() {
    printf("=== BUCKET SORT ===\n\n");

    // Ejemplo 1: Numeros entre [0, 1)
    double A[] = {0.78, 0.17, 0.39, 0.26, 0.72, 0.94, 0.21, 0.12, 0.23, 0.68};
    int n = sizeof(A) / sizeof(A[0]);

    printf("Ejemplo 1 - Distribucion uniforme [0,1):\n");
    printf("Original: ");
    print_array(A, n);

    struct timeval tv0, tv1;
    long elapsed_time;
    gettimeofday(&tv0, NULL);

    bucket_sort(A, n);

    printf("Ordenado: ");
    print_array(A, n);

    gettimeofday(&tv1, NULL);
    elapsed_time = (tv1.tv_sec - tv0.tv_sec)*1000000 + (tv1.tv_usec - tv0.tv_usec);
    printf("execution time: %ld microseconds\n", elapsed_time);


    return 0;
}