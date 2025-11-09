#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <windows.h>

typedef struct Node { // Assuming constant time
    int value;
    struct Node *next;
} Node;

typedef struct Queue { // Assuming constant time
    Node *front, *rear;
} Queue;

void initQueue(Queue *q) { // Assuming constant time
    q->front = NULL;
    q->rear = NULL;
}

int isEmpty(Queue *q) { // Assuming constant time
    return q->front == NULL;
}

void enqueue(Queue *q, int v) { // Assuming constant time
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->value = v;
    newNode->next = NULL;
    if (q->rear == NULL) {
        q->front = newNode;
        q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

int dequeue(Queue *q) { // Assuming constant time
    if (isEmpty(q))
        return -1;
    Node *temp = q->front;
    int value = temp->value;
    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;
    free(temp);
    return value;
}

void radix_sort(int A[], int n, int d) {
    Queue buckets[10]; // 1
    for (int k = 0; k < 10; k++) // 11 = k + 1 = possible values
        initQueue(&buckets[k]); // 10
    int exp = 1; // 1
    for (int i = 0; i < d; i++) { // d + 1
        for (int j = 0; j < n; j++) { // sum(i=0->d) n + 1
            int digit = (A[j] / exp) % 10; // sum(i=0->d) n
            enqueue(&buckets[digit], A[j]); // sum(i=0->d) n
        }
        int index = 0; // d
        for (int k = 0; k < 10; k++) { // sum(i=0->d) 11 = sum(i=0->d) k+1
            while (!isEmpty(&buckets[k])) // sum(i=0->d) k
                A[index++] = dequeue(&buckets[k]); // sum(i=0->d) 1
        }
        exp *= 10; // d
    }
}

// Export Windows library (DLL)
__declspec(dllexport) double radix_sort_c(int *A, int n, int d) {
    struct timeval tv_start, tv_end;
    gettimeofday(&tv_start, NULL);
    radix_sort(A, n, d);
    gettimeofday(&tv_end, NULL);
    long ms = ((tv_end.tv_sec - tv_start.tv_sec) * 1000000L + (tv_end.tv_usec - tv_start.tv_usec)) / 1000;
    return ms;
}

/*
int main() {
    struct timeval tv_start, tv_end;
    int n, d;
    scanf("%d %d", &n, &d);
    int A[n];
    for (int i = 0; i < n; i++)
        scanf("%d", &A[i]);

    gettimeofday(&tv_start, NULL);
    radix_sort(A, n, d);
    gettimeofday(&tv_end, NULL);

    long ms = ((tv_end.tv_sec - tv_start.tv_sec) * 1000000L + (tv_end.tv_usec - tv_start.tv_usec)) / 1000;
    printf("%ld\n", ms);

    for (int i = 0; i < n; i++)
        printf("%d ", A[i]);

    return 0;
}
*/
