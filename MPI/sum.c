#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <limits.h>

int a[128][128],b[128][128],c[128][128];

void sum(int lower, int upper){
	int i, j;
	for(i=lower; i<upper;i++){
		for(j=0; j<128;j++){
			c[i][j] = a[i][j] + b[i][j];
		}

	}

}

main(int argc, char** argv){
	 
        int j, i, k,l, index;
	int rank,size; 
	int upper, lower; 
	MPI_Request request; 
	MPI_Status status;
        int aux[16][128];
	//initializing matrix 
	for(i=0; i<128;i++)
		for(j=0;j<128;j++)
			a[i][j] = b[i][j] = i;
	
	 
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLS, &size);

	//work done by master 
	if(rank==0){
		sum(rank, rank+15);
		for(i=1; i<8;i++){
			 MPI_Isend(&b, 128*128, MPI_INT, i, MASTER_TO_SLAVE_TAG, MPI_COMM_WORLD, &request); 
			 MPI_Isend(&a, 128*128, MPI_INT, i, MASTER_TO_SLAVE_TAG, MPI_COMM_WORLD, &request);
		}

	}
		
	//work done by the slaves
	if(rank > 0) {
		for(i=1; i<8;i++){
	                MPI_RECV(b, 128*128, MPI_INT, 0, MASTER_TO_SLAVE_TAG,MPI_COMM_WORLD, &status);
	        	 MPI_RECV(a, 128*128, MPI_INT, 0, MASTER_TO_SLAVE_TAG,MPI_COMM_WORLD, &status);
 			index = 0;  
			for(k=rank*16; k<rank*16+15; k++){
				for(l=0; l<128; l++)
					aux[index][l] = a[k][l] + b[k][l];
			
				index++;
			}		 
			MPI_Barrier(MPI_COMM_WORLD);
		}	

	}


	//gathers the work from the slave processes 
	if(rank==0){

	}
	MPI_Finalize();

}
