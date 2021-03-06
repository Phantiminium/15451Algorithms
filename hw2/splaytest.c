#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct node {
  int data;
  struct node *l;
  struct node *r;
  struct node *p;
};

struct node *root;

int max(int x, int y) {
  if (x > y) return x;
  else return y;
}

int getDepth(struct node *x) {
  int counter = -1;
  struct node *temp = x;
  while (temp) {
    counter = counter + 1;
    temp = temp->p;
  }
  return counter;
}

void left(struct node *x) {
  struct node *y = x->r;
  if(y) {
    x->r = y->l;
    if(y->l) y->l->p = x;
    y->p = x->p;
  }

  if (!x->p) root = y;
  else if (x == x->p->l) x->p->l = y;
  else x->p->r = y;
  if(y) y->l = x;
  x->p = y;

  //x->depth = getDepth(x);
  //y->depth = getDepth(y);
}

void right(struct node *x) {
  struct node *y = x->l;
  //int xDepth = getDepth(x);
  //int yDepth = getDepth(y);
  if(y) {
    x->l = y->r;
    if(y->r) y->r->p = x;
    y->p = x->p;
  }

  if (!x->p) root = y;
  else if (x == x->p->l) x->p->l = y;
  else x->p->r = y;
  if(y) y->r = x;
  x->p = y;

  //x->depth = getDepth(x);
  //y->depth = getDepth(y);
}

int splay(struct node *x)
{
  int counter = 0;
  while(x->p) {
    if(!x->p->p) {
      if(x->p->l == x) {
        right(x->p);
        counter += 1;
      }
      else {
        left(x->p);
        counter += 1;
      }
    }
    else if(x->p->r == x && x->p->p->r == x->p) {
      left(x->p->p);
      left(x->p);
      counter += 2;
    }
    else if(x->p->l == x && x->p->p->l == x->p) {
      right(x->p->p);
      right(x->p);
      counter += 2;
    }
    else if(x->p->l == x && x->p->p->r == x->p) {
      right(x->p);
      left(x->p);
      counter += 2;
    }
    else {
      left(x->p);
      right(x->p);
      counter += 2;
    }
  }
  return counter;
}

struct node *find(int x) {
  struct node *temp = root;
  while (temp->data != x) {
    if (x < temp->data) temp = temp->l;
    else temp = temp->r;
  }
  return temp;
}

int main(int argc, char **argv) {
  int l;
  int n;
  //int A[];
  //struct node *T[];

  scanf("%d", &l);
  scanf("%d", &n);
  int A[n];
  int j;
  for(j=0;j<n;j++) scanf("%d", &A[j]);


  /* Test
  l = 1000000;
  n = 10;
  int A[n];
  A[0] = 0;
  A[1] = 1;
  A[2] = 2;
  A[3] = 3;
  A[4] = 4;
  A[5] = 5;
  A[6] = 6;
  A[7] = 7;
  A[8] = 8;
  A[9] = 9; */


  /* Test *
  A[0] = 2;
  A[1] = 1;
  A[2] = 0;
  A[3] = 4;
  A[4] = 4;
  A[5] = 3; */


  /* Test
  l = 1;
  n = 3;
  int A[n];
  A[0] = 0;
  A[1] = 0;
  A[2] = 0; */

  //int
  //struct node *T[l];
  //for (j = 0; j < n; j++)


  struct node *temp = (struct node*)malloc(sizeof(struct node));
  temp->data = l - 1;
  //temp->depth = l - 1;
  temp->r = NULL;
  temp->p = NULL;
  //temp->l = NULL;
  root = temp;
  //T[l - 1] = temp;

  int i;
  for(i = l - 2; i > -1; i--) {
    struct node *temp2 = (struct node*)malloc(sizeof(struct node));
    temp2->data = i;
    temp2->r = NULL;
    //temp2->l = NULL;
    temp2->p = temp;

    temp->l = temp2;
    temp = temp2;
    //T[i] = temp;
  }

  /* Tree Building Test
  temp = root;
  for(i = 0; i < l - 1; i++) {
    printf("%d\n", temp->data);
    temp = temp->l;
  }
  printf("%d\n", temp->data);

  for(i = 0; i < l; i++) {
    printf("%d\n", temp->data);
    temp = temp->p;
  }
  */


  printf("\n");
  for (i = 0; i < n; i++) {
    struct node *x = find(A[i]);
    //struct node *x = T[A[i]];
    //printf("%d ", root->depth - x->depth);
    //printf("root depth %d, x depth %d \n", root->depth, x->dept);
    //printf("%d ", getDepth(x));
    int depth = splay(x);
    printf("%d ", depth);
    //printf("%d ", root->depth);
  }
  printf("\n");
  return 0;

}
