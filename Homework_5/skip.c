#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

//special values
#define PLUS_INF INT_MAX
#define MINUS_INF INT_MIN

//for n elements, height O(log n)
#define MAX_LEVEL 32

//Quad-Node
typedef struct QuadNode
{
    int key;
    void* element;
    struct QuadNode* prev;
    struct QuadNode* next;
    struct QuadNode* above;
    struct QuadNode* below;
} QuadNode;

//skip list structure
typedef struct SkipList
{
    QuadNode* header; //pointer to header on superior list
    QuadNode* tail; //pointer to tail on superior list
    int height; //actual height(h)
    int size; //elements on S0
} SkipList;

//create new quad-node
QuadNode* createQuadNode(int key, void* element, int level)
{
    QuadNode* newNode = (QuadNode*)malloc(sizeof(QuadNode));
    newNode->key = key;
    newNode->element = element;
    newNode->prev = NULL;
    newNode->next = NULL;
    newNode->above = NULL;
    newNode->below = NULL;

    return newNode;
}

//initialice skip list
SkipList* createSkipList()
{
    SkipList* list = (SkipList*)malloc(sizeof(SkipList));

    //create list S0 with -infinite and +infinite
    QuadNode* minusInf = createQuadNode(MINUS_INF, NULL, 0);
    QuadNode* plusInf = createQuadNode(PLUS_INF, NULL, 0);
    minusInf->next = plusInf;
    plusInf->prev = minusInf;
    list->header = minusInf;
    list->tail = plusInf;
    list->height = 0; //start with height 09 on S0
    list->size = 0;

    return list;
}

//coin toss, determinate new level for the node
int randomLevel()
{
    int level = 0;
    //toss the coin until we have tails (0)
    while (rand() % 2 == 0 && level < MAX_LEVEL - 1)
    {
        level++;
    }
    return level;
}

void find_path(SkipList* list, int key, QuadNode** update)
{
    QuadNode* current = list->header; // Empezar en la cabecera superior

    for (int i = list->height; i >= 0; i--)
    {
        //move horizontal on level i, while next key is less than the one to insert
        while (current->next != NULL && current->next->key < key)
        {
            current = current->next;
        }
        //save the predecessor node on this level
        update[i] = current;
        
        //go down to next level
        if (current->below != NULL)
        {
            current = current->below;
        }
    }
}

QuadNode* search(SkipList* list, int key)
{
    QuadNode* update[MAX_LEVEL + 1];
    find_path(list, key, update);
    QuadNode* target = update[0]->next; //the node is after the predecessor on level 0

    //verify if the node is the correct one
    if (target != NULL && target->key == key)
    {
        return target;
    }
    return NULL; // not found
}

void insert(SkipList* list, int key, void* element)
{
    QuadNode* update[MAX_LEVEL + 1];
    find_path(list, key, update);

    //verify if the key exist and update it
    QuadNode* existing = update[0]->next;

    if (existing != NULL && existing->key == key)
    {
        existing->element = element; //update element
        return;
    }
    //determinate new level for the node
    int i = randomLevel();

    //if new level > actual size
    if (i > list->height)
    {
        //add new void levels
        for (int j = list->height + 1; j <= i; j++)
        {
            QuadNode* newHeader = createQuadNode(MINUS_INF, NULL, j);
            QuadNode* newTail = createQuadNode(PLUS_INF, NULL, j);
            newHeader->next = newTail;
            newTail->prev = newHeader;
            newHeader->below = list->header; //link old header/tail
            newTail->below = list->tail;
            list->header->above = newHeader;
            list->tail->above = newTail;
            list->header = newHeader; //update pointers
            list->tail = newTail;
            update[j] = newHeader; //predecessor is the new header on level
        }
        list->height = i; //update height
    }

    //insert new node on each level, from 0 to i
    QuadNode* prevNode = NULL; //for vertica link

    for (int j = 0; j <= i; j++)
    {
        QuadNode* newNode = createQuadNode(key, element, j);

        //link horizontal
        newNode->next = update[j]->next;
        newNode->prev = update[j];
        update[j]->next->prev = newNode;
        update[j]->next = newNode;

        //vertical link
        if (j > 0)
        {
            newNode->below = prevNode;
            prevNode->above = newNode;
        }
        prevNode = newNode; //save node for link it to above on next level
    }
    list->size++;
}

int delete(SkipList* list, int key)
{
    QuadNode* update[MAX_LEVEL + 1];
    find_path(list, key, update);

    //node to erase is after update[0]
    QuadNode* target = update[0]->next;

    //if key does not exist
    if (target == NULL || target->key != key)
    {
        return 0; //not found
    }
    //erase down to up
    QuadNode* nodeToRemove = target;
    int i = 0;

    while (i <= list->height && update[i]->next == nodeToRemove)
    {
        //unlink horizontal
        update[i]->next = nodeToRemove->next;

        if (nodeToRemove->next != NULL)
        {
            nodeToRemove->next->prev = update[i];
        }

        QuadNode* nodeAbove = nodeToRemove->above; //save upper node

        if (i == 0)
        {
            list->size--;//decrement size one time
        }
        free(nodeToRemove); //free actual node
        nodeToRemove = nodeAbove; //move to upper node
        i++;
    }

    //clear void levels, except S0
    while (list->height > 0 && list->header->next == list->tail)
    {
        QuadNode* oldHeader = list->header;
        QuadNode* oldTail = list->tail;
        list->header = oldHeader->below;
        list->tail = oldTail->below;
        list->header->above = NULL;
        list->tail->above = NULL;
        free(oldHeader);
        free(oldTail);
        list->height--;
    }
    return 1; //erase
}

void print_SkipList(SkipList* list) {
    printf("Skip List (height: %d, size: %d):\n", list->height, list->size);
    QuadNode* levelStart = list->header; //start from upper header

    for (int i = list->height; i >= 0; i--)
    {
        printf("Level %d: ", i);
        QuadNode* current = levelStart;

        while (current != NULL) {
            if (current->key == MINUS_INF)
            {
                printf("-inf");
            }
            else if (current->key == PLUS_INF)
            {
                printf(" -> +inf");
            }
            else
            {
                printf(" -> %d", current->key);
            }
            current = current->next;
        }
        printf("\n");
        levelStart = levelStart->below; //move header to the next level
    }
    printf("\n");
}

void free_SkipList(SkipList* list)
{
    QuadNode* currentLevel = list->header; //start from header
    while (currentLevel != NULL)
    {
        QuadNode* current = currentLevel;
        QuadNode* nextLevel = currentLevel->below; //save reference for below level
        
        while (current != NULL)
        {
            QuadNode* next = current->next; //save reference for next node
            free(current);
            current = next;
        }
        currentLevel = nextLevel; //move one level down
    }
    free(list);
}

void run_test() {
    int testValues[] = {12, 23, 26, 31, 34, 44, 56, 64, 78};
    int testSize = sizeof(testValues) / sizeof(testValues[0]);
    SkipList* list = createSkipList();

    printf("Inserting values: ");

    for (int i = 0; i < testSize; i++)
    {
        printf("%d ", testValues[i]);
        insert(list, testValues[i], NULL);
    }
    printf("\n\n");
    print_SkipList(list);

    printf("Search tests:\n");
    int searchKeys[] = {23, 34, 45, 78};

    for (int i = 0; i < 4; i++)
    {
        QuadNode* result = search(list, searchKeys[i]);

        if (result != NULL)
        {
            printf("Key %d: FOUND\n", searchKeys[i]);
        }
        else
        {
            printf("Key %d: NOT FOUND\n", searchKeys[i]);
        }
    }
    printf("\n");

    //deletion
    printf("Deleting key 34...\n");
    delete(list, 34);
    print_SkipList(list);

    //insertion of values
    printf("Inserting 15 and 45...\n");
    insert(list, 15, NULL);
    insert(list, 45, NULL);
    print_SkipList(list);
    free_SkipList(list);
}

void shuffle(int *array, size_t n) {
    if (n > 1)
    {
        for (size_t i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

void performance_test(FILE* csvFile, int N)
{
    clock_t start, end;
    double total_time_sec, avg_time_sec;

    //generate N random keys
    int* keys = (int*)malloc(N * sizeof(int));
    for (int i = 0; i < N; i++) keys[i] = i;
    shuffle(keys, N);

    //insertion test
    SkipList* list = createSkipList();
    start = clock();

    for (int i = 0; i < N; i++)
    {
        insert(list, keys[i], NULL);
    }
    end = clock();
    total_time_sec = ((double)(end - start)) / CLOCKS_PER_SEC;
    avg_time_sec = total_time_sec / N;
    fprintf(csvFile, "%d,Insert,%.10f,%.10e\n", N, total_time_sec, avg_time_sec);

    //search test
    shuffle(keys, N);
    start = clock();

    for (int i = 0; i < N; i++)
    {
        search(list, keys[i]);
    }
    end = clock();
    total_time_sec = ((double)(end - start)) / CLOCKS_PER_SEC;
    avg_time_sec = total_time_sec / N;
    fprintf(csvFile, "%d,Search,%.10f,%.10e\n", N, total_time_sec, avg_time_sec);

    //deletion test
    shuffle(keys, N);
    start = clock();

    for (int i = 0; i < N; i++)
    {
        delete(list, keys[i]);
    }
    end = clock();
    total_time_sec = ((double)(end - start)) / CLOCKS_PER_SEC;
    avg_time_sec = total_time_sec / N;
    fprintf(csvFile, "%d,Delete,%.10f,%.10e\n", N, total_time_sec, avg_time_sec);

    free(keys);
    free_SkipList(list);
}

int main() {
    srand(time(NULL)); //stat random seed
    //run_test();

    //large dataset
    
    FILE* f = fopen("skip_list_performance.csv", "w");
    if (f == NULL) {
        printf("Error al abrir el archivo!\n");
        return 1;
    }
    
    //CSV header
    fprintf(f, "N,Operation,TotalTime,AverageTime\n");

    int N_values[] = {1000, 5000, 10000, 20000, 50000, 100000, 200000, 500000};
    int num_tests = sizeof(N_values) / sizeof(N_values[0]);

    for (int i = 0; i < num_tests; i++) {
        printf("Processing N = %d...\n", N_values[i]);
        performance_test(f, N_values[i]);
    }

    fclose(f);

    printf("Results saved on 'skip_list_performance.csv'\n");
    

    return 0;
}