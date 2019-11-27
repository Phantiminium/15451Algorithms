#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

/*
 * For this problem, I transformed the input into a max-flow network graph and
 * ran the transformation on the Ford-Fulkerson algorithm to find the max
 * flow of the resulting network. The one-to-one correspondence is, the max
 * flow of the tranformed input is the number of beers that can be created via
 * the original input.
 *
 * The runtime of this problem is dominated by the Ford-Fulkerson algorithm,
 * which is O(F(n + m)). In this case, F is equal to the total number of beers
 * that can be created in the original graph.
 */

int r;
int c;
int x;
int num_b;
int num_r;
int num_e;

struct node {
  char val;
  int rank;
  int index;
  int edge;
  struct node* next;
};

struct Graph {
  int numVertices;
  struct node** aL;
};


/*
 * The following code creates a queue that stores ints.
 */
struct qnode {
  int data;
  struct qnode* prev;
};

struct Queue {
  struct qnode *head;
  struct qnode *tail;
  int size;
  int limit;
};

struct Queue *init_q(void) {
  struct Queue *queue = (struct Queue *)malloc(sizeof(struct Queue));
  queue->limit = 0;
  queue->size = 0;
  queue->head = NULL;
  queue->tail = NULL;

  return queue;
}

void enqueue(struct Queue *Q, int vertex) {
  struct qnode *temp = (struct qnode*)malloc(sizeof(struct qnode));
  temp->data = vertex;
  temp->prev = NULL;

  if (Q->size == 0) {
    Q->head = temp;
    Q->tail = temp;
  }
  else {
    Q->tail->prev = temp;
    Q->tail = temp;
  }
  Q->size++;
}

int dequeue(struct Queue *Q) {
  if (Q->size == 0) {
    printf("Queue empty\n");
    return -1;
  }
  else {
    struct qnode *temp = Q->head;
    Q->head = (Q->head)->prev;
    Q->size--;

    int temp2 = temp->data;
    free(temp);
    return temp2;
  }
}

void freequeue(struct Queue *Q) {
  while (Q->size > 0) {
    dequeue(Q);
  }
  free(Q);
}

/*
 * insert inserts the given node into the specified start index.
 */
void insert(struct Graph* G, int index, struct node* temp) {
  struct node *root = G->aL[index];
  if (!root) {
    G->aL[index] = temp;
  }
  else {
    while (root->next) {
      root = root->next;
    }
    root->next = temp;
  }
}

/*
 * bfs runs breadth-first search on the graph G until the sink is reached,
 * storing the parent of each vertex in the parent array, using the cap arrray
 * to check if an edge is active, and using the visited array and vcond to
 * check whether a node has been visited.
 */
bool bfs(struct Graph *G, int parent[], int cap[], int visited[],
         int vcond) {

  struct Queue *q = init_q();
  enqueue(q, 0);
  visited[0] = true;
  parent[0] = -1;

  while (q->size > 0) {
    int u = dequeue(q);

    struct node *root;

    for (root = G->aL[u]; root != NULL; root = root->next) {
      int vi = root->index;
      int e = root->edge;

      if ((visited[vi] != vcond) && (cap[e] == 1)) {
        enqueue(q, vi);
        parent[vi] = u;
        visited[vi] = vcond;
        if (vi == 1) {
          freequeue(q);
          return true;
        }
      }
    }
  }

  freequeue(q);
  return false;
}

/*
 * edge returns the edge which goes from the start to the goal by going through
 * the adjacency matrix.
 */
int edge(struct Graph *G, int start, int goal) {

  struct node *root;
  for (root = G->aL[start]; root != NULL; root = root->next) {
    if (root->index == goal) {
      return root->edge;
    }
  }
  return -1;

}

/*
 * create makes the edge in G specified by the start and the end nodes given,
 * denoting the edge edge_n as given.
 */
void create(struct Graph *G, char start, int s_rank,
            char type, int rank_n, int edge_n) {

  struct node *temp = (struct node*)malloc(sizeof(struct node));
  temp->val = type;
  temp->rank = rank_n;
  temp->edge = edge_n;
  temp->next = NULL;

  if (type == 's') temp->index = 0;
  else if (type == 't') temp->index = 1;
  else if (type == 'b') temp->index = 2 + rank_n;
  else if (type == 'r') temp->index = 2 + num_b + rank_n;
  else if (type == 'u') temp->index = 2 + num_b + num_r + rank_n;
  else temp->index = 2 + num_b + num_r + num_e + rank_n;

  if (start == 's') insert(G, 0, temp);
  else if (start == 't') insert(G, 1, temp);
  else if (start == 'b') insert(G, 2 + s_rank, temp);
  else if (start == 'r') insert(G, 2 + s_rank + num_b, temp);
  else if (start == 'u') insert(G, 2 + num_b + num_r + s_rank, temp);
  //start == 'v'
  else insert(G, 2 + num_b + num_r + num_e + s_rank, temp);

}

/*
 * print prints the adjacency list in G.
 */
void print(struct Graph *G) {
  int k;
  for (k = 0; k < G->numVertices; k++) {
    if (k == 0) printf("s: ");
    else if (k == 1) printf("t: ");
    else if (k < 2 + num_b) printf("b%d: ", k - 2);
    else if (k < 2 + num_b + num_r) printf("r%d: ", k - num_b - 2);
    else if (k < 2 + num_b + num_r + num_e)
      printf("eu%d: ", k - num_b - num_r - 2);
    else
      printf("ev%d: ", k - num_b - num_r - num_e - 2);

    struct node *root = G->aL[k];
    while (root) {
      printf("(%c %d %d) ", root->val, root->rank, root->edge);
      root = root->next;
    }
    printf("\n");
  }
}

int main(int argc, char ** argv) {
  int k;
  x = pow(10, 5);
  char *s  = (char*)malloc(sizeof(char) * x);

  scanf("%d %d", &r, &c);

  char *s_row;
  for (k = 0; k < r; k++) {
    s_row = (char*)malloc(sizeof(char) * c);
    scanf("%s", s_row);
    strcat(s, s_row);
    free(s_row);
  }

  /* Adjacency Graph Creation */

  /* ID the vertices */
  int *ranks = malloc(sizeof(int) * r * c);
  num_b = 0;
  num_r = 0;
  num_e = 0;
  for (k = 0; k < r * c; k++) {
    if (s[k] == 'b') {
      ranks[k] = num_b;
      num_b++;
    }
    else if (s[k] == 'r') {
      ranks[k] = num_r;
      num_r++;
    }
    else if (s[k] == 'e') {
      ranks[k] = num_e;
      num_e++;
    }
    //no else, we don't keep track of '.'
  }

  /* Create the adjacency list */
  struct Graph *G = (struct Graph*)malloc(sizeof(struct Graph));
  int n = num_b + num_r + (num_e * 2) + 2;
  G->numVertices = n;
  G->aL = malloc(G->numVertices * sizeof(struct node));

  int edge_n = 0;

  // adjacency list for source and sink
  for (k = 0; k < num_b; k++) {
    create(G, 's', 0, 'b', k, edge_n);
    edge_n++;

    create(G, 'b', k, 's', 0, edge_n);
    edge_n++;
  }

  for (k = 0; k < num_r; k++) {
    create(G, 'r', k, 't', 0, edge_n);
    edge_n++;

    create(G, 't', 0, 'r', k, edge_n);
    edge_n++;
  }

  // adjacency list for e's vertex-edges
  for (k = 0; k < num_e; k++) {

    //u->v forward and backward
    create(G, 'u', k, 'v', k, edge_n);
    edge_n++;

    create(G, 'v', k, 'u', k, edge_n);
    edge_n++;
  }

  // adjacency list for everything else
  for (k = 0; k < r * c; k++) {
    int i = k / c;
    int j = k % c;
    int num;

    if (s[k] == 'b') {
      int rank_b = ranks[k];
      if (j > 0 && s[k - 1] == 'e') {
          num = ranks[k - 1];

          //e<-b edge, forward and backward
          create(G, 'b', rank_b, 'u', num, edge_n);
          edge_n++;

          create(G, 'u', num, 'b', rank_b, edge_n);
          edge_n++;
      }
      if (j < c - 1 && s[k + 1] == 'e') {
          num = ranks[k + 1];

          //b->e edge, forward and backward
          create(G, 'b', rank_b, 'u', num, edge_n);
          edge_n++;

          create(G, 'u', num, 'b', rank_b, edge_n);
          edge_n++;
      }
      if (i > 0 && s[k - c] == 'e') {
          num = ranks[k - c];

          //b^e edge, forward and backward
          create(G, 'b', rank_b, 'u', num, edge_n);
          edge_n++;

          create(G, 'u', num, 'b', rank_b, edge_n);
          edge_n++;
      }
      if (i < r - 1 && s[k + c] == 'e') {
        num = ranks[k + c];

        //b_e edge, forward and backward
        create(G, 'b', rank_b, 'u', num, edge_n);
        edge_n++;

        create(G, 'u', num, 'b', rank_b, edge_n);
        edge_n++;
      }
    }

    else if (s[k] == 'r') {
      int rank_r = ranks[k];
      if (j > 0 && s[k - 1] == 'e') {
        num = ranks[k - 1];

        //e->r edge, forward and backward
        create(G, 'v', num, 'r', rank_r, edge_n);
        edge_n++;

        create(G, 'r', rank_r, 'v', num, edge_n);
        edge_n++;
      }
      if (j < c - 1 && s[k + 1] == 'e') {
        num = ranks[k + 1];

        //r<-e edge, forward and backward
        create(G, 'v', num, 'r', rank_r, edge_n);
        edge_n++;

        create(G, 'r', rank_r, 'v', num, edge_n);
        edge_n++;
      }
      if (i > 0 && s[k - c] == 'e') {
        num = ranks[k - c];

        //r^e edge, forward and backward
        create(G, 'v', num, 'r', rank_r, edge_n);
        edge_n++;

        create(G, 'r', rank_r, 'v', num, edge_n);
        edge_n++;
      }
      if (i < r - 1 && s[k + c] == 'e') {
        num = ranks[k + c];

        //r_e edge, forward and backward
        create(G, 'v', num, 'r', rank_r, edge_n);
        edge_n++;

        create(G, 'r', rank_r, 'v', num, edge_n);
        edge_n++;
      }
    }

    else if (s[k] == 'e') {
      int rank_e = ranks[k];
      if (j > 0 && s[k - 1] == 'e') {
        num = ranks[k - 1];

        //u<-v edge, forward and backward
        create(G, 'v', rank_e, 'u', num, edge_n);
        edge_n++;

        create(G, 'u', num, 'v', rank_e, edge_n);
        edge_n++;
      }
      if (j < c - 1 && s[k + 1] == 'e') {
        num = ranks[k + 1];

        //v->u edge, forward and backward
        create(G, 'v', rank_e, 'u', num, edge_n);
        edge_n++;

        create(G, 'u', num, 'v', rank_e, edge_n);
        edge_n++;
      }
      if (i > 0 && s[k - c] == 'e') {
        num = ranks[k - c];

        //v^u edge, forward and backward
        create(G, 'v', rank_e, 'u', num, edge_n);
        edge_n++;

        create(G, 'u', num, 'v', rank_e, edge_n);
        edge_n++;
      }
      if (i < r - 1 && s[k + c] == 'e') {
        num = ranks[k + c];

        //v_u edge, forward and backward
        create(G, 'v', rank_e, 'u', num, edge_n);
        edge_n++;

        create(G, 'u', num, 'v', rank_e, edge_n);
        edge_n++;
      }
    }
  }

  /* Find the max flow of the graph */
  if (num_b == 0 || num_r == 0 || num_e == 0)
    printf("0 beers\n");
  else {
    // Ford-Fulkerson
    int *parent = (int*)malloc(sizeof(int) * n);
    int *visited = (int*)malloc(sizeof(int) * n);
    int *cap = (int*)malloc(sizeof(int) * edge_n);
    int iter = 1;

    for (k = 0; k < edge_n; k++) cap[k] = 1 + (k % 2);

    int max_flow = 0;
    memset(visited, 0, n);
    while (bfs(G, parent, cap, visited, iter)) {

      int v;
      for (v = 1; v != 0; v = parent[v]) {
        int u = parent[v];
        int e = edge(G, u, v);
        cap[e] = 1 - cap[e];
        cap[e^1] = 1 - cap[e];
      }

      max_flow++;
      iter++;
    }

    if (max_flow == 1) printf("1 beer\n");
    else printf("%d beers\n", max_flow);
  }

  return 0;
}
