#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

/*
 * The algorithm I used to calculated the maximum contiguous palindrome
 * substring with at most 1 mistake is coded below. First, I find the maximum
 * possible length of the interior palindrome using Manacher's algorithm. I
 * then calculate the maximum possible length of the outer "palindrome" using
 * a riff of the Rabin-Karp algorithm; I binary search on the range of
 * possible lengths of the outer palindrome, using a backward and forward
 * hash to efficiently calculate and hashes of substrings.
 *
 * We first have that all preprocessing steps are done in linear time. Next,
 * we have Manacher's algorithm, which runs in linear time.
 *
 * For the outer "palindrome" length calculation, a binary search on all
 * possible lengths is O(logn). Since we calculate this for all indices,
 * and the hash in computed in constant time, we have that the entire loop
 * is O(nlogn).
 *
 * Thus, as the random prime may have to be reselected, we have that the
 * expected runtime of the algorithm is O(c * nlogn) = O(nlogn).
 */

int n;
int x;

/*
 * Compute the forward hash of a char list, i.e. at every index i,
 * h(i) = b^(i-1)s_0 + b^(i-2)s_1 + ... + b^1s_(i-2) + b^0s_(i-1) mod p
 * [TESTED PROBABLY WORKING]
 */
void fHash(char *B, long *H, long b, long p) {
  H[0] = (long)B[0];
  int i;
  for (i = 1; i < 2 * n + 3; i++) H[i] = ((H[i - 1] * b) + (long)B[i]) % p;
}

/*
 * Compute the backward hash of a char list, i.e. at every index i,
 * h(i) = b^0s_i + b^1_s(i+1) + ... + b^(n-i-2)s_(n-2) + b^(n-i-1)s_(n-1) mod p
 * [TESTED PROBABLY WORKING]
 */
void bHash(char *B, long *H, long b, long p) {
  H[2 * n + 2] = (long)B[0];
  int i;
  for (i = 2 * n + 1; i > -1; i--) H[i] = ((H[i + 1] * b) + (long)B[i]) % p;
}

/*
 * Function to build the subhash from index f to index f + sign(l)(|l| - 1)
 * [TESTED PROBABLY WORKING]
 */
long subhash(long *H, long f, long l, long p, long *BP) {
  long e1 = H[f];
  long e2i = f + l;
  long e2;
  if (e2i < 0 || e2i > (2 * n) + 2) e2 = 0;
  else e2 = H[e2i];

  long hash = e1 - (BP[abs(l)] * e2);
  return (((hash) % p) + p) % p;
}

/*
 * We preform binary search over the possible values of the max input, n, and
 * return the maximum outer palindrome length
 */
long binsearch(long *FH, long *BH, int s, int e, int max, long p, long *BP) {
  int u = max;
  int l = 0;
  int m;
  //bool finished = false;
  bool works;
  //int counter = 0;

  while (true) {
    //printf("%d %d\n", u, l);
    m = (u + l) / 2;

    long s1 = subhash(FH, s, -m, p, BP);
    long s2 = subhash(BH, e, m, p, BP);

    works = s1 == s2;

    //We are finished when the top is equal to the bottom.
    if (u == l) {
      if (works) return m;
      else return m - 1;
    }
    else if (u - l == 1) {
      long f1 = subhash(FH, s, -u, p, BP);
      long f2 = subhash(BH, e, u, p, BP);
      if (f1 == f2) return u;
      else return l;
    }

    //If s1 == s2:
    //Range of possible values goes from m to t, as we know m to be the
    //lowest possible value of the longest outer palindrome length.
    if (works) l = m;

    //If s1 != s2:
    //Range of posible values goes from b to m - 1, because we know an
    //outer palindrome of length m does not work, so the possible values
    //goes up to length m - 1.
    else u = m - 1;

    //counter++;
  }
  return -1;
}

long prime(void) {
  long p;
  bool finished = false;
  while (!finished) {
    p = (long)rand() % (long)pow(2, 30);
    long m = (long)sqrt(p);
    long i;
    for (i = 1; i < m; i++) {
      if (p % i == 0) {
        break;
      }
      else if (i == m - 1) finished = true;
    }
  }

  return p;
}

int main(int argc, char ** argv) {
  srand(time(0));
  int j = rand() % 15;
  int i;
  long b = 256;
  long p[] = {883523327, 1840845547, 1863936989, 1552003097, 260725853,
              694415279, 620219987, 56730967, 102450979, 514067027,
              992648693, 478354453, 958157461, 429540647, 570508319};

  x = pow(10, 6);

  //Read in string
  char *s = malloc(sizeof(char) * x);
  scanf("%s", s);

  n = strlen(s);

  //Test degenerate case
  if (n == 0) {
    printf("\n");
    return 0;
  }

  //Preprocess string
  char *B = malloc(sizeof(char) * (2 * n + 3));
  B[0] = '@';
  B[2 * n + 2] = '$';

  for (i = 1; i < 2 * n + 2; i++) {
    if (i % 2 == 0) B[i] = s[i / 2 - 1];
    else B[i] = '#';
  }


  int *P = malloc(sizeof(int) * (2 * n + 3));
  P[0] = 0;
  P[2 * n + 2] = 0;
  int *P1 = malloc(sizeof(int) * (2 * n + 3));
  P1[0] = 0;
  P1[2 * n + 2] = 0;

  //Compute forward and backward hash at every index
  long *FH = malloc(sizeof(long) * (2 * n + 3));
  long *BH = malloc(sizeof(long) * (2 * n + 3));
  fHash(B, FH, b, p[j]);
  bHash(B, BH, b, p[j]);

  /* Create array of powers of b mod p */
  long *BP = malloc(sizeof(long) * n);
  BP[0] = 1;
  for (i = 1; i < n; i++) BP[i] = (BP[i - 1] * b) % p[j];

  //Run the algorithm
  bool finished = false;

  //int j;
  while (!finished) {

    //Compute longest inner palindrome at each index [TESTED CORRECT]
    int c = 0;
    int r = 0;

    for (i = 1; i < 2 * n + 2; i++) {
      //Find the mirror of the current index
      int m = 2 * c - i;

      //Assign the maximum possible deducible length of the palindrome at
      //using the mirror; 0 if no rules apply
      if (i < r) {
        if (r - i >= P[m]) P[i] = P[m];
        else P[i] = r - i;
      }
      else P[i] = 0;

      //Obtain maximum possible palindrome length using above deductions
      int l = P[i] + 1;
      while (B[i + l] == B[i - l]) {
          P[i]++;
          l++;
      }

      //If the palindrome extends past boundary, reassign boundaries and center
      if (i + l - 1 > r) {
        r = i + l - 1;
        c = i;
      }
    }

    //Compute longest outer palindrome at each index [TESTED CORRECT]
    for (i = 1; i < 2 * n + 2; i++) {
      //Get start and end index of inner palindrome
      int si = i - P[i] - 3;
      int ei = i + P[i] + 3;

      //Get maximum possible length of outer palindrome
      int max;
      if (si > (2 * n) + 2 - ei) max = si;
      else max = (2 * n) + 2 - ei;

      //Find the maximum length of outer palindrome
      if (si > 0 && ei < 2 * n + 2)
        P1[i] = binsearch(FH, BH, si, ei, max, p[j], BP);
      else if (si < -1 || ei > 2 * n + 3)
        P1[i] = -P[i] - 1;
      else
        P1[i] = 0;
    }

    finished = true;

  }

  /* Find maximum one mistake palindrome */
  int max1 = 0;
  int index1 = 0;
  for (i = 1; i < 2 * n + 3; i++) {
    if (P[i] + 2 + P1[i] > max1) {
      max1 = P[i] + 2 + P1[i];
      index1 = i;
    }
  }

  /* Find maximum perfect palindrome */
  int max0 = 0;
  int index0 = 0;
  for (i = 1; i < 2 * n + 3; i++) {
    if (P[i] > max0) {
      max0 = P[i];
      index0 = P[i];
    }
  }

  /* Find greater of the two */
  int max;
  int index;
  if (max0 == max1) {
    if (index0 < index1) {
      max = max0;
      index = index0;
    }
    else {
      index = index1;
      max = max1;
    }
  }
  else if (max0 > max1) {
    max = max0;
    index = index0;
  }
  //max1 > max0
  else {
    max = max1;
    index = index1;
  }

  /* Output calculated result */
  for (i = index - max; i <= index + max; i++) {
    bool top = i > -1;
    bool bot = i < 2 * n + 3;
    top = true;
    bot = true;
    if (B[i] != '#' && B[i] != '$' && B[i] != '@' && top && bot) {
      printf("%c", B[i]);
    }
  }
  printf("\n");

  return 0;
}
