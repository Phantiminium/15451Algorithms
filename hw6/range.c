#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
 * For the query problem, I used a combination of segtrees and lazy programming
 * in order to achieve an O(log n) time bound on both the IncRange and RangeMin
 * operations. The segtree was also augmented to include two variables to keep
 * track of pending updates, and current minimums.
 *
 * Specifically, for the IncRange operation, the idea is to find the terminal
 * blocks, update both its minimum value and pending updates, and then
 * send the new minimum to its ancestors to revise their minimums. For the
 * RangeMin operation, the idea is to again find the terminal blocks and
 * recursively send their minimums upward in the tree. For both operations,
 * we must be mindful of propogating pending updates downard whenever we
 * encounter them.
 *
 * Since we find the terminal blocks, like in the original segtree, and there
 * are O(log n) levels, we preform a constant factor of O(log n) operations for
 * each query. Thus, both operations have runtime O(log n).
 */

typedef int bool;
#define true 1
#define false 0

struct op {
  char type;
  int i;
  int j;
  int d;
};

struct node {
  int updates;
  int rangeMin;
  bool valid;
};

int SuperCeiling(int n) {
  int p;
  //int count = 0;
  for (p = 1; p < n; p = 2 * p);
  return p;
}

int max (int a, int b) {return (a > b) ? a : b;}
int min (int a, int b) {return (a < b) ? a : b;}
int Parent(int i) {return i / 2;}
int LeftChild(int i) {return 2 * i;}
int RightChild(int i) {return 2 * i + 1;}

int glue(int a, int b) {
  return min(a, b);
}

int identity = INT_MAX;
int n, N;
struct node **A;

void printTree() {
  int i;
  int power = 1;
  printf("\n");
  for (i = 0; i < 2 * N; i++) {
    if (i == power) {
      printf("\n");
      power *= 2;
    }
    printf("(%d: %d, %d) ", i, A[i]->rangeMin, A[i]->updates);
    /*
    if (A[i]->valid)
      printf("V) ");
    else
      printf("I) " );
    */
  }
  printf("\n\n");
}

void printop(struct op **ops, int i) {
  printf("%c %d %d ", ops[i]->type, ops[i]->i, ops[i]->j);
  if (ops[i]->type == 'i') {
    printf("%d", ops[i]->d);
  }
  printf("\n");
}

void PropDown(int v) {
  int left = LeftChild(v);
  int right = RightChild(v);

  A[left]->rangeMin += A[v]->updates;
  A[left]->updates += A[v]->updates;
  A[right]->rangeMin += A[v]->updates;
  A[right]->updates += A[v]->updates;

  A[v]->updates = 0;
}

void f1(int v, int l, int r, int i, int j, int d) {
  int m;

  /* When we find a terminal block, we update the minimum on the right
   * and the minimum on the left. Then, we propogate the minium of the two
   * upwards.
   */
  if (l == i && r == j) {
    A[v]->updates += d;
    A[v]->rangeMin += d;

    /* Propogate minimum value among ancestors. */

    int c = v;
    int p = Parent(c);

    for (p = Parent(v); p > 0; p = Parent(p))
      A[p]->rangeMin =
        glue (A[LeftChild(p)]->rangeMin, A[RightChild(p)]->rangeMin);

  }

  /* Otherwise, we continue searching for the terminal value.
   */
  else {
    m = (l + r) / 2;

    PropDown(v);

    if (i <= m)
      /* If we continue searching on the left child, we update the values for
       * the left child using the left minimum of the parent. */
      f1 (LeftChild(v), l, m, i, (min(m, j)), d);


    if (j > m)
      /* Likewise, if we continue searching on the right child, we update the
       * values for the right child using the right minimum of the parent. */
      f1 (RightChild(v), (m + 1), r, (max(i, (m + 1))), j, d);

  }
}

void IncRange(int i, int j, int d) {
  f1 (1, 0, (N - 1), i, j, d);
}

int f2 (int v, int l, int r, int i, int j) {
  int t1, t2, m;
  /* When we get to a terminal block, we know its values have been updated, so
   * we simply return the minimum of the minimum left and right.
   */
  if (l == i && r == j) {
    return A[v]->rangeMin;
  }
  /* Otherwise, we continue the search into the children, updating the children
   *  if their values are not consistent with the parents'.
   */
  else {
    m = (l + r) / 2;

    PropDown(v);

    if (i <= m)
      /* If we continue searching on the left child, we update the values for
       * the left child using the left minimum of the parent. */
      t1 = f2 (LeftChild(v), l, m, i, (min(m, j)));
    else
      t1 = identity;

    if (j > m)
      /* Likewise, if we continue searching on the right child, we update the
       * values for the right child using the right minimum of the parent. */
      t2 = f2 (RightChild(v), (m + 1), r, (max(i, (m + 1))), j);
    else
      t2 = identity;

    return glue(t1, t2);
  }
}

int RangeMin(int i, int j) {
  return f2 (1, 0, (N - 1), i, j);
}

int main(int argc, char **argv) {
  int i, m;

  int IR = 0;
  int RM = 0;

  scanf("%d %d", &n, &m);

  struct op **ops = (struct op**)malloc(sizeof(struct op*) * m);
  char t[5];
  int s, e, d;
  for (i = 0; i < m; i++) {
    scanf("%s", t);
    if (t[0] == 'I') {

      struct op *temp = (struct op*)malloc(sizeof(struct op));
      temp->type = 'i';
      scanf("%d %d %d", &s, &e, &d);
      temp->i = s;
      temp->j = e;
      temp->d = d;

      ops[i] = temp;
      IR++;
    }
    else {

      struct op *temp = (struct op*)malloc(sizeof(struct op));
      temp->type = 'r';
      scanf("%d %d", &s, &e);
      temp->i = s;
      temp->j = e;

      ops[i] = temp;
      RM++;
    }
  }

  N = SuperCeiling(n);
  A = (struct node**)malloc(sizeof(struct node*) * 2 * N);

  for (i = 0; i < 2 * N; i++) {
    A[i] = (struct node*)malloc(sizeof(struct node));
    A[i]->rangeMin = 0;
    A[i]->updates = 0;
  }

  /* Do operations */
  for (i = 0; i < m; i++) {
    if (ops[i]->type == 'i') {
      IncRange(ops[i]->i, ops[i]->j, ops[i]->d);
    }
    else {
      int ans = RangeMin(ops[i]->i, ops[i]->j);
      printf("%d\n", ans);
    }
  }



  return 0;
}
