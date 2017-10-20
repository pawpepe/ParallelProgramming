#include <stdio.h>
#include <math.h>
#include <omp.h>

int main(){

	int sequence[10] = {0,1,2,3,4,5,6,7,8,9};
	for(int k=0; k<10; k++){
		printf("Numero: \n");
		scanf("%d", &sequence[k]);
	}
	int jump=0;
	int i,j; 
	printf("Sizeof %d", (int)(sizeof(sequence)/sizeof(int)));
	while(pow(2,jump) < 10){
		#pragma omp for schedule(static)  			
		for(i=9; i>0; i--) {
			j = i -pow(2,jump);
			sequence[i] = sequence[i] + sequence[j];	
			printf("%d ", sequence[i]);	
			if((i-1-pow(2,jump))<0){
				i=0;	
				}	
		
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
