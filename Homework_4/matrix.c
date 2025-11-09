#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <limits.h>


void matrix_chain_order(int p[], int n, int **m, int **s)
{
    int i, l, j, k, q;

    for (i = 1; i <= n; i++) //chain lenght 1
    {
        m[i][i] = 0;
    }
    for (l = 2; l <= n; l++) // l is the chain lenght
    {
        for (i = 1; i <= n-l+1; i++) //chain begins at Ai
        {
            j = i + l - 1; //chain ends at Aj
            m[i][j] = INT_MAX; //infinite representation
            for (k = i; k <= j-1; k++) //try Ai:kAk+1
            {
                q = m[i][k] + m[k+1][j] + p[i-1]*p[k]*p[j];
                if (q < m[i][j])
                {
                    m[i][j] = q; //remember this cost
                    s[i][j] = k; // remember this index
                }
            }
        }
    }
}


void print_optimal_parens(int **s, int i, int j)
{
    if (i == j)
    {
        printf("A%d", i);
    }
    else
    {
        printf("(");
        print_optimal_parens(s,i,s[i][j]);
        print_optimal_parens(s,s[i][j] + 1, j);
        printf(")");
    }
}


int main()
{
    /*
    matrix dimentions:
    A1 = 10x55
    A2 = 55x25
    A3 = 25x5
    A4 = 4x40
    A5 = 40x20
    A6 = 20x35
    */
    int p[] ={ 10, 55, 25, 5, 40, 20, 35};
    int n = sizeof(p) / sizeof(p[0]) - 1;
    int **m = (int **)malloc((n + 1) * sizeof(int *));
    int **s = (int **)malloc((n + 1) * sizeof(int *));

    for (int i = 0; i <= n; i++)
    {
        m[i] = (int *)malloc((n + 1) * sizeof(int));
        s[i] = (int *)malloc((n + 1) * sizeof(int));
    }
    // Init matrix
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            m[i][j] = 0;
            s[i][j] = 0;
        }
    }

    matrix_chain_order(p,n,m,s);
    printf("Minumum cost for multiplication: %d\n", m[1][n]);
    printf("Optimal order: ");
    print_optimal_parens(s, 1, n);
    printf("\n");

    printf("m table (minimum costs):\n");
    for (int i = 1; i <= n; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            if (i <= j)
            {
                printf("%6d ", m[i][j]);
            }
            else
            {
                printf("       ");
            }
        }
        printf("\n");
    }

    printf("\ns table(divition points):\n");
    for (int i = 1; i < n; i++) {
        for (int j = 2; j <= n; j++) {
            if (i < j) {
                printf("%d ", s[i][j]);
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }

    //free memory
    for (int i = 0; i <= n; i++) {
        free(m[i]);
        free(s[i]);
    }
    free(m);
    free(s);
    
    return 0;
}