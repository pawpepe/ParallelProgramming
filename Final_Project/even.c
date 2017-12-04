#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include  <math.h>
#include <limits.h>


int cmpFunc(const void *a, const void *b){
	return (*(int*)a - *(int*)b);
}

int *split_compare(int *chunk, int* chunk2, int thebit, int chunkSize, int* count){
	
		int k,l,j;
		k=l=0;
		int *auxChunk;
		auxChunk = (int*) malloc((chunkSize)*sizeof(int));
		(*count) = 0;	
		if(thebit == 0){
			for(j=0;j<chunkSize;j++){
				if(chunk[k] <= chunk2[l]){
					auxChunk[j] = chunk[k];
					k++;
				}else{
					auxChunk[j] = chunk2[l];
					l++;
					(*count)++;
				}
			}			
		}

		//if processor is greater than 2^i keep(select the bigger half)  second half 
		else{ 
   			k = l = chunkSize -1;
			for(j=chunkSize -1 ;j>=0;j--){
				if(chunk[k] >= chunk2[l]){
					auxChunk[j] = chunk[k];
					k--;
				}else{
					auxChunk[j] = chunk2[l];
					l--;
					(*count)++;
				}
			}

		}
		
		for(j=0; j<chunkSize; j++){
			chunk[j] = auxChunk[j];
		}
		
		free(auxChunk);
		return chunk;

	}
void oddEvenBroadcast(int *chunk, int *chunk2, int rank, int i, int *thebit, MPI_Status *status){
		int chunkSize =  sizeof(chunk)/sizeof(chunk[0]);
	
		if(i%2==1){ //odd phase
			if(rank%2==1){
				if(rank!=7){
					MPI_Send(&chunk, chunkSize, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
					MPI_Recv(&chunk2, chunkSize, MPI_INT, rank+1, 0, MPI_COMM_WORLD,status);
					(*thebit)=0;
				}
			}
			else{
				if(rank!=0){
					MPI_Recv(&chunk2, chunkSize, MPI_INT, rank-1, 0, MPI_COMM_WORLD,status);
					MPI_Send(&chunk,chunkSize, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
					(*thebit)=1;
				}
			}	
		}else{//even phase
		
			if(rank%2==0){
					MPI_Send(&chunk, chunkSize, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
					MPI_Recv(&chunk2, chunkSize, MPI_INT, rank+1, 0, MPI_COMM_WORLD,status);
					(*thebit)=0;
			}
			else{
					MPI_Recv(&chunk2, chunkSize, MPI_INT, rank-1, 0, MPI_COMM_WORLD, status);
					MPI_Send(&chunk,chunkSize, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
					(*thebit)=1;
				}
		}
	
}
int main( int argc, char* argv[]){
	long int k,i,j;
	int rank,size, pSize;
	unsigned long int  numb; 
 	int *arr, *arr2,*arr3, *chunk, *chunk2, *auxChunk; 
	int aux, auxSize,l, power ; 
	int mask;
    	int masked_rank;
    	int thebit;
	int totalCount;
	int oddEven;
	int swap = INT_MAX;
	int count;
	int auxCount; 
	MPI_Status  status;
	int oddrank, evenrank;
	int swap2;
	double t1, t2;


	pSize = (int) log(size)/log(2);
	double p =5;
	numb = (unsigned long int) pow(2, p);
	unsigned int chunkSize =(unsigned int) pow(2, p-3);

	
	arr2 = (int*)malloc(numb*sizeof(int));
	arr3 = (int*) malloc(numb*sizeof(int));
	arr = (int*) malloc(numb*sizeof(int));
	chunk = (int*) malloc((chunkSize)* sizeof(int));
	chunk2 = (int*) malloc((chunkSize)*sizeof(int));
	auxChunk = (int*) malloc((chunkSize)*sizeof(int));
	MPI_Init(&argc, &argv); 

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);


	
	if(rank==0){
		
		for(i=0; i<numb; i++){
			arr3[i]=arr2[i]=arr[i] = rand()%(129);
			printf("%d ", arr[i]);
		}
		t1 = MPI_Wtime();
	
		qsort(arr2, numb, sizeof(int), cmpFunc);
		t2 = MPI_Wtime();
		printf("\nQsort: %f\n", t2-t1);
		for(i=0; i<numb; i++)
			printf("% d", arr2[i]);	
	}

	t1 = MPI_Wtime();
	// Scatter 
	MPI_Scatter(arr,chunkSize, MPI_INT,chunk, chunkSize, MPI_INT, 0,  MPI_COMM_WORLD);
		


	
	// sort the chunk received 
	qsort(chunk,chunkSize,sizeof(int), cmpFunc);


	for(i=2; i>=0; i--){
		mask =  1<< i;
		masked_rank = rank & mask; 
		thebit = masked_rank >> i;
		power = (int) pow(2,i);
		
	
		//Send their chunk to its pair id rank is smaller than 2^i send to rank + 2^i 
		if(thebit==0){
		       MPI_Send(chunk,chunkSize, MPI_INT,rank+power,0, MPI_COMM_WORLD); 
		       MPI_Recv(chunk2,chunkSize, MPI_INT,rank+power,0, MPI_COMM_WORLD,&status); 

		}else{
			MPI_Recv(chunk2, chunkSize, MPI_INT, rank-power,0, MPI_COMM_WORLD,&status);
			MPI_Send(chunk, chunkSize, MPI_INT, rank-power,0, MPI_COMM_WORLD);
		}
		
				   				
		chunk= split_compare(chunk, chunk2, thebit, chunkSize, &count);
	}
		MPI_Barrier(MPI_COMM_WORLD);	
	//Second phase
	
	swap=swap2=INT_MAX;
	i=0;
	while(swap!=0 ||  swap2!=0){
		totalCount=INT_MAX;
		thebit=1;
		if(i%2==1){ //odd phase
			if(rank%2==1){
				if(rank!=7){
					MPI_Send(chunk, chunkSize, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
					MPI_Recv(chunk2, chunkSize, MPI_INT, rank+1, 0, MPI_COMM_WORLD,&status);
					thebit=0;
				}
			}
			else{
				if(rank!=0){
					MPI_Recv(chunk2, chunkSize, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
					MPI_Send(chunk,chunkSize, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
					thebit=1;
				}
			}	
		}else{//even phase
		
			if(rank%2==0){
					MPI_Send(chunk, chunkSize, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
					MPI_Recv(chunk2, chunkSize, MPI_INT, rank+1, 0, MPI_COMM_WORLD,&status);
					thebit=0;
			}
			else{
					MPI_Recv(chunk2, chunkSize, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
					MPI_Send(chunk,chunkSize, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
					thebit=1;
				}
		}
	

		if(!(i%2==1 && (rank==0 || rank==7))){
			chunk=split_compare(chunk, chunk2, thebit, chunkSize, &count);
		}
		if(i==1){
			MPI_Allreduce(&count,&totalCount,1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
			if(totalCount==0)
				swap=0;

		}else{
			MPI_Allreduce(&count,&totalCount,1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
			if(totalCount==0)
				swap2=0;
		}
		if(i==0)
			i=1;
		else 
			i=0;
		 
	}		
		
	//Gather all the chunks back to one vector 
	MPI_Gather(chunk, chunkSize, MPI_INT,arr, chunkSize, MPI_INT, 0, MPI_COMM_WORLD);
	// odd-even phase algorithm 
	if(rank==0){
		t2 = MPI_Wtime();
		printf("\nShell Sort time:%f\n", t2-t1);
		for(i=0; i<numb; i++){
			printf("%d ", arr[i]);
		}
	}
	//ODD_EVEN SORT
	t1 = MPI_Wtime();
	MPI_Scatter(arr3, chunkSize, MPI_INT, chunk, chunkSize, MPI_INT, 0, MPI_COMM_WORLD);
	// sort the chunk received 
	qsort(chunk,chunkSize,sizeof(int), cmpFunc);


	for(i=0; i<size; i++){	
		if(i%2==1){ //odd phase
			if(rank%2==1){
				if(rank!=7){
					MPI_Send(chunk, chunkSize, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
					MPI_Recv(chunk2, chunkSize, MPI_INT, rank+1, 0, MPI_COMM_WORLD,&status);
					thebit=0;
				}
			}
			else{
				if(rank!=0){
					MPI_Recv(chunk2, chunkSize, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
					MPI_Send(chunk,chunkSize, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
					thebit=1;
				}
			}	
		}else{//even phase
		
			if(rank%2==0){
					MPI_Send(chunk, chunkSize, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
					MPI_Recv(chunk2, chunkSize, MPI_INT, rank+1, 0, MPI_COMM_WORLD,&status);
					thebit=0;
			}
			else{
					MPI_Recv(chunk2, chunkSize, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
					MPI_Send(chunk,chunkSize, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
					thebit=1;
				}
		}
	

		if(!(i%2==1 && (rank==0 || rank==7))){
			chunk=split_compare(chunk, chunk2, thebit, chunkSize, &count);
		}
	
	}		
	

	MPI_Gather(chunk, chunkSize, MPI_INT, arr3, chunkSize, MPI_INT, 0, MPI_COMM_WORLD);
	t2 = MPI_Wtime();
	if(rank==0){
		printf("\nOdd-Even sort: %f\n", t2-t1);
		for(i=0; i<numb; i++){
			printf("%d ", arr3[i]);
	
		}
		
	}		
	MPI_Finalize();
	
	return 0; 
}
