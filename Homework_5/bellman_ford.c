#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define INF INT_MAX
#define NIL -1

//vertex structure
typedef struct Vertex
{
    int d;       //shortest path from origin
    int pi;      //Predecessor
} Vertex;

//edge structure
typedef struct Edge
{
    int u; //index for origin vertex
    int v; //index for destination vertex
    int weight; //weight
} Edge;

// graph structure
typedef struct Graph
{
    int num_vertices; //# vertices
    int num_edges;    //# edges
    Vertex *vertices; //vertices array
    Edge *edges;      //edge array
} Graph;

//initialize predecessors and distances
void initialize_single_source(Graph *g, int s)
{
    for (int i = 0; i < g->num_vertices; i++)
    {
        g->vertices[i].d = INF;
        g->vertices[i].pi = NIL;
    }
    g->vertices[s].d = 0;
}

//relaxation for and edge
void relax(Graph *g, Edge edge) {
    if (g->vertices[edge.u].d != INF && g->vertices[edge.v].d > g->vertices[edge.u].d + edge.weight)
    {
        g->vertices[edge.v].d = g->vertices[edge.u].d + edge.weight;
        g->vertices[edge.v].pi = edge.u;
    }
}

int bellman_ford(Graph *g, int s)
{
    initialize_single_source(g, s);
    
    //relx edges |V|-1 times
    for (int i = 1; i <= g->num_vertices - 1; i++)
    {
        for (int j = 0; j < g->num_edges; j++)
        {
            relax(g, g->edges[j]);
        }
    }
    //negative cycles verification
    for (int i = 0; i < g->num_edges; i++)
    {
        Edge edge = g->edges[i];
        if (g->vertices[edge.u].d != INF && g->vertices[edge.v].d > g->vertices[edge.u].d + edge.weight)
        {
            return 0; 
        }
    }
    return 1; //TRUE -> No negative cycles
}

//small tests
Graph* create_graph(int num_vertices, int num_edges) {
    Graph *g = (Graph*)malloc(sizeof(Graph));
    g->num_vertices = num_vertices;
    g->num_edges = num_edges;
    g->vertices = (Vertex*)malloc(num_vertices * sizeof(Vertex));
    g->edges = (Edge*)malloc(num_edges * sizeof(Edge));
    return g;
}

//prety large test
Graph* create_large_random_graph(int num_vertices, int num_edges) {
    Graph *g = create_graph(num_vertices, num_edges);

    //random edges
    srand(time(NULL));
    for (int i = 0; i < num_edges; i++)
    {
        g->edges[i].u = rand() % num_vertices;
        g->edges[i].v = rand() % num_vertices;
        // Pesos entre -100 y 100 (incluye negativos para probar ciclos)
        g->edges[i].weight = (rand() % 201) - 100;
    }
    return g;
}

void benchmark_bellman_ford(FILE* csvFile)
{
    int test_cases[][2] = {
        {100, 1000},     // V*E = 100,000
        {500, 5000},     // V*E = 2,500,000
        {1000, 10000},   // V*E = 10,000,000
        {2000, 20000},   // V*E = 40,000,000
        {3000, 30000},   // V*E = 90,000,000
        {4000, 40000},   // V*E = 160,000,000
        {5000, 50000}    // V*E = 250,000,000
    };
    
    int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    
    for (int i = 0; i < num_tests; i++)
    {
        int V = test_cases[i][0];
        int E = test_cases[i][1];
        printf("Making graph with %d vertices and %d edges...\n", V, E);
        Graph *g = create_large_random_graph(V, E);

        clock_t start = clock();
        int result = bellman_ford(g, 0); //vertex 0 as origin
        clock_t end = clock();

        double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        long long V_times_E = (long long)V * E;

        printf("result: %s\n", result ? "without negative cycles" : "with negative cycles");
        printf("execution time: %.4f sec\n", cpu_time_used);
        printf("theoric complex O(V*E): O(%d*%d) = O(%d)\n\n", V, E, V * E);
        fprintf(csvFile, "%d,%d,%lld,%.10f\n", V, E, V_times_E, cpu_time_used);

        free(g->vertices);
        free(g->edges);
        free(g);
    }
}

void print_path(Graph *g, int s, int v) {
    
    // Base case 1: Si 'v' es el origen 's', solo imprime el origen.
    if (v == s) {
        printf("%d", s);
    } 
    // Base case 2: Si el predecesor es NIL, no hay camino.
    else if (g->vertices[v].pi == NIL) {
        // Esto se maneja en main, pero es una buena guarda.
        printf("(sin camino)"); 
    } 
    // Paso recursivo:
    else {
        // Llama recursivamente para el predecesor
        print_path(g, s, g->vertices[v].pi);
        // Después de que la llamada recursiva termine, imprime el vértice actual
        printf(" -> %d", v);
    }
}

int main() {
    srand(time(NULL));
    //create a graph (vertices and edges)
    Graph *g = create_graph(5, 8);
    //source, destination, weight
    /*
    int edges[8][3] = {
        {0, 1, 6},
        {0, 2, 7},
        {1, 2, 8},
        {1, 3, 5},
        {1, 4, -4},//neg
        {2, 3, -3},//neg
        {2, 4, 9},
        {3, 1, -2}//neg
    };**/
    // negative cycle: 1-2-3-1
    
    int edges[8][3] = {
        {0, 1, 6},
        {0, 2, 7},
        {1, 2, -8},
        {1, 3, 5},
        {1, 4, -4},//neg
        {2, 3, -3},//neg
        {2, 4, 9},
        {3, 1, -2}//neg
    };

    printf("Initial graph\n");
    //assign edges
    for (int i = 0; i < 8; i++) {
        g->edges[i].u = edges[i][0];
        g->edges[i].v = edges[i][1];
        g->edges[i].weight = edges[i][2];
        printf(" (%d) --(%d)--> (%d)\n", g->edges[i].u, g->edges[i].weight, g->edges[i].v);
    }

    printf("\n");
    int source = 0;

   if (bellman_ford(g, source))
    {
        printf("there are not cycles with negative weights\n");
        printf("distances and paths from vertex %d:\n", source);

        for (int i = 0; i < g->num_vertices; i++)
        {
            if (g->vertices[i].d == INF)
            {
                printf("vertex %d: [INF] (no path)\n", i);
            }
            else
            {
                printf("vertex %d: [%d] \tpath: ", i, g->vertices[i].d);
                print_path(g, source, i);
                printf("\n");
            }
        }
    }
    else
    {
        printf("there is a cycle with negative weight\n");
    }
    printf("\n");
    
    free(g->vertices);
    free(g->edges);
    free(g);

    /*analysis*/
    
    FILE* f = fopen("bellman_ford_performance.csv", "w");
    if (f == NULL) {
        printf("Error while openning\n");
        return 1;
    }
    fprintf(f, "V,E,V_times_E,TotalTime\n");
    benchmark_bellman_ford(f);
    fclose(f);
    printf("\nResults from analysis save as 'bellman_ford_performance.csv'\n");
    
    return 0;
}