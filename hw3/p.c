#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int main(int argc, char **argv) {
  int n = 0;
  //char A[10^6];

  int j;

  //scanf("%c", &n);
  /*
  for(j=0; j<10^6;j++) {
    scanf("%c", &A[j]);
    n += 1;
  }
  */
  char s[10^6];
  scanf("%s", s);
  n = strlen(s);

  //printf("%s", s);
  //printf("\n");
  //printf("%d\n", n);
  //n = 8;
  //char A[] = {'a', 'a', 'a', 'T', 'a', 'a', 'c', 'a'};

  //n = 13;
  //char A[]= {'d', 'o', 'g', 'a', 'a', 'a', 'a', 'T', 'a', 'a', 'c', 'a', 't'};

  //n = 10;
  //char A[] = {'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a'}; */
  /*
  int j;
  for (j = 0; j < n; j++) {
    printf("%c ", A[j]);
  }
  */

  //Preprocess A
  char B[2*n + 3];
  int i;
  B[0] = '$';
  B[2*n + 2] = '@';
  for (i = 1; i < 2 * n + 2; i++) {
    if (i % 2 == 0) B[i] = s[i / 2 - 1];
    else B[i] = '#';
  }

  int P[2 * n + 3];
  int O[2 * n + 3];
  //O[0] = 0;
  //O[1] = 0;

  /*
  for (i = 0; i < 2 * n + 3; i++) {
    printf("%c ", B[i]);
  }
  printf("\n");
  */

  int C = 0;
  int R = 0;

  int max = 0;
  int index = 0;

  int max2 = 0;
  int index2 = 0;

  for (i = 1; i < 2 * n + 2; i++) {
    int mirr = 2 * C - i;

    if (i < R) {
      if (R - i < P[mirr]) {
        P[i] = R - i;
        //O[i] = R - i;
      }
      else {
        P[i] = P[mirr];
        //O[i] = P[mirr];
      }
    }
    else {
      P[i] = 0;
    }
    //printf("%d\n", i);
    //printf("%d\n", P[i]);
    while (B[i + (1 + P[i])] == B[i - (1 + P[i])]) {
      /*
      if (i == 9) {
        printf("%c %c \n", B[i + (1 + P[i])], B[i + (1 + P[i])]);
      }
      */
      P[i]++;
      //O[i]++;
    }
    O[i] = P[i];
    //printf("%d\n", i);
    if ((i + 1 + O[i] < 2 * n + 3) && (i - (1 + O[i]) > 0)) {
      //printf("yes");
      O[i]++;
      while ((i + (1 + O[i]) < 2 * n + 3) && (i - (1 + O[i]) > 0) &&
             (B[i + (1 + O[i])] == B[i - (1 + O[i])])) {
        O[i]++;
      }
    }
    /*
    if ((i + (1 + O[i]) >= 2 * n + 3) || (i - (1 + O[i]) <= 0)) {
      //printf("yes");
      O[i] = 0;
    }
    */
    if (P[i] > max) {
      max = P[i];
      index = i;
    }

    if (O[i] > max2) {
      max2 = O[i];
      index2 = i;
    }

    if (i + P[i] > R) {
      C = i;
      R = i + P[i];
    }
  }

  for (i = index - max; i <= index + max; i++) {
    if (B[i] != '#') {
      printf("%c", B[i]);
    }
  }
  printf("\n");

  for (i = index2 - max2; i < index2 + max2; i++) {
    if (B[i] != '#' && B[i] != '@' && B[i] != '$') {
      printf("%c", B[i]);
    }
  }
  printf("\n");

  return 0;

}
