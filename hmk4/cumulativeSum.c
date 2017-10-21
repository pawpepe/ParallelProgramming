#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

int main(){

	

	int *sequence; 
	int *aux;
	
	int threadi, k; 
	
	for(int l=2; l<=4; l+=2){
		sequence =  (int*) malloc(pow(2,l));
		aux = (int*) malloc(pow(2,l));
		int stop = pow(2,l);
		#pragma omp parallel
                {      

                        #pragma omp for
			for( k=0; k<stop; k++){
				sequence[k] = aux[k] = k;
				printf("%d ",aux[k]);
			}
			#pragma omp barrier 
		}

		#pragma omp barrier 
		int jump=0;
		int i,j;
		
     		printf("\niniciated \n");
		
		while(pow(2,jump) < pow(2,l)){

			#pragma omp parallel
			{
			
				#pragma omp for private(i) 
				for(i=pow(2,l)-1; i>=0; i--) {
	
					j = i-pow(2,jump);
					
					if(j>=0){
					      aux[i] = sequence[i] + sequence[j];
						printf("%d ", aux[i]); 	
			                    }
				}
                               	printf("\n");

				#pragma omp barrier
					
				#pragma omp for private(i)
				for(i=0; i < stop; i++){
					sequence[i] = aux[i];
				}
				#pragma omp barrier 
			}
			#pragma omp barrier 
		
			jump++;
			
		}

		for(i=0; i <stop; i++){
			printf("%d ", sequence[i]);
		}
		printf("\n");	
		sequence = aux = NULL;
	}
return 0;



}
