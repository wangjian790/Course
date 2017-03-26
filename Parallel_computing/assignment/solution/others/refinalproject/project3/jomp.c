#include <unistd.h>
#include <omp.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#define N 4096

float x[N+2][N+2];
float y[N+2][N+2];

#define myabs(a) (((a) > 0) ? (a):(-(a)))

int main(int argc, char *argv[]) 
{
  int NN;
  float a1, a2, a3, a4, a5, a6;
  float MAXDIFF;
  int i, j;
  int t, t1;
  float  maxdiff1;
  int iteration;
  FILE *fp;
  if(argc > 1) {
    omp_set_num_threads(atoi(argv[1]));
  }
  if( (fp = fopen("input.jacobi", "r+")) == NULL)
    {
      printf("No such file\n");
      exit(1);
    }  
  
  if (fp == NULL)
    {
      printf("Error Reading File\n");
    }
  fscanf(fp,"%d %f %f %f %f %f %f %f", &NN, &a1, &a2, &a3, &a4, &a5, &a6, &MAXDIFF);
  

  printf("%d %f %f %f %f %f %f %f\n", NN, a1, a2, a3, a4, a5, a6, MAXDIFF);   
  /*  a1 = a2 = a3 = a4 = 0.25; a6 = 0; a5 = 0.1;

  MAXDIFF = 0.0001;
  */

  printf("maxdiff = %13.12f\n", MAXDIFF);
  for (i=1; i<=NN+1; i++) {
    x[i][0] = a5*i;
    y[i][0] = a5*i;

    x[i][NN+1] = a6*i;
    y[i][NN+1] = a6*i;

    x[0][i] = 0.0;
    y[0][i] = 0.0;

    x[NN+1][i] = 0.0;
    y[NN+1][i] = 0.0; 
  }

  for (i=1; i<=NN; i++) 
    for (j=1; j<=NN; j++) {
      x[i][j] = 0.0;
      y[i][j] = 0.0;
    }

  printf("maxdiff=%13.12f\n", MAXDIFF);

  t = 0;
  maxdiff1 = 100000.0;
  iteration = 0;

  while (maxdiff1 > MAXDIFF) {
    maxdiff1 = -1.0;
    if(t == 0){
#pragma omp parallel for shared (NN, t, t1, a1, a2, a3, a4, x) private(i, j)
    for (i=1; i <= NN; i++) 
      for (j=1; j <=NN; j++) {
        x[i][j] = a1*y[i-1][j] + a3 * y[i+1][j] +
	  a2*y[i][j-1] + a4 * y[i][j+1];
	if (myabs(x[i][j] - y[i][j]) > maxdiff1)
	  maxdiff1 = myabs(x[i][j] - y[i][j]);
      }
    t = 1;
    }
    else {
#pragma omp parallel for shared (NN, t, t1, a1, a2, a3, a4, x) private(i, j)
    for (i=1; i <= NN; i++) 
      for (j=1; j <=NN; j++) {
        y[i][j] = a1*x[i-1][j] + a3 * x[i+1][j] +
	  a2*x[i][j-1] + a4 * x[i][j+1];
	if (myabs(x[i][j] - y[i][j]) > maxdiff1)
	  maxdiff1 = myabs(x[i][j] - y[i][j]);
      }
    t = 0;
    }
    printf("iteration = %d, maxdiff1 = %f, MAXDIFF = %f\n", 
           iteration++, maxdiff1, MAXDIFF);
  }
  printf("MAXDIFF = %f, maxdiff = %f\n", MAXDIFF, maxdiff1);
  
  if ((i=open("jomp.output", O_WRONLY | O_CREAT | O_TRUNC, 0600)) < 0) {
    fprintf(stderr, "Cannot open file proj3.output.\n");
    exit(0);
  }

  for (j = 0; j <= NN + 1; j++) {
    if(t==1){
      if ((t1= write(i, &(x[NN/2][j]),  sizeof(float))) != sizeof(float)) {
	fprintf(stderr, "write error %d %d.\n", j, t1);
	exit(0);
      }
    }
    else{
      if ((t1= write(i, &(y[NN/2][j]),  sizeof(float))) != sizeof(float)) {
	fprintf(stderr, "write error %d %d.\n", j, t1);
	exit(0);
      }
    }
  }

  for (j = 0; j <= NN + 1; j++) {
    if(t == 1){
      if ((t1 = write(i, &(x[j][NN/2]),  sizeof(float))) != sizeof(float)) {
	fprintf(stderr, "write error. %d %d\n", j, t1);
	exit(0);
      }
    }
    else{
      if ((t1 = write(i, &(y[j][NN/2]),  sizeof(float))) != sizeof(float)) {
	fprintf(stderr, "write error. %d %d\n", j, t1);
	exit(0);
      }
    }
  }
  close(i);
  return 0;
}
