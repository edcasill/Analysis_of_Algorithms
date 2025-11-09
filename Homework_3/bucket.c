#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <windows.h>

typedef struct Node {
    double value;
    struct Node *next;
} Node;

Node* create(double v) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = v;
    newNode->next = NULL;
    return newNode;
}

void insert(Node** head, double v) {
    Node* newNode = create(v);
    if (*head == NULL) {
        *head = newNode;
    } else {
        Node* temp = *head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newNode;
    }
}

int list_to_array(Node* head, double arr[]) {
    int count = 0;
    Node* temp = head;
    while (temp != NULL) {
        arr[count++] = temp->value;
        temp = temp->next;
    }
    return count;
}

void insertion_sort(double arr[], int n) {
    for (int i = 1; i < n; i++) {
        double key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void bucket_sort(double A[], int n) {
    Node *B[n];
    for (int i = 0; i < n; i++)
        B[i] = NULL;
    
    for (int i = 0; i < n; i++) {
        int index = (int)(n * A[i]);
        if (index >= n) index = n - 1;
        insert(&B[index], A[i]);
    }
    
    int k = 0;
    for (int i = 0; i < n; i++) {
        double temp[n];
        int size = list_to_array(B[i], temp);
        if (size > 0) {
            insertion_sort(temp, size);
            for (int j = 0; j < size; j++)
                A[k++] = temp[j];
        }
    }
}

__declspec(dllexport) double bucket_sort_c(double *A, int n) {
    struct timeval tv_start, tv_end;
    gettimeofday(&tv_start, NULL);
    bucket_sort(A, n);
    gettimeofday(&tv_end, NULL);
    long ms = ((tv_end.tv_sec - tv_start.tv_sec) * 1000000L + (tv_end.tv_usec - tv_start.tv_usec)) / 1000;
    return ms;
}

/*
int main() {
    struct timeval tv_start, tv_end;
    int n;
    scanf("%d", &n);
    double A[n];
    for (int i = 0; i < n; i++)
        scanf("%lf", &A[i]);

    gettimeofday(&tv_start, NULL);
    bucket_sort(A, n);
    gettimeofday(&tv_end, NULL);

    long ms = ((tv_end.tv_sec - tv_start.tv_sec) * 1000000L + (tv_end.tv_usec - tv_start.tv_usec)) / 1000;
    printf("%ld\n", ms);

    for (int i = 0; i < n; i++) 
        printf("%lf ", A[i]);
    printf("\n");

    return 0;
}
*/