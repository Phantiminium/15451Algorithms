#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv) {
  int i, n, m, seed;

  //scanf("%d %d %d", &n, &m, &seed);
  n = atoi(argv[1]);
  m = atoi(argv[2]);
  seed = atoi(argv[3]);

  //printf("\n");
  srand(seed);

  printf("%d %d ", n, m);

  for (i = 0; i < m; i++) {
    int s, e;
    s = rand() % n;
    e = rand() % n;
    while (e < s) {
      s = rand() % n;
      e = rand() % n;
    }
    if (rand() % 2 == 0) {
      printf("IR %d %d %d ", s, e, (rand() % 2000) - 1000);
    }
    else {
      printf("RM %d %d ", s, e);
    }
  }

  printf("\n");

  return 0;
}
