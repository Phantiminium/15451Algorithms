#include <stdio.h>
#include <stdbool.h>

/*
 * The code below sorts a given array using
 * block swaps. The general idea of the algorithm
 * is to sort starting from index 0, and increasingly
 * block swap the element that is supposed to be at
 * that index to it. This is broken up into a few
 * cases, which take into account whether the element
 * is currently above the halfway index of the unsorted
 * elements, whether the total number of sorted elements
 * is even, and a special case when the element is also
 * at the very end of the array.
 *
 * The code performs at most 4n swaps, as each case does
 * no more than 3 swaps, and the loop is run n times, for
 * each element in the array.
 *
 */

/* Code to find the index of an element */
int n;
int A[30000];

int find_n(int x) {
  int i;
  for (i = 0; i < n; i++) {
    if (A[i] == x) {
      return i;
    }
  }
}

/* Code to perform block swapping */
void swap(int index, int len) {
  int i;
  //printf("%d, %d\n", index, len);
  for (i = index; i < index + len; i++) {
    int temp1 = A[i];
    //printf("%d, %d\n", i, i + len);
    int temp2 = A[i + len];
    //printf("%d, %d\n", i, len);
    A[i] = temp2;
    A[i + len] = temp1;

    /*
    int j;
    printf("[ ");
    for (j = 0; j < n; j++) {
      printf("%d ", A[j]);
    }
    printf("]\n");
    */
  }


}

void z_printf(int i, int len) {
  if (len > 0) {
    printf("%d %d\n", i, len);
    /*
    int j;
    printf("[ ");
    for (j = 0; j < n; j++) {
      printf("%d ", A[j]);
    }
    printf("]\n");
    */
  }
}

int main(int argc, char **argv) {

  /* Get/Parse inputs */

  int j;
  scanf("%d", &n);
  for(j=0;j<n;j++) scanf("%d", &A[j]);


  /* Test
  A[0] = 0;
  A[1] = 4;
  A[2] = 1;
  A[3] = 3;
  A[4] = 2;
  */


  /* Sort */

  int i;
  for (i = 0; i < n - 1; i++) {
    int index = find_n(i);
    int left = n - i;
    int half = left / 2;

    if (left % 2 == 0) {
      if (index < half + i) {
        //printf("\n\ncase 1\n");
        swap(i, index - i);
        z_printf(i, index - i);
      }
      else {
        //printf("\n\ncase 2\n");
        swap(i, half);
        z_printf(i, half);

        int index2 = find_n(i);
        swap(i, index2 - i);
        z_printf(i, index2 - i);
      }
    }
    else {
      if (index ==  n - 1) {
        //printf("\n\ncase 3\n");
        swap(n - 2, 1);
        z_printf(n - 2, 1);

        swap(i, half);
        z_printf(i, half);

        int index2 = find_n(i);
        //printf("%d, %d\n", index2, i);
        swap(i, index2 - i);
        z_printf(i, index2 - i);
      }
      else if (index >= half + i) {
        //printf("\n\ncase 4\n");
        swap(i, half);
        z_printf(i, half);

        int index2 = find_n(i);
        swap(i, index2 - i);
        z_printf(i, index2 - i);
      }
      else {
        //printf("\n\ncase 5\n");
        swap(i, index - i);
        z_printf(i, index - i);
      }
    }
  }
  printf("%d %d\n", 0, 0);

  return 0;
}
