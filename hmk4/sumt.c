#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

int main(){
	clock_t start, end;
	int *sequence;
	int *aux;
	int p;
	double cpu_time_used;
	start = clock();
		
	for(p=2; p<=4; p++){
		int s_seq =pow(2,p);
                
		//alocating memory for the array of numbers to be summed and it's auxiliar one 
        	sequence =(int*) malloc(s_seq*sizeof(int));
		aux =(int*) malloc(s_seq*sizeof(int));
		

		//initializing the array and its auxiliar 
		for(int k=0; k<s_seq; k++){
			aux[k]=sequence[k]=k;
		}
		int jump=0;
		int i,j;
		int a =0;
		while(pow(2,jump) < s_seq){
	
			#pragma omp parallel 
			{	
				if(a==0){
					printf("numbrs of threads: %d\n", omp_get_thread_num());
				}
				a=1;
			
				#pragma omp for private(i) private(j)
	 			for(i=s_seq-1; i>0; i--) {

					j = i-pow(2,jump);
					
					if(j>=0){
					     aux[i] = sequence[i] + sequence[j];	
						}	
			
			}
			
			#pragma omp barrier
				#pragma omp for private(i)
				for(i=0; i<s_seq; i++){
					sequence[i] = aux[i];
				}
				 
			}
		
			jump++;
		
		}
	
		for(i=0; i <s_seq; i++){
			printf("%d ", sequence[i]);
		} 	
		sequence = NULL;
		printf("\n");
	}

	end = clock();
	cpu_time_used = ((double)(end-start))/CLOCKS_PER_SEC;
	
	printf("%f\n", cpu_time_used);
	return 0;



}
