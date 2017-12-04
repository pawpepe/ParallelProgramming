#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

int main(){
	clock_t start, end;
	int *sequence;
	int *aux;
	int p,i,j,k;
	double cpu_time_used;
	start = clock();
	int s_seq;
	
	for(p=8; p<=14; p++){
	        s_seq =pow(2,p);
                
		//alocating memory for the array of numbers to be summed and it's auxiliar one 
        	sequence =(int*) malloc(s_seq*sizeof(int));
		aux = (int*) malloc(s_seq*sizeof(int));
		
			
		//initializing the array and its auxiliar 
		#pragma omp parallel for private(k)
		for(k=0; k< s_seq; k++)
				aux[k]=sequence[k]=k;
			
					
		for(j=0; j<log2(s_seq);j++){
			#pragma omp parallel private(i)
			{
				#pragma omp for
				for(i=1<<j; i< s_seq; i++)
					aux[i] = sequence[i] + sequence[i-1<<j];	
				
				#pragma omp for
				for(i=1<<j; i< s_seq; i++)
					sequence[i] = aux[i];
			
				
			}
		
		}
		
		
		for(i=0; i< s_seq; i++)
			printf("%d ", sequence[i]);

		printf("\n");
		aux = sequence = NULL;
		
	
	}
	
	end = clock();
	cpu_time_used = ((double)(end-start))/CLOCKS_PER_SEC;
	
	printf("%f\n", cpu_time_used);
	return 0;
}
