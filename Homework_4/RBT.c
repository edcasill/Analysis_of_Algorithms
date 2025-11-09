#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


typedef enum {RED, BLACK} color_t;

typedef struct Node
{
    /*
    Properties for the nodes
    */
    int key;
    color_t color;
    struct Node *left, *right, *parent;
}Node;

typedef struct RBT
{
    /*
    Struct for the tree
    */
    Node *root;
    Node *nil;
}RBT;

RBT* createRBT()
{
    /*
    Creates the initial structure of the RBT, NIL and root pointing to each other
    */
    RBT *tree = (RBT*)malloc(sizeof(RBT));
    tree->nil = (Node*)malloc(sizeof(Node));
    tree->nil->color = BLACK;
    tree->nil->left = tree->nil->right = tree->nil->parent = tree->nil;
    tree->root = tree->nil;
    return tree;
}

Node* createNode(RBT *tree, int key)
{
    /*
    Creates a new node into the RBT
    */
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->color = RED;
    newNode->left = tree->nil;
    newNode->right = tree->nil;
    newNode->parent = tree->nil;
    return newNode;
}


void left_rotate(RBT *T, Node *x)
{
    Node *y = x->right;
    x->right = y->left; //turn y's left subtree into x's right subtree
    if (y->left != T->nil) //if y's left subtree is not empty...
    {
        y->left->parent = x; //... then x becomes y's parent
    }
    y->parent = x->parent; //x's parent becomes y's parent
    if (x->parent == T->nil) //if x was the root...
    {
        T->root = y; //then y becomes the root
    }
    else if (x == x->parent->left) //otherwise, if x was a left child...
    {
        x->parent->left = y; //...then y becomes a left child
    }
    else
    {
        x->parent->right = y; // otherwise, x was a rigth child, and now y is
    }
    y->left = x; //make x become y's left child
    x->parent = y;
}


void right_rotate(RBT *T, Node *y)
{
    Node *x = y->left;
    y->left = x->right; //turn y's left subtree into x's right subtree
    if (x->right != T->nil) //if y's rigth subtree is not empty...
    {
        x->right->parent = y; //... then x becomes y's parent
    }
    x->parent = y->parent; //x's parent becomes y's parent
    if (y->parent == T->nil) //if x was the root...
    {
        T->root = x; //then y becomes the root
    }
    else if (y == y->parent->left) //otherwise, if x was a left child...
    {
        y->parent->left = x; //...then y becomes a left child
    }
    else
    {
        y->parent->right = x; // otherwise, x was a rigth child, and now y is
    }
    x->right = y; //make x become y's right child
    y->parent = x;
}


void RB_insert_fixup(RBT *T, Node *z)
{
    while (z->parent->color == RED)
    {
        if (z->parent == z->parent->parent->left) //if z's parent a left child?
        {
            Node *y = z->parent->parent->right; //y is z's uncle
            if (y->color == RED) //are z's parent and unce both red?
            {
                /*case 1*/
                z->parent->color = RED;
                y->color = BLACK;
                z->parent->parent->color =RED;
            }
            else
            {
                if (z == z->parent->right)
                {
                    /* case 2 */
                    z = z->parent;
                    left_rotate(T,z);
                }
                /*case 3*/
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(T, z->parent->parent);
            } 
        }
        else
        {
            /*same  as above, but with "right" and "left" exchanged*/
            Node *y = z->parent->parent->left; //y is z's uncle
            if (y->color == RED) //are z's parent and unce both red?
            {
                /*case 1*/
                z->parent->color = RED;
                y->color = BLACK;
                z->parent->parent->color =RED;
            }
            else
            {
                if (z == z->parent->left)
                {
                    /* case 2 */
                    z = z->parent;
                    right_rotate(T,z);
                }
                /*case 3*/
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(T, z->parent->parent);
            } 
        }
    }
    T->root->color = BLACK;
}


void RB_insert(RBT *T, int key)
{
    Node *z = createNode(T, key);
    Node *x = T->root; //node being compared with z
    Node *y = T->nil; //y will be parent of z

    while (x != T->nil) // descend until reaching the sentinel
    {
        y = x;
        if (z->key < x->key)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }        
    }
    z->parent = y; //found the location - insert z with parent y
    if (y == T->nil)
    {
        T->root = z; // tree T was empty
    }
    else if (z->key < y->key)
    {
        y->left = z;
    }
    else
    {
        y->right = z;
    }
    //z->left = T->nil; //both of z's children are the sentinel
    //z->right = T->nil;
    //z->color = RED; //the new node starts out red
    RB_insert_fixup(T,z);
}


Node* tree_search(RBT *T, Node *x, int key)
{
    if (x == T->nil || key == x->key)
    {
        return x;
    }
    if (key < x->key)
    {
        return tree_search(T, x->left, key);
    }
    else
    {
        return tree_search(T, x->right, key);
    }
}


Node* tree_minimum(RBT *T, Node *x)
{
    while (x->left != T->nil)
    {
        x = x->left;
    }
    return x;
}

Node* tree_maximum(RBT *T, Node *x)
{
    while (x->right != T->nil)
    {
        x = x->right;
    }
    return x;
}


void RB_transplant(RBT *T, Node *u, Node *v)
{
    if (u->parent == T->nil)
    {
        T->root = v;
    }
    else if(u == u->parent->left)
    {
        u->parent->left = v;
    }
    else
    {
        u->parent->right = v;
    }
    v->parent = u->parent;
}


void RB_delete_fixup(RBT *T, Node *x)
{
    while (x != T->root && x->color == BLACK)
    {
        if (x == x->parent->left) //is x a left child
        {
            Node *w = x->parent->right; //w is x's sibling?
            if (w->color == RED)
            {
                /*Case 1*/
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(T, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK)
            {
                /* case 2 */
                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->right->color == BLACK)
                {
                    /* case 3 */
                    w->left->color = BLACK;
                    w->color = RED;
                    right_rotate(T, w);
                    w = x->parent->right;
                }
                /*case 4*/
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                left_rotate(T, x->parent);
                x = T->root;
            }
        }
        else
        {
            /*same as above, but left and right exchanged*/
            Node *w = x->parent->left; //w is x's sibling?
            if (w->color == RED)
            {
                /*Case 1*/
                w->color = BLACK;
                x->parent->color = RED;
                right_rotate(T, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK)
            {
                /* case 2 */
                w->color = RED;
                x = x->parent;
            }
            else
            {
                if (w->left->color == BLACK)
                {
                    /* case 3 */
                    w->right->color = BLACK;
                    w->color = RED;
                    left_rotate(T, w);
                    w = x->parent->left;
                }
                /*case 4*/
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                right_rotate(T, x->parent);
                x = T->root;
            }
        }
    }
    x->color = BLACK;
}


void RB_delete(RBT *T, Node *z)
{
    Node *y = z;
    Node *x;
    color_t y_og_color = y->color;

    if (z->left == T->nil)
    {
        x = z->right;
        RB_transplant(T, z, z->right); //replace z by its right child
    }
    else if(z->right == T->nil)
    {
        x = z->left;
        RB_transplant(T, z, z->left); //replace z by its left child
    }
    else
    {
        y = tree_minimum(T, z->right);
        y_og_color = y->color;
        x = y->right;
        if (y != z->right) //is y father down the tree?
        {
            RB_transplant(T, y, y->right); //replace y by its right child
            y->right = z->right; //z's right child becomes y's right child
            y->right->parent = y;
        }
        else
        {
            x->parent = y; // in case x is T.nil
        }
        RB_transplant(T, z, y); //replace z by its succesor y
        y->left = z->left; //and give z's left child to y, which had no left child
        y->left->parent = y;
        y->color = z->color;
    }
    if (y_og_color == BLACK) //if any red-black violations ocurred
    {
        RB_delete_fixup(T, x); //correct them
    }
    free(z);
}


void RB_delete_by_key(RBT *T, int key) {
    Node *z = tree_search(T, T->root, key);
    if (z != T->nil)
    {
        RB_delete(T, z);
    }
}


void inOrder(RBT *T, Node *x)
{
    if (x != T->nil)
    {
        inOrder(T, x->left);
        printf("%d(%s) ", x->key, (x->color == RED) ? "RED" : "BLACK");
        inOrder(T, x->right);
    }
}


void print_tree(RBT *T, Node *root, int space)
{
    /*print the RBT*/
    if (root == T->nil) return;
    space += 5;

    print_tree(T, root->right, space);
    printf("\n");

    for (int i = 5; i < space; i++)
    {
        printf(" ");
    }
    printf("%d(%c)\n", root->key, (root->color == RED) ? 'R' : 'B');
    print_tree(T, root->left, space);
}

/*
================================================================
NEW 'main' for empirical tests
================================================================
*/

// Function to get the current time in seconds
double get_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

// Function to free all tree memory (post-order traversal)
void free_tree(RBT *T, Node *x)
{
    if (x == T->nil)
    {
        return;
    }
    free_tree(T, x->left);
    free_tree(T, x->right);
    free(x);
}

void destroyRBT(RBT *T)
{
    free_tree(T, T->root); // Free all nodes
    free(T->nil);          // Free NIL
    free(T);               // Free tree struct
}


int main()
{
    /*RBT *T = createRBT();
    
    printf("Values to insert: 10, 20, 30, 15, 25, 5, 35, 40\n");
    int values[] = {10, 20, 30, 15, 25, 5, 35, 40};
    for (int i = 0; i < 8; i++)
    {
        RB_insert(T, values[i]);
    }
    
    printf("\nOredered tour: ");
    inOrder(T, T->root);
    
    printf("\n\nTree structure:\n");
    print_tree(T, T->root, 0);
    
    // find element on tree
    int searchKey = 15;
    Node *found = tree_search(T, T->root, searchKey);
    if (found != T->nil) {
        printf("\nElement %d found on tree.\n", searchKey);
    } else {
        printf("\nElement %d NOT found on tree.\n", searchKey);
    }

    Node *minNode = tree_minimum(T, T->root);
    Node *maxNode = tree_maximum(T, T->root);
    printf("Minimum: %d, Maximum: %d\n", minNode->key, maxNode->key);

    printf("Deleting 10 from RBT\n");
    RB_delete_by_key(T, 10);
    inOrder(T, T->root);
    print_tree(T, T->root, 0);
    */

    //Mod for test the complexity
    //Initialize the random number generator
    srand(time(NULL));

    //n sizes to test
    int N_values[] = {10000, 50000, 100000, 500000, 1000000};
    int num_sizes = sizeof(N_values) / sizeof(N_values[0]);

    // Number of searches/eliminations to be performed per test
    // We keep it constant to isolate the effect of 'n'
    const int NUM_OPS = 10000; 

    printf("Starting empirical tests for RBT...\n");
    printf("================================================================\n");
    printf("%-10s | %-20s | %-25s\n", "N", "Insertion time (s)", "Search time (s)");
    printf("----------------------------------------------------------------\n");

    for (int i = 0; i < num_sizes; i++)
    {
        int N = N_values[i];
        RBT *T = createRBT();

        // --- Insertion test ---
        // Measures the time to insert N random elements
        // Large random key range (0 to N*10) to reduce collisions
        
        double start_time = get_time();
        for (int j = 0; j < N; j++)
        {
            RB_insert(T, rand() % (N * 10));
        }
        double end_time = get_time();
        double insert_time = end_time - start_time;

        // --- Search test ---
        // Measures the time for NUM_OPS random searches
        start_time = get_time();
        for (int j = 0; j < NUM_OPS; j++)
        {
            tree_search(T, T->root, rand() % (N * 10));
        }
        end_time = get_time();
        double search_time = end_time - start_time;
        
        // Print results for this N
        printf("%-10d | %-20.6f | %-25.6f\n", N, insert_time, search_time);

        // --- DELETE TEST (Optional) ---
        // Measuring this is similar to searching.
        // We focus on insertion and searching, which are the most straightforward.

        // Free the tree's memory for the next iteration
        destroyRBT(T);
    }
    
    printf("================================================================\n");
    printf("Tests end\n");

    return 0;
}
