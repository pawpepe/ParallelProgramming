#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <time.h>
#include <math.h>

int main()
{
  clock_t start, end;
  int *sequence, *partial, *aux;
  int numThreads, work, n;
  int i,k, mynum, last;
 
 
  double cpu_time_used;
  start = clock();
  
  for(k=8; k<=14; k++){

	  n = pow(2, k);
	  if(!(sequence = (int *) malloc (sizeof (int) * n))) return -1;

	  //#pragma omp parallel for private(i) 
	  for(i = 0; i<n; i++)
		sequence[i] =i;
	  

	  #pragma omp parallel default(none) private(i, mynum, last) shared(sequence, partial, aux, numThreads, work, n)
	  {
	    #pragma omp single
	    {
	      numThreads = omp_get_num_threads();
	      if(!(partial = (int *) malloc (sizeof (int) * numThreads))) exit(-1);
	      if(!(aux = (int *) malloc (sizeof (int) * numThreads))) exit(-1);
	      work = n / numThreads + 1; /*sets length of sub-arrays*/
	    }
	    mynum = omp_get_thread_num();
	   
	    // calculating prefix sum of each subarray
	    for(i = work * mynum + 1; i < work * mynum + work && i < n; i++)
	      sequence[i] += sequence[i - 1];
	    partial[mynum] = sequence[i - 1];
	    #pragma omp barrier
	    /*calculate prefix sum for the array that was made from last elements of each of the previous sub-arrays*/
	    for(i = 1; i < numThreads; i <<= 1) {
	      if(mynum >= i)
		aux[mynum] = partial[mynum] + partial[mynum - i];
		#pragma omp barrier
		#pragma omp single
	      memcpy(partial + 1, aux + 1, sizeof(int) * (numThreads - 1));
	    }
	    /*update original array*/
	    for(i = work * mynum; i < (last = work * mynum + work < n ? work * mynum + work : n); i++)
	      sequence[i] += partial[mynum] - sequence[last - 1];
	  }
	  for(i = 0; i < n; i++)
	    printf("%d ", sequence[i]);
	  printf("\n");
}
   end = clock();
   cpu_time_used = ((double)(end-start))/CLOCKS_PER_SEC;
	
   printf("%f\n", cpu_time_used);
  return 0;
}
