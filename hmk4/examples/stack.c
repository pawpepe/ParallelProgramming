#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

void prefixsum_inplace(float *x, int N) {
    float *suma;
    #pragma omp parallel
    {
        const int ithread = omp_get_thread_num();
        const int nthreads = omp_get_num_threads();
        #pragma omp single
        {
            suma = (float*) malloc(sizeof(float)*(nthreads+1));
            suma[0] = 0;
        }
        float sum = 0;
        #pragma omp for schedule(static)
        for (int i=0; i<N; i++) {
            sum += x[i];
            x[i] = sum;
        }
        suma[ithread+1] = sum;
        #pragma omp barrier
        float offset = 0;
        for(int i=0; i<(ithread+1); i++) {
            offset += suma[i];
        }
        #pragma omp for schedule(static)
        for (int i=0; i<N; i++) {
            x[i] += offset;
        }
    }
    free(suma);
}

int main() {
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    const int n = 2000000;
    float x[n];
    for(int i=0; i<n; i++) x[i] = i;
    prefixsum_inplace(x, n);
    end = clock();
	cpu_time_used = ((double)(end-start))/CLOCKS_PER_SEC;
	
	
   // for(int i=0; i<n; i++) printf("%f\n", x[i]);

    printf("%f\n", cpu_time_used);
}
