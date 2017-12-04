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
	int s_seq,d,i,j;		
        int log2,last, u;
	int aux, index1, index2,index, p;
        //#pragma omp parallel for private(sequence)
        for(u=1; u<=8; u*=2){
           start = omp_get_wtime();
	for(p=8; p<=14; p++){
		s_seq =pow(2,p);
                
		//alocating memory for the array of numbers to be summed and it's auxiliar one 
        	//init(&sequence, s_seq);
 		
		int k;
		if(!(sequence = (int *) malloc (sizeof (int) * s_seq))) return -1;
		//initializing the array and its auxiliar 
		for(k=0; k< s_seq; k++){
			sequence[k]=k;
		}
		/**for(i=0; i < s_seq; i++){
			printf("%d ", sequence[i]);
		} 
		printf("\n");**/
		log2 = (int)(log(s_seq)/log(2));
		printf("log: %d \n",log2);
		
                for(i=0; i<log2; i++){
			#pragma opm parallel for privat(j) num_threads(u)
			for(j=pow(2,i)-1; j< s_seq; j+=pow(2,i+1)){
				index =j+pow(2,i);
				if(index < s_seq)
					sequence[index] += sequence[j];
		
			}
			#pragma omp barrier 
			
                }

		
		last = sequence[s_seq-1];
		sequence[s_seq-1] =0 ;
		/**printf("\n");
		for(int l=0; l < s_seq; l++)
			    printf("%d ", sequence[l]);
		 	
		         printf("\n\n");**/

		
		for(i=log2; i>0 ; i--){
			#pragma opm parallel for private(j) num_threads(u)
			for(j=s_seq-1; j>=0;j-=pow(2,i)){
			    index =  j - pow(2,i-1);
			    aux = sequence[index];
			    sequence[index] = sequence[j];
			    sequence[j] += aux;
		
   			}
			#pragma omp barrier 
			

		}
		sequence[0] =last;
		printf("\n\n\n\n");
		for(i=0; i < s_seq; i++){
			printf("%d ", sequence[i]);
		} 	
		
		
		free(sequence);
		

      } printf("Threads: %d Time: ", u);
	end = omp_get_wtime();
	cpu_time_used = end - start;
	
	printf("%f\n", cpu_time_used);
			
}

       
return 0;

}
