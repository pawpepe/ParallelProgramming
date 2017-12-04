#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <time.h>
#include <math.h>

void init(int **sequence, int n){
	
}


int main(){

	double start, end;
	int *sequence;
	double cpu_time_used;
	start = opm_get_wtime();;
	int s_seq,d,i,j;		
        int log2;
	int aux, index, index2,p;
	for(p=2; p<=4; p++){
		s_seq =pow(2,p);
                
		//alocating memory for the array of numbers to be summed and it's auxiliar one 
        	//init(&sequence, s_seq);
 		
		int k;
		if(!(sequence = (int *) malloc (sizeof (int) * s_seq))) return -1;
		//initializing the array and its auxiliar 
		for(k=0; k< s_seq; k++){
			sequence[k]=k;
			}
		log2 = (int)log(s_seq)/log(2);

		for(i=0; i< log2; i++){
	
			for(j=0; j< s_seq; j++){
				 if((j% (int)pow(2,i+1)) == (int) (pow(2,i+1)-1)){
					index =j+pow(2,i);
					sequence[j] += sequence[index];
				}
			}

                }
		
		sequence[s_seq -1] =0 ;
		
		
		for(i=log2; i>=0 ; i--){
			for(j=0; j< s_seq; j++){
			    if((j%(int)pow(2,i+1)) == (int)( pow(2,i+1)-1)){
				index = j - pow(2,i);
				aux = sequence[j];
				sequence[j]  = sequence[index] + aux;
				sequence[index] = aux;
			}
		
   			}
		}

		for(i=0; i < s_seq; i++){
			printf("%d ", sequence[i]);
		} 	
		printf("\n");
		
		//free(sequence);
		

      }

       
	end = opm_get_wtime();
	cpu_time_used = end - start;
	
	printf("%f\n", cpu_time_used);
	return 0;

}
