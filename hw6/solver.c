#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct op {
  char type;
  int i;
  int j;
  int d;
};

int n, *A;

void IncRange(int i, int j, int d) {
  int k;
  for (k = i; k <= j; k++) {
    A[k] += d;
  }
}

int RangeMin(int i, int j) {
  int k;
  int min = INT_MAX;
  for (k = i; k <= j; k++) {
    if (A[k] < min)
      min = A[k];
  }
  return min;
}

void print() {
  int i;
  //printf("\n");
  for (i = 0; i < n; i++) {
    printf("%d ", A[i]);
  }
  printf("\n");
}


int main(int argc, char **argv) {
  int m, i;

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
    }
    else {

      struct op *temp = (struct op*)malloc(sizeof(struct op));
      temp->type = 'r';
      scanf("%d %d", &s, &e);
      temp->i = s;
      temp->j = e;

      ops[i] = temp;
    }
  }

  A = (int*)malloc(sizeof(int) * n);
  for (i = 0; i < n; i++) A[i] = 0;

  for (i = 0; i < m; i++) {
    //print();
    if (ops[i]->type == 'i')
      IncRange(ops[i]->i, ops[i]->j, ops[i]->d);
    else
      printf("%d\n", RangeMin(ops[i]->i, ops[i]->j));
  }

  return 0;
}
