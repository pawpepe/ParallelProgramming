#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#include <limits.h>



int main(int argc, char* argv[]){
	int (*a)[128];
	int (*b)[128];
	int (*c)[128];
	int ** chunks, **chunks2,**chunks3;
	int i,j, rank, size;
	FILE *f = fopen("result.txt", "w");
        
	MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD,&rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
	if(rank==0){
	
		a = malloc(128*sizeof(*a));
		b = malloc(128*sizeof(*b));
		c = malloc(128*sizeof(*c));
	
		for(i=0; i<128; i++)
			for(j=0; j<128; j++){
				a[i][j]=i;
				b[i][j]=i; 
			}

	}
	
	
	int *aux = (int*)malloc(16*128*sizeof(int));
	chunks=(int**)malloc(16*sizeof(int*));
	
	for(i=0; i<16;i++)
		chunks[i]=&aux[i*128];

	
	int *aux2 = (int*)malloc(16*128*sizeof(int));
	chunks2=(int**)malloc(16*sizeof(int*));
	
	for(i=0; i<16;i++)
		chunks2[i]=&aux2[i*128];

	int *aux3 = (int*)malloc(16*128*sizeof(int));
	chunks3=(int**)malloc(16*sizeof(int*));
	
	for(i=0; i<16;i++)
		chunks3[i]=&aux3[i*128];

	MPI_Scatter(&(a[0][0]), 16*128, MPI_INT, &(chunks[0][0]),16*128, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(&(b[0][0]), 16*128, MPI_INT, &(chunks2[0][0]),16*128, MPI_INT, 0, MPI_COMM_WORLD);

	for(i=0; i<16;i++){
		for(j=0;j<128;j++){
			chunks3[i][j]=chunks[i][j]+chunks2[i][j];
	        }
        }
	
	MPI_Gather(&(chunks3[0][0]), 16*128, MPI_INT, &(c[0][0]),16*128,MPI_INT,0,MPI_COMM_WORLD);
//	printf("Rank %d \n " , rank);

	if(rank==0){
		for(i=0; i<128; i++){
                        for(j=0; j<128;j++){
                                fprintf(f,"%d ", c[i][j]);
                        }
                        fprintf(f,"\n");
		}
		
	}	

	MPI_Finalize();
	fclose(f);
}
