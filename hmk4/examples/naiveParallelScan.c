//Be sure to compile with std=c99
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h> 

#define N 20000000

int main(int argc, char ** argv)
{
  clock_t start, end;
  long log_of_N = (long)ceil(log(N)/log(2));
  long * arr = (long *) malloc(sizeof(long)*N);
  long ** scanSum = (long **) malloc(sizeof(long *)*2);
  scanSum[0] = (long *) malloc(sizeof(long)*N);
  scanSum[1] = (long *) malloc(sizeof(long)*N);

  int i, j;
  int in = 1, out = 0;
  double cpu_time_used;
  start = clock();

  //srand(1234567);
  for(i = 0; i < N; i++)
    arr[i] = i;

  memcpy(scanSum[out], arr, (N)*sizeof(long));
  memset(scanSum[in], 0, (N)*sizeof(long));
  int two_i = 0, two_i_1 = 0;

  printf("%ld\n", log_of_N);

  for(i = 1; i <= log_of_N; i++)
  {
    two_i_1 = 1 << (i-1);
    two_i = 1 << i;
    out = 1 - out;
    in = 1 - out;
    #pragma omp parallel for private(j) shared(scanSum, in, out)
    for(j = 0; j < N; j++)
    {
      if(j >= two_i_1)
        scanSum[out][j] = scanSum[in][j] + scanSum[in][j - two_i_1];
      else
        scanSum[out][j] = scanSum[in][j];
    }
  }

  for(i = 0; i < 10; i++)
    printf("i = %d, arr[%d] = %ld, scanSum[%d] = %ld\n", i, i, arr[i], i,
      scanSum[out][i]);

  for(i = N-10; i < N; i++)
    printf("i = %d, arr[%d] = %ld, scanSum[%d] = %ld\n", i, i, arr[i], i,
      scanSum[out][i]);

  free((void *) scanSum[0]);
  free((void *) scanSum[1]);
  free((void *) scanSum);
  free((void *) arr);

  end = clock();
  cpu_time_used = ((double)(end-start))/CLOCKS_PER_SEC;
	
   printf("%f\n", cpu_time_used);
  return 0;
}
