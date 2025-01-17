/* This is the MPI program computing the value of PI */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int main(int argc, char *argv[])
{
  int done = 0, n, myid, numprocs, i, rc;
  double PI25DT = 3.141592653589793238462643;
  double mypi, pi, h, sum, x, a;

  if (argc != 2) {
    printf("Usage: a.out N\n");
    exit(0);
  }
  n = atoi(argv[1]);

  h   = 1.0 / (double) n;
  sum = 0.0;

#pragma omp parallel for shared (h) private(i, x) reduction(+: sum)
  for (i = 1; i <= n; i ++) {
    x = h * ((double)i - 0.5);
    sum += 4.0 / (1.0 + x*x);
  }

  mypi = h * sum;
  printf("pi is approximately %.16f, Error is %.16f, sum = %lf\n", mypi, fabs(mypi - PI25DT), sum);
  return 0;
}
