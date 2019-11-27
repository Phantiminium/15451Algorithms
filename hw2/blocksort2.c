#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/*
 *
 */

struct node {
  int data;
  struct node *l;
  struct node *r;
  struct node *p;
};

struct node *root;
int n;

void size_check(struct node *T[]) {
  int i;
  printf("\n");
  for (i = 0; i < n; i++) {
    printf("%d\n", T[i]->data);
  }
  printf("\n");
}

void left(struct node *x) {
  struct node *y = x->r;

  int s_a = 0;
  if (x->l) s_a = x->l->data;

  int s_b = 0;
  int s_c = 0;
  if (y) {
    if (y->l) s_b = y->l->data;
    if (y->r) s_c = y->r->data;
  }

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

  x->data = s_a + s_b + 1;
  if (y) y->data = x->data + s_c + 1;

  //x->depth = getDepth(x);
  //y->depth = getDepth(y);
}

void right(struct node *x) {
  struct node *y = x->l;

  int s_c = 0;
  if (x->r) s_c = x->r->data;

  int s_a = 0;
  int s_b = 0;
  if (y) {
    if (y->l) s_a = y->l->data;
    if (y->r) s_b = y->r->data;
  }

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

  x->data = s_b + s_c + 1;
  if (y) y->data = x->data + s_a + 1;

  //x->depth = getDepth(x);
  //y->depth = getDepth(y);
}

void splay(struct node *x)
{
  while(x->p) {
    if(!x->p->p) {
      if(x->p->l == x) {
        right(x->p);
      }
      else {
        left(x->p);
        //counter += 1;
      }
    }
    else if(x->p->r == x && x->p->p->r == x->p) {
      left(x->p->p);
      left(x->p);
      //counter += 2;
    }
    else if(x->p->l == x && x->p->p->l == x->p) {
      right(x->p->p);
      right(x->p);
      //counter += 2;
    }
    else if(x->p->l == x && x->p->p->r == x->p) {
      right(x->p);
      left(x->p);
      //counter += 2;
    }
    else {
      left(x->p);
      right(x->p);
      //counter += 2;
    }
  }
  //return counter;
}

//traverse finds the node in a tree such that, when that node is splayed,
//the left subtree of the resulting tree will have size len.
struct node *traverse(int len, struct node *temp) {
  if (!temp) return temp;

  if (len == 0) return temp;
  else {
    if (temp->l) {
      if (temp->l->data > len) return traverse(len, temp->l);
      else if (temp->l->data < len)
        return traverse(len - temp->l->data - 1, temp->r);
      else return traverse(len - temp->l->data, temp);
    }
    else {
      if (temp->r) {
        //if the length is nonzero, the left subtree is null, and the right
        //subtree is also null, we've made a mistake.
        return traverse(len - 1, temp->r);
      }
    }
  }
}

//insert puts a node (and its children) at the leftmost part of the tree,
//updating size values as it walks along.
void insert(struct node *n) {
  if (n) {
    struct node *temp = root;
    while (temp->l) {
      temp->data = temp->data + n->data;
      temp = temp->l;
    }
    temp->l = n;
    n->p = temp;
    temp->data = temp->data + n->data;
  }
}

//same as insert, but it inserts at the end.
void insertL(struct node *n) {
  if (n) {
    struct node *temp = root;
    while (temp->r) {
      //printf("yes\n");
      temp->data = temp->data + n->data;
      temp = temp->r;
    }
    temp->r = n;
    n->p = temp;
    temp->data = temp->data + n->data;
  }
}

//delete deletes the node at the leftmost part of the tree, updating size
//values as it walks along, and returns that node.
struct node *delete() {
  struct node *temp = root;
  if (temp->l) {
    while (temp->l) {
      temp->data = temp->data - 1;
      temp = temp->l;
    }
    struct node *par = temp->p;
    temp->p = NULL;
    par->l = NULL;
    if (temp->r) {
      struct node *rr = temp->r;
      par->l = rr;
      rr->p = par;
      temp->r = NULL;
    }
    //par->data = par->data - 1;
    temp->data = 1;
    return temp;
  }
  //At the root
  else if (temp->r) {
    struct node *child = temp->r;
    root = child;
    child->p = NULL;
    temp->r = NULL;
    temp->data = 1;
    return temp;
  }
}

//same as delete, except deletes from the end.
struct node *deleteL() {
  struct node *temp = root;
  if (temp->r) {
    while (temp->r) {
      temp->data = temp->data - 1;
      temp = temp->r;
    }
    struct node *par = temp->p;
    temp->p = NULL;
    par->r = NULL;
    if (temp->l) {
      struct node *ll = temp->l;
      par->r = ll;
      ll->p = par;
      temp->l = NULL;
    }
    //par->data = par->data - 1;
    temp->data = 1;
    return temp;
  }
  //At the root
  else if (temp->l) {
    struct node *child = temp->l;
    root = child;
    child->p = NULL;
    temp->l = NULL;

    return temp;
  }
}


void swap(int bias, int par, int len, struct node *T[], bool excise) {
  //printf(" swap\n");

  int len_l = 0;
  if (root->l) len_l = root->l->data;

  //printf("%d, %d\n", len_l, len);

  //If we are given a length equal to that of the left subtree, we know that
  //the splayed node's index is less than half, thus the right subtree is
  //very non empty.
  if (len_l == len && bias > -1) {
    struct node *x = root;
    struct node *xl = root->l;
    struct node *xr = root->r;
    //printf("%d\n", root->r->data);
    //printf("%d\n", T[2]->data);
    //not necessary
    // x->l = NULL;
    //x->r = NULL;

    xl->p = NULL;
    xr->p = NULL;

    root = xr;
    //printf("yes1");
    //printf("%d\n", root->l->l->data);

    if (len - 1 > 0) {
      //printf("yes");
      struct node *pivot = traverse(len - 1, xr);
      splay(pivot);
    }
    //printf("%d\n", T[2]->l->data);
    //printf("%d\n", root->l->data);

    struct node *pl = root->l;
    if (pl && xl->data > 1) {
      pl->p = NULL;
      root->l = NULL;
      root->data = root->data - pl->data;
    }

    if (xl->data > 1) {
      root->l = xl;
      xl->p = root;
      //printf("%d\n", T[2]->l->data);
      root->data = root->data + xl->data;
      //printf("%d\n", T[2]->l->data);
      //printf("%d\n", pl->data);
      insert(pl);
      //printf("%d\n", T[2]->l->data);
    }
    else {
      insert(xl);
    }
  }

  //Otherwise, if the bias is 0, and the length of the left subtree is greater
  //than that passed in, then we are swapping halves.
  else if (bias <= 0) {
    //Tree has odd number of elements, and we're swapping half
    if (par == 1) {
      //printf("odd\n");
      //printf("%d\n", T[1]->data);
      /*
      struct node *pivot = traverse(len, root);
      splay(pivot);

      struct node *pl1 = root->l;
      pl1->p = NULL;
      root->l = NULL;
      root->data = root->data - pl1->data;

      pivot = traverse(len, root);
      splay(pivot);

      struct node *pl2 = root->l;
      pl2->p = NULL;
      root->l = NULL;
      root->data = root->data - pl1->data;

      root->l = pl1;
      pl1->p = root;
      root->data = root->data + pl1->data;
      insert(pl2);
      */
      //size_check(T);
      //printf("%d\n", root->l->r->data);
      struct node *last = deleteL();
      //size_check(T);
      //printf("%d\n", root->data);
      struct node *pivot = traverse(len, root);
      splay(pivot);
      //printf("%d\n", root->l->data);
      struct node *pl = root->l;
      pl->p = NULL;
      root->l = NULL;
      root->data = root->data - pl->data;
      //printf("%d\n", pl->data);
      struct node *pr = root;
      root = pl;

      if (excise) {
        root->l = pr->r;
        //printf("%d\n", last->data);
        insertL(last);
        //printf("%d\n", last->data);
      }
      else {
        insert(pr);
        //size_check(T);
        insertL(last);
        //printf("%d\n", root->data);
        //size_check(T);
      }
    }
    //Tree has even number of elements, and we're swapping half
    else {
      //printf("%d\n", root->data);
      //printf(" even\n");
      struct node *pivot = traverse(len, root);
      splay(pivot);
      //size_check(T);
      //printf("%d\n", T[1]->data);

      struct node *pl = root->l;
      pl->p = NULL;
      root->l = NULL;
      root->data = root->data - pl->data;

      struct node *pr = root;
      root = pl;

      insert(pr);
      //printf("%d\n", T[0]->data);
    }
  }
  //Tree has an odd number of elements, and the desired element is at the end
  //of the tree.
  else if (bias > 0) {
    //size_check(T);
    //printf(" odd end\n");
    //printf(" %d\n", root->l->r->r->data);
    struct node *first = delete();
    //printf("%d\n", first->r->data);
    //printf(" %d\n", root->l->p->data);
    //printf("yes\n");
    //size_check(T);
    //printf("%d\n", root->l->data);
    struct node *pivot = traverse(len, root);
    splay(pivot);
    //printf("yes\n");
    //printf(" %d\n", root->l->p->data);
    //size_check(T);
    //printf("yes2\n");
    //printf("%d\n", root->l->data);
    //printf("yes3\n");
    struct node *pl = root->l;
    //printf("%d\n", pl->data);
    //printf("yes2");
    pl->p = NULL;
    root->l = NULL;
    root->data = root->data - pl->data;
    //printf(" %d\n", pl->data);
    struct node *pr = root;
    root = pl;
    //printf("%d\n", root->data);
    //printf("%d\n", pr->data);
    //printf("%d\n", first->data);
    //printf("yes3\n");
    insert(pr);
    //printf(" %d\n", root->l->p->data);
    //size_check(T);
    insert(first);
    //printf(" %d\n", first->data);
    //size_check(T)
    //printf("%d\n", root->l->l->r->data);
  }
  else printf("there's a mistake!");
}

void z_printf(int i, int len) {
  if (len > 0) {
    printf("%d %d\n", i, len);
  }
}

int main(int argc, char **argv) {
  /* Get/Parse inputs */
  int j;
  scanf("%d", &n);
  int A[n];
  for(j = 0; j < n; j++) scanf("%d", &A[j]);


  /* Test
  n = 4;
  int A[n];
  A[0] = 3;
  A[1] = 2;
  A[2] = 1;
  A[3] = 0; */
  //A[4] = 2;
  //A[5] = 5;

  struct node *T[n];

  /* Preprocess */
  struct node *temp = (struct node*)malloc(sizeof(struct node));
  temp->data = n;
  temp->r = NULL;
  temp->p = NULL;
  root = temp;
  T[A[n-1]] = temp;

  int i;
  for(i = n - 2; i > -1; i--) {
    struct node *temp2 = (struct node*)malloc(sizeof(struct node));
    temp2->data = i + 1;
    temp2->r = NULL;
    temp2->p = temp;
    T[A[i]] = temp2;

    temp->l = temp2;
    temp = temp2;
  }

  //size_check(T);

  /* Splay Tests

  struct node *temp1 = root;
  while (temp1->r) {
    printf("%d\n", temp1->data);
    temp1 = temp1->r;
  }
  printf("%d\n", temp1->data);
  while(temp1->p) {
    printf("%d\n", temp1->data);
    temp1 = temp1->p;
  }
  */

  /* Insert Tests
  struct node *temp1 = (struct node*)malloc(sizeof(struct node));
  temp1->data = 2;
  temp1->r = NULL;
  temp1->p = NULL;
  T[5] = temp1;

  struct node *temp2 = (struct node *)malloc(sizeof(struct node));
  temp2->data = 1;
  temp2->r = NULL;
  temp2->l = NULL;
  temp2->p = temp1;

  temp1->l = temp2;
  T[6] = temp2;

  insert(temp1);

  //printf("%d\n", root->l->data);
  splay(T[A[3]]);
  printf("%d\n", root->l->data);
  printf("%d\n", root->r->data);
  printf("%d\n", root->l->l->data);
  */

  /* Delete Tests
  splay(T[A[2]]);
  printf("%d\n", root->l->data);
  printf("%d\n", root->r->data);
  struct node *temp1 = delete();
  printf("%d\n", temp1->data);
  printf("%d\n", root->l->data);
  printf("%d\n", root->r->data);
  insert(temp1);
  printf("%d\n", root->data);
  printf("%d\n", root->r->data);
  printf("%d\n", root->l->data);
  */

  /* Traverse Tests
  struct node *temp1 = traverse(1, root);
  splay(temp1);
  printf("%d\n", root->l->data);
  */
  //size_check(T);
  /* Sorting*/
  for(i = 0; i < n - 1; i++) {
    struct node *pointer = T[i];
    //size_check(T);
    //printf("%d\n", root->data);
    splay(pointer);
    //size_check(T);
    //printf("%d\n", root->data);
    //size_check(T);
    int index = i;
    //printf("%);
    if (root->l) index = root->l->data + i;
    int left = n - i;
    int half = left / 2;

    //printf("%d, %d\n", index, i);
    if (index == i) {
      //printf("case 0\n");
      struct node *r = root;
      struct node *rr = root->r;
      root = rr;
      if(root) root->p = NULL;
      //printf("%d\n", root->data);
      //size_check(T);
      //printf("\n");
    }
    else if (left % 2 == 0) {
      if (index < half + i) {
        //printf("case 1\n");
        //size_check(T);
        swap(0, 0, index - i, T, true);
        z_printf(i, index - i);

        //size_check(T);
        //break;
        //printf("\n");
      }
      else {
        //printf("case 2\n");
        //size_check(T);
        //printf("%d\n", pointer->data);
        int x = 0;
        if (index == half + i) x = -1;
        swap(x, 0, half, T, false);
        z_printf(i, half);
        //size_check(T);
        //printf("%d\n", T[0]->data);
        //printf("%d\n", root->l->data);
        //printf("%d\n", pointer->data);
        //printf("yes");
        splay(pointer);
        //printf("%d\n", pointer->data);
        //printf("%d\n", root->r->data);
        //printf("%d\n", T[3]->data);
        int index2 = i;
        if (root->l) index2 = root->l->data + i;
        //printf("%d\n", index2);
        if (index2 > i) {
          swap(0, 0, index2 - i, T, true);
          z_printf(i, index2 - i);
        }
        else {
          struct node *r = root;
          struct node *rr = root->r;
          root = rr;
          rr->p = NULL;
          r->r = NULL;
        }
        //printf("%d\n", root->r->data);
        //printf("%d\n", T[2]->data);
        //printf("\n");

      }
    }
    else {
      if (index == n - 1) {
        //printf("%d\n", root->data);
        //printf("%d\n", root->l->p->data);
        //size_check(T);
        //printf("case 3\n");
        //printf("%d\n", root->data);
        swap(1, 1, half, T, false);
        z_printf(i + 1, half);

        //printf("%d\n", root->l->l->data);
        //size_check(T);
        //printf("%d\n", root->l->l->r->data);
        splay(pointer);
        //printf("%d\n", root->l->data);

        int index2 = root->l->data + i;
        swap(-1, 1, half, T, true);
        z_printf(i, half);

        //printf("%d\n", root->l->data);
        //size_check(T);
        //printf("\n");
        //break;
      }
      else if (index >= half + i) {
        //printf("case 4\n");
        int x = 0;
        if (index == half + i) x = -1;
        swap(x, 1, half, T, false);
        z_printf(i, half);

        //size_check(T);
        //printf("%d\n", pointer->r->r->data);
        //size_check(T);

        splay(pointer);

        int index2 = i;
        if (root->l) index2 = root->l->data + i;
        //printf("%d\n", index2);
        if (index2 > i) {
          swap(0, 0, index2 - i, T, true);
          z_printf(i, index2 - i);
        }
        else {
          struct node *r = root;
          struct node *rr = root->r;
          root = rr;
          rr->p = NULL;
          r->r = NULL;
        }
        //printf("\n");
        //printf("%d\n", root->data);
        //size_check(T);
      }
      else {
        //printf("case 5\n");
        swap(0, 1, index - i, T, true);
        z_printf(i, index - i);

        //printf("\n");
      }
    }
  }
  //printf("yes\n");

  printf("%d %d\n", 0, 0);

  return 0;
}
