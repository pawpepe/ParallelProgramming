#include <stdio.h>
#include <math.h>
#include <omp.h>

int main(){

	int sequence[10] = {0,1,2,3,4,5,6,7,8,9};
	int aux[10];

	for(int k=0; k<10; k++){
		aux[k] = sequence[k];
	}
	int jump=0;
	int i,j; 
	while(pow(2,jump) < 10){

		#pragma omp parallel
		{
			
			#pragma omp for private(i) private(j)
	 		for(i=9; i>0; i--) {

				j = i-pow(2,jump);
					
				if(j>=0){
				      aux[i] = sequence[i] + sequence[j];	
				      printf("%d ",aux[i]);
					}	
			
			}
			#pragma omp barrier 
			
			#pragma omp for private(i)
			for(i=0; i<10; i++){
				sequence[i] = aux[i];
			}
			#pragma omp barrier 
		}
		
		printf("\n ");
		jump++;
		
	}

	for(i=0; i <10; i++){
		printf("%d ", sequence[i]);
	}	

	printf("\n \n \n");
return 0;



}
