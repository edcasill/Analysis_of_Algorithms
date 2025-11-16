#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <math.h>

#define INF INT_MAX
#define NIL -1

typedef struct Vertex
{
    int d;           //shortest path
    int pi;          //predecessor
    int visited;     //if it was already used
} Vertex;

typedef struct Edge
{
    int u, v, weight;
} Edge;

//adjacency list to improve the complexity
typedef struct AdjListNode
{
    int v; //destination
    int weight;
    struct AdjListNode* next; //next edge
} AdjListNode;

typedef struct AdjList
{
    AdjListNode *head;
} AdjList;

typedef struct Graph
{
    int vertex_num;
    int edges_num;
    Vertex *vertices;
    AdjList *array; //adjacency lists
} Graph;

//heap structure for min-heap
typedef struct HeapNode
{
    int vertex;      //vertex index
    int key;         //distance
} HeapNode;

typedef struct min_heap
{
    int size;
    int capacity;
    HeapNode **array;
    int *pos; //map vertex -> position on heap
} min_heap;

HeapNode* createHeapNode(int vertex, int key)
{
    HeapNode* node = (HeapNode*)malloc(sizeof(HeapNode));
    node->vertex = vertex;
    node->key = key;

    return node;
}

min_heap* create_min_heap(int capacity)
{
    min_heap* heap = (min_heap*)malloc(sizeof(min_heap));
    heap->size = 0;
    heap->capacity = capacity;
    heap->array = (HeapNode**)malloc(capacity * sizeof(HeapNode*));
    heap->pos = (int*)malloc(capacity * sizeof(int));

    return heap;
}

void swap_nodes(HeapNode** a, HeapNode** b)
{
    HeapNode* temp = *a;
    *a = *b;
    *b = temp;
}

void min_heapify(min_heap *heap, int i)
{
    int l = 2*i + 1;
    int r = 2*i + 2;
    int smallest = i;

    if (l < heap->size && heap->array[l]->key < heap->array[i]->key)
    {
        smallest = l;
    }
    if (r < heap->size && heap->array[r]->key < heap->array[smallest]->key)
    {
        smallest = r;
    }
    if (smallest != i)
    {
        //update positions
        heap->pos[heap->array[i]->vertex] = smallest;
        heap->pos[heap->array[smallest]->vertex] = i;
        swap_nodes(&heap->array[i], &heap->array[smallest]);
        min_heapify(heap, smallest);
    }
}

HeapNode* extract_min(min_heap* heap)
{
    if (heap->size <= 0)
    {
        return NULL;
    }

    HeapNode* root = heap->array[0];
    HeapNode* last = heap->array[heap->size - 1];
    heap->array[0] = last;

    //update positions
    heap->pos[root->vertex] = heap->size - 1;
    heap->pos[last->vertex] = 0;
    heap->size--;
    min_heapify(heap, 0);

    return root;
}

void decrease_key(min_heap* heap, int vertex, int new_key) {
    int i = heap->pos[vertex];
    heap->array[i]->key = new_key;

    while (i > 0 && heap->array[(i-1)/2]->key > heap->array[i]->key)
    {
        heap->pos[heap->array[i]->vertex] = (i-1)/2;
        heap->pos[heap->array[(i-1)/2]->vertex] = i;
        swap_nodes(&heap->array[i], &heap->array[(i-1)/2]);
        i = (i-1)/2;
    }
}

void insert_heap(min_heap* heap, int vertex, int key)
{
    if (heap->size >= heap->capacity)
    {
        printf("Heap overflow\n");
        return;
    }

    int i = heap->size;
    heap->array[i] = createHeapNode(vertex, key);
    heap->pos[vertex] = i;
    heap->size++;

    while (i > 0 && heap->array[(i-1)/2]->key > heap->array[i]->key)
    {
        heap->pos[heap->array[i]->vertex] = (i-1)/2;
        heap->pos[heap->array[(i-1)/2]->vertex] = i;
        swap_nodes(&heap->array[i], &heap->array[(i-1)/2]);
        i = (i-1)/2;
    }
}

int is_in_heap(min_heap* heap, int vertex)
{
    return heap->pos[vertex] < heap->size;
}

void initialize_single_source(Graph *g, int s)
{
    for (int i = 0; i < g->vertex_num; i++)
    {
        g->vertices[i].d = INF;
        g->vertices[i].pi = NIL;
        g->vertices[i].visited = 0;
    }
    g->vertices[s].d = 0;
}

void relax(Graph *g, min_heap *heap, int u, int v, int w)
{
    if (g->vertices[u].d != INF && 
        g->vertices[v].d > g->vertices[u].d + w) {
        g->vertices[v].d = g->vertices[u].d + w;
        g->vertices[v].pi = u;

        if (is_in_heap(heap, v))
        {
            decrease_key(heap, v, g->vertices[v].d);
        }
    }
}

void dijkstra(Graph *g, int s)
{
    initialize_single_source(g, s);
    min_heap* heap = create_min_heap(g->vertex_num);

    for (int i = 0; i < g->vertex_num; i++)
    {
        insert_heap(heap, i, g->vertices[i].d);
    }
    while (heap->size > 0)
    {
        HeapNode* min_node = extract_min(heap);
        int u = min_node->vertex;
        g->vertices[u].visited = 1;
        AdjListNode* crawler = g->array[u].head;
        while (crawler != NULL)
        {
            int v = crawler->v;
            int w = crawler->weight;

            if (!g->vertices[v].visited)
            {
                relax(g, heap, u, v, w);
            }
            crawler = crawler->next;
        }
        free(min_node);
    }
    free(heap->array);
    free(heap->pos);
    free(heap);
}

Graph* create_graph(int vertex_num) {
    Graph *g = (Graph*)malloc(sizeof(Graph));
    g->vertex_num = vertex_num;
    g->edges_num = 0;
    g->vertices = (Vertex*)malloc(vertex_num * sizeof(Vertex));
    g->array = (AdjList*)malloc(vertex_num * sizeof(AdjList));

    for (int i = 0; i < vertex_num; i++)
    {
        g->array[i].head = NULL;
    }
    return g;
}

AdjListNode* newAdjListNode(int v, int weight) {
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->v = v;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

void add_edge(Graph* g, int u, int v, int weight) {
    AdjListNode* newNode = newAdjListNode(v, weight);
    newNode->next = g->array[u].head;
    g->array[u].head = newNode;
    g->edges_num++;
}

void print_graph(Graph *g) {
    printf("Vertices: %d, Edges: %d\n", g->vertex_num, g->edges_num);

    for (int u = 0; u < g->vertex_num; u++)
    {
        AdjListNode* crawler = g->array[u].head; //crawler will move across the adjacency list

        //if there are no edges, don't print them
        if (crawler == NULL)
        {
            continue; 
        }
        printf("  (%d) -> ", u); //print origin

        while (crawler != NULL) {
            printf("(%d, w:%d) | ", crawler->v, crawler->weight);
            crawler = crawler->next;
        }
        printf("\n");
    }
    printf("------------------------------\n\n");
}

void print_path(Graph *g, int s, int v)
{
    if (v == s)//if v is origin, print origin
    {
        printf("%d", s);
    } 
    else if (g->vertices[v].pi == NIL) //if predecessor is NIL, no path
    {
        printf("(no path from %d)", s); 
    }
    else
    {
        print_path(g, s, g->vertices[v].pi);
        printf(" -> %d", v); //print actual vertex
    }
}
void free_graph(Graph *g)
{
    if (g == NULL) return;

    for (int i = 0; i < g->vertex_num; i++)
    {
        AdjListNode* crawler = g->array[i].head;
        //free each node from adjacency list
        while (crawler != NULL)
        {
            AdjListNode* temp = crawler; //save actual node
            crawler = crawler->next; //move to next one
            free(temp); //free actual node
        }
    }
    free(g->array);
    free(g->vertices);
    free(g);
}

Graph* create_random_graph(int V, int E)
{
    Graph* g = create_graph(V);

    for (int i = 0; i < E; i++)
    {
        int u = rand() % V;
        int v = rand() % V;
        int weight = (rand() % 100) + 1;

        if (u != v)
        {
            add_edge(g, u, v, weight);
        }
        else
        {
            i--;
        }
    }
    return g;
}

void benchmark_dijkstra(FILE* csvFile)
{
    int test_cases_V[] = { 1000, 5000, 10000, 20000, 40000, 50000 };
    int num_tests = sizeof(test_cases_V) / sizeof(test_cases_V[0]);
    int RUNS_PER_TEST = 20;

    for (int i = 0; i < num_tests; i++) {
        int V = test_cases_V[i];
        int E = V * 5; 

        printf("Generating graph with V=%d, E=%d...\n", V, E);
        Graph* g = create_random_graph(V, E);

        double total_time_for_runs = 0.0;

        for (int j = 0; j < RUNS_PER_TEST; j++)
        {
            clock_t start = clock();
            dijkstra(g, 0);
            clock_t end = clock();

            if (j < RUNS_PER_TEST - 1)
            { 
                initialize_single_source(g, 0); 
            }
            total_time_for_runs += ((double)(end - start)) / CLOCKS_PER_SEC;
        }
        double average_time_used = total_time_for_runs / RUNS_PER_TEST;
        double theoretical_work = (double)V * log2((double)V);

        printf("  Average time: %.4f seg.\n", average_time_used);
        fprintf(csvFile, "%d,%d,%.2f,%.10f\n", V, E, theoretical_work, average_time_used);
        free_graph(g);
    }
}

int main() {
    srand(time(NULL));
    int source = 0;
    Graph *g = create_graph(5);
    
    //origin - destination - weight
    add_edge(g, 0, 1, 10);
    add_edge(g, 0, 2, 5);
    add_edge(g, 1, 2, 2);
    add_edge(g, 1, 3, 1);
    add_edge(g, 2, 1, 3);
    add_edge(g, 2, 3, 9);
    add_edge(g, 2, 4, 2);
    add_edge(g, 3, 4, 4);
    add_edge(g, 4, 3, 6);

    printf("initial graph\n");
    print_graph(g);
    dijkstra(g, 0);

    printf("distances from vertex 0:\n");
    for (int i = 0; i < g->vertex_num; i++)
    {
        printf("vertex %d: %d (predecessor: %d)  | ", i, g->vertices[i].d, g->vertices[i].pi);
        if (g->vertices[i].d == INF) {
            printf("vertex %d: [INF] \t(no path)\n", i);
        }
        else
        {
            printf("\tpath: ");
            print_path(g, source, i);
            printf("\n");
        }
    }
    free_graph(g);

    /*analysis*/
    FILE* f = fopen("dijkstra_performance.csv", "w");
    if (f == NULL) {
        printf("Error to open\n");
        return 1;
    }
    fprintf(f, "V,E,V_log_V,TotalTime\n");
    benchmark_dijkstra(f);
    fclose(f);

    return 0;
}