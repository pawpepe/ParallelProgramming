#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <limits.h>


main(int argc, char** argv){
        int a[128][128],b[128][128],c[128][128];
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
	MPI_Bcast(a, 128*128, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(b, 128*128, MPI_INT,0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
        //work done by everyone 
	index = 0; 
	for(i=rank*16; i<rank*16+15; i++)
		for(j=0; j<128;j++)
			aux[index][j] = a[i][j] + b[i][j]; 	
		
        MPI_Gather(aux, 16*128, MPI_INT,c, 16*128, MPI_INT,0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	
	if(rank==0){
		FILE *f = fopen("result.txt", "w");
		for(i=0; i<128; i++){
			for(j=0; j<128;j++){
		  		fprintf(f,"%d ", c[j][i]);
			}
			fprintf(f,"\n");
		}
	}
}
