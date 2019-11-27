#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * For the cops and robbers problem, I converted the problem into a linear
 * program via the Floyd-Warshall algorithm, and then used the simplex
 * algorithm to solve the linear program.
 *
 * In order to create the payoff matrix for the problem, I used an All-Pairs
 * Shortest Paths algorithm, the Floyd-Warshall algorithm, to find the
 * value of each vertex choice for the robber and the cop. Then, to solve
 * the resultant payoff matrix, I used the system of coefficients as given
 * by the matrix, and created a constraint for each, in which a universal
 * variable, which we can call 'a', is less than or equal to all linear
 * expressions of the payoff matrix, e.g.
 *
 * 1 2 3       a <= x_1 + 2x_2 + 3x_3
 * 4 5 6  ==>  a <= 4x_1 + 5x_2 + 6x_3
 * 7 2 4       a <= 7x_1 + 2x_2 + 4x_3
 *
 * ...and then 2 additional constraints, which detail that the sum of 'x'
 * must be equal to 1, e.g.:
 *
 * 1 <= x_1 + x_2 + x_3
 * 1 >= x_1 + x_2 + x_3
 *
 * The objective function is simply 'a', which we seek to maximize.
 * We then convert all the constraints to a "less than" from for simplex
 * algorithm, to which we feed the A, b, and c matrices as detailed above.
 *
 * The runtime of Floyd-Warshall is O(n^3) in the worst case, in which
 * n = |V|. The runtime of simplex can be exponential, but in most cases,
 * is fairly efficient.
 */

int V;
int E;

int allocs_in_use;
void my_free(void * p) {
    allocs_in_use--;
    free(p);
}
void * my_alloc(int size) {
    void * p = malloc(size);
    allocs_in_use++;
    if (p == NULL) {
        fprintf(stderr, "Ran out of space.  Requested size=%d.\n", size);
        exit(1);
    }
    return p;
}

/* used internally by the LP solver  */
int m, n;         /* number of constraints and variables */
int* non_basic;   /* indices of non-basic variables.  Length=n */
int* basic;       /* indices of basic variables.  Length=m */
double** a;       /* (m+1) x (n+1) tableau for simplex */

#define INF 1e100
#define EPS 1e-9

void pivot(int r, int c) {
    int i,j,t;
    t = non_basic[c];
    non_basic[c] = basic[r];
    basic[r] = t;

    a[r][c]=1/a[r][c];
    for(j=0;j<=n;j++) if(j!=c) a[r][j]*=a[r][c];
    for(i=0;i<=m;i++) if(i!=r) {
            for(j=0;j<=n;j++) if(j!=c) a[i][j]-=a[i][c]*a[r][j];
            a[i][c] = -a[i][c]*a[r][c];
        }
}

int feasible() {
    int r,c,i; double p,v;
    while(1) {
        for(p=INF,i=0; i<m; i++) if(a[i][n]<p) p=a[r=i][n];
        if(p>-EPS) return 1;
        for(p=0,i=0; i<n; i++) if(a[r][i]<p) p=a[r][c=i];
        if(p>-EPS) return 0;
        p = a[r][n]/a[r][c];
        for(i=r+1; i<m; i++) if(a[i][c]>EPS) {
                v = a[i][n]/a[i][c];
                if(v<p) r=i,p=v;
            }
        pivot(r,c);
    }
}

int simplex(int m0, int n0, double* A[], double B[], double C[], double* z, double soln[]) {
    /*
    input:
      m = #constraints, n =#variables
      max C dot x s.t. A x <= B  x >= 0
      where A = mxn, B = m vector, C = n vector
    output:
      returns 1 (feasible), 0 (infeasible), or -1 (unbounded)
      If feasible, then stores objective value in z, and the solution in soln,
      an array of length n supplied for the variables.
    caveats:
      Cycling is possible.  Nothing is done to mitigate loss of
      precision when the number of iterations is large.
    */
    int r,c,i,j;
    double p, v;

    m = m0;
    n = n0;

    non_basic = (int *) my_alloc(n * sizeof(int));
    basic = (int *) my_alloc(m * sizeof(int));
    for(i=0; i<n; i++) non_basic[i]=i;
    for(i=0; i<m; i++) basic[i]=n+i;

    a = (double **) my_alloc((m+1) * sizeof (double *));

    for (i=0; i<=m; i++) {
        a[i] = (double *) my_alloc((n+1) * sizeof (double));
    }

    for(i=0; i<m; i++) {
        for(j=0; j<n; j++) {
            a[i][j] = A[i][j];
        };
        a[i][n] = B[i];
    }
    for(j=0; j<n; j++) {
        a[m][j] = C[j];
    }

    a[m][n] = 0.0;

    if(!feasible()) return 0;

    while(1) {
        for(p=0,i=0; i<n; i++) if(a[m][i]>p) p=a[m][c=i];
        if(p<EPS) {
            for(i=0; i<n; i++) if(non_basic[i]<n) soln[non_basic[i]]=0.0;
            for(i=0; i<m; i++) if(basic[i]<n) soln[basic[i]]=a[i][n];
            *z = -a[m][n];
            return 1;
        }
        for(p=INF,i=0; i<m; i++) if(a[i][c]>EPS) {
                v = a[i][n]/a[i][c];
                if(v<p) p=v,r=i;
            }
        if(p==INF) return -1;
        pivot(r,c);
    }
}

void printm(double **M) {
  int i;
  int j;
  for (i = 0; i < V + 2; i++) {
    for (j = 0; j < V + 1; j++)
      printf("%lf ", M[i][j]);
    printf("\n");
  }
}

void print(double *A, int len) {
  int i;
  printf("\n");
  for (i = 0; i < len; i++)
    printf("%lf ", A[i]);
  printf("\n\n");
}

void FW(double *dist) {
  int k;
  int i;
  int j;

  for (k = 0; k < V; k++) {
    for (i = 0; i < V; i++) {
      for (j = 0; j < V; j++) {
        if (dist[i * V + k] + dist[k * V + j] < dist[i * V + j]) {
          dist[i * V + j] = dist[i * V + k] + dist[k * V + j];
        }
      }
    }
  }
}

int main(int argc, char **argv) {
  int k;
  int i;
  int j;

  /* Read in input */
  scanf("%d %d", &V, &E);

  double *dist = (double*)malloc(sizeof(double) * V * V);
  for (i = 0; i < V; i++) {
    for (j = 0; j < V; j++) {
      if (i == j) dist[i * V + j] = 0.0;
      else dist[i * V + j] = -1.0;
    }
  }
  for (k = 0; k < E; k++) {
    int r;
    int c;
    double v;
    scanf("%d %d %lf", &r, &c, &v);

    dist[r * V + c] = (double)v;
    dist[c * V + r] = (double)v;
  }

  for (i = 0; i < V; i++) {
    for (j = 0; j < V; j++) {
      if (dist[i * V + j] < 0.0) {
        dist[i * V + j] = DBL_MAX;
      }
    }
  }

  FW(dist);

  /* Form linear program */
  double **A = (double**)malloc(sizeof(double*) * (V + 2));
  for (k = 0; k < V + 2; k++)
    A[k] = (double*)malloc(sizeof(double) * (V + 1));

  for (i = 0; i < V; i++) {
    for (j = 0; j < V + 1; j++) {
      if (j == 0) A[i][j] = 1.0;
      else A[i][j] = -1.0 * dist[i * V + j - 1];
    }
  }

  for (j = 0; j < V + 1; j++) {
    if (j == 0) A[V][j] = 0.0;
    else A[V][j] = 1.0;
  }

  for (j = 0; j < V + 1; j++) {
    if (j == 0) A[V + 1][j] = 0.0;
    else A[V + 1][j] = -1.0;
  }

  double *b = (double*)malloc(sizeof(double) * (V + 2));
  memset(b, 0.0, sizeof(double) * V);
  b[V] = 1.0;
  b[V + 1] = -1.0;

  double *c = (double*)malloc(sizeof(double) * (V + 1));
  memset(c, 0.0, sizeof(double) * (V + 1));
  c[0] = 1.0;

  double z;
  double *soln = (double*)malloc(sizeof(double) * (V + 1));

  if (simplex(V + 2, V + 1, A, b, c, &z, soln) > -1) {
    //round z to 6 digits
    printf("%.6lf\n", z);
  }
  else printf("Something failed\n");

  return 0;
}
