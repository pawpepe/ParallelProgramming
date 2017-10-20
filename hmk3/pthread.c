#include <stdio.h>

#include <pthread.h>

#define T 8

int M1[128][128]={{0}};
int M2[128][128]={{0}};
int M3[128][128]={{0}};


void *addColumn(void* arg){
     int  id =* (int*) arg;
     int i=0;
     int j=0;
     for( i=id*16; i<=id*16+15;i++){
        for(j=0; j<128;j++){
                M3[i][j] = M1[i][j] + M2[i][j];

        }

     }

     pthread_exit(NULL);
}


void init(){
	int i, j;
	for(i=0; i<128; i++)
		for( j=0; j<128; j++)
			M2[i][j]=M1[i][j] = i;
			
}


void print(FILE *f){
	int i,j; 
	for(i=0; i<128; i++){
		for(j=0; j<128;  j++){
			printf("%d ",M3[j][i]);
			fprintf(f,"%d ", M3[j][i]);
		}
		printf("\n");
		fprintf(f,"\n");
	}

}

int main(){
	pthread_t thread[T];
	int tId[T];
	int i,j;
	FILE *f = fopen("output.txt","w");
	if(f==NULL){
		printf("Error opening the file");
	}	
	//initializing the matrices 
	init();
	for(i=0; i<T; i++){
		tId[i]=i;
	       if(pthread_create(&thread[i], NULL, addColumn, &tId[i])!=0){
			perror("pthread create failed");
		}
	
	}


	for(i=0; i<T ;i++){
		if(pthread_join(thread[i],NULL)!=0){
			perror("Pthread join fails");
		}
		fprintf(f,"Thread %d is ready!\n",i);
		printf("Thread %d is ready!\n", i);
	}
	
	print(f);
	fclose(f);

}
