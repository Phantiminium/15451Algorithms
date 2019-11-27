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
int find_n(int x, int n, int *arr) {
  int i;
  for (i = 0; i < n; i++) {
    if (arr[i] == x) {
      return i;
    }
  }
}

/* Code to perform block swapping */
void swap(int *A, int index, int len) {
  int i;
  for (i = index; i < len; i++) {
    int temp = A[i + len];
    A[i + len] = A[i];
    A[i] = temp;
  }
}

int main(int argc, char **argv) {
  //printf("yes");
  /* get/parse inputs */
  int n = 0;
  int *A = NULL;

  /* Sort */
  int i;
  for (i = 0; i < n; i++) {
    int index = find_n(i, n, A);
    int left = n - i;
    int half = left / 2;

    if (left % 2 == 0) {
      if (index < half + i) {
        swap(A, i, index - i);

        printf("%d %d\n", i, index - left);
      }
      else {
        swap(A, i, half);
        int index2 = find_n(i, n, A);
        swap(A, i, index2 - i);

        printf("%d %d\n", i, half);
        printf("%d %d\n", i, index2 - i);
      }
    }
    else {
      if (index ==  n - 1) {
        swap(A, n - 2, 1);
        swap(A, i, half);
        int index2 = find_n(i, n, A);
        swap(A, i, index2 - i);

        printf("%d %d\n", n - 2, 1);
        printf("%d %d\n", i, half);
        printf("%d %d\n", i, index2 - i);
      }
      else if (index >= half + i) {
        swap(A, i, half);
        int index2 = find_n(i, n, A);
        swap(A, i, index2 - i);

        printf("%d %d\n", i, half);
        printf("%d %d\n", i, index2 - i);
      }
      else {
        swap(A, i, index - i);

        printf("%d %d\n", i, index - i);
      }
    }
  }

  return 0;
}
