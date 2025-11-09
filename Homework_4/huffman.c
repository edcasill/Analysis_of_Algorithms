#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <limits.h>

typedef struct Node
{
    /*
    Properties for the nodes
    */
    unsigned freq;
    char data;
    struct Node *left, *right;
}Node;

Node* createNode(char data, unsigned freq)
{
    /*
    Creates a new node for Huffman
    */
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->freq = freq;
    newNode->left = newNode->right = NULL;
    return newNode;
}

typedef struct min_heap {
    unsigned size;
    unsigned capacity;
    Node** array;
} min_heap;

int parent(int i)
{return (i-1)/2;}

int left(int i)
{return 2*i+1;}

int right(int i)
{return 2*i+2;}

min_heap* create_min_heap(unsigned capacity)
{
    min_heap* mini_heap = (min_heap*)malloc(sizeof(min_heap));
    mini_heap->size = 0;
    mini_heap->capacity = capacity;
    mini_heap->array = (Node**)malloc(mini_heap->capacity * sizeof(Node*));
    return mini_heap;
}

void min_heapify(min_heap *mini_heap, int i)
{
    int l = left(i);
    int r = right(i);
    int smallest;

    if (l < mini_heap->size && mini_heap->array[l]->freq < mini_heap->array[i]->freq)
    {
        smallest = l;
    }
    else
    {
        smallest = i;
    }
    if (r < mini_heap->size && mini_heap->array[r]->freq < mini_heap->array[smallest]->freq)
    {
        smallest = r;
    }
    if (smallest != i)
    {
        //exchange nodes
        Node* temp = mini_heap->array[i];
        mini_heap->array[i] = mini_heap->array[smallest];
        mini_heap->array[smallest] = temp;
        min_heapify(mini_heap, smallest);
    }
}

void build_min_heap(min_heap* mini_heap)
{
    mini_heap->size = mini_heap->capacity;
    for (int i = mini_heap->capacity/2-1; i >= 0; i--)
    {
        min_heapify(mini_heap, i);
    }
}

Node* min_heap_extract_min(min_heap* mini_heap)
{
    if (mini_heap->size < 1)
    {
        printf("heap overflow\n");
        return NULL;
    }
    
    Node* min = mini_heap->array[0];
    mini_heap->array[0] = mini_heap->array[mini_heap->size - 1];
    mini_heap->size--;
    min_heapify(mini_heap, 0);
    return min;
}

void min_insert(min_heap* mini_heap, Node* x)
{
    if (mini_heap->size >= mini_heap->capacity)
    {
        printf("Heap overflow\n");
        return;
    }
    
    int i = mini_heap->size;
    mini_heap->array[i] = x;
    mini_heap->size++;
    
    // Ajustar hacia arriba
    while (i > 0 && mini_heap->array[parent(i)]->freq > mini_heap->array[i]->freq)
    {
        // Intercambiar con el padre
        Node* temp = mini_heap->array[i];
        mini_heap->array[i] = mini_heap->array[parent(i)];
        mini_heap->array[parent(i)] = temp;
        i = parent(i);
    }
}


Node* huffman(char data[], unsigned freq[], int n)
{
    min_heap* Q = create_min_heap(n);

    //Q = C
    for (int i = 0; i < n; i++)
    {
        Q->array[i] = createNode(data[i], freq[i]);
    }
    Q->size = n;
    build_min_heap(Q);

    for (int i = 1; i <= n-1; i++)
    {
        /* allocate a new node z */
        Node* z = createNode('$', 0); //$ indicates internal node
        z->left = min_heap_extract_min(Q);
        z->right = min_heap_extract_min(Q);
        z->freq = z->left->freq + z->right->freq;
        min_insert(Q,z);
    }
    return min_heap_extract_min(Q);
}

void print_huffman(Node* root, int arr[], int top) {
    if (root->left) {
        arr[top] = 0;
        print_huffman(root->left, arr, top + 1);
    }

    if (root->right) {
        arr[top] = 1;
        print_huffman(root->right, arr, top + 1);
    }

    if (!root->left && !root->right) {
        printf("'%c': ", root->data);
        for (int i = 0; i < top; i++) {
            printf("%d", arr[i]);
        }
        printf("\n");
    }
}


void free_Huffman(Node* root) {
    if (root == NULL) return;
    free_Huffman(root->left);
    free_Huffman(root->right);
    free(root);
}

void run_test(int n) {
    //Generate data test
    char* data = (char*)malloc(n * sizeof(char));
    unsigned* freq = (unsigned*)malloc(n * sizeof(unsigned));

    if (!data || !freq) {
        printf("Error allocating memory for n=%d\n", n);
        free(data);
        free(freq);
        return;
    }

    for (int i = 0; i < n; i++) {
        data[i] = (char)((i % 95) + 32); //ascii characters
        freq[i] = (rand() % 1000) + 1;    // Random frequencies between 1 and 1000
    }

    //Measure the running time of huffman()
    struct timeval start, end;
    double time_taken;

    gettimeofday(&start, NULL); //Start timer
    
    Node* root = huffman(data, freq, n); //Execute algorithm
    
    gettimeofday(&end, NULL); // Detener temporizador

    //Calculate the elapsed time in seconds
    time_taken = (end.tv_sec - start.tv_sec) * 1e6; // seg t microseg
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6; // microseg to seg

    printf("%d, %f\n", n, time_taken);

    //free memory
    free_Huffman(root);
    free(data);
    free(freq);
}


int main()
{
    /*
    char data[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    unsigned freq[] = {45, 13, 12, 16, 9, 5};
    int n = sizeof(data) / sizeof(data[0]);

    printf("Character freq: \n");
    for (int i=0; i<n; i++)
    {
        printf("%c: %u\n", data[i], freq[i]);
    }

    printf("Huffman tree\n");
    Node* root = huffman(data, freq, n);

    printf("\n");
    printf("Huffman codes: \n");
    int arr[100], top = 0;
    print_huffman(root, arr, top);

    printf("\n");
    printf("Frequency from root: %u\n", root->freq);

    free_Huffman(root);
    */
   //Initialize the random number generator
    srand(time(NULL));

    //Define the sizes of n that we want to test
    int test_sizes[] = {100, 500, 1000, 5000, 10000, 20000, 50000, 100000};
    int num_tests = sizeof(test_sizes) / sizeof(test_sizes[0]);

    printf("n, time_taken_sec\n");
    for (int i = 0; i < num_tests; i++) {
        run_test(test_sizes[i]);
    }

    return 0;
}
