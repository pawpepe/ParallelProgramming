Anna Paula Pawlicka Maule 
gl0779

This program in MPI was developed in the following way: 

1) Declared all the variables
2) Allocated enough space for matrix A, B, C, and the chunks that will be passed on the MPI functions Sacatter and Gather. Where C will have the result of the sum of A + B
3) Process zero initialize the values of matrix A and B
4) Init MPI communication among all the processors 
5) Used MPI Scatter function to split the matrix A and B in equal size chunks and send one chunk to each process
6) each process is responsible for calculating the sum of the chunk A and chunk B that it received
7) Used MPI Gather to combine all the chunks into matric C 
8) Process 0 saves the final result, Matrix C into the file "result.txt" 

MPI Scatter and MPI Gather take care of the Syncronization. So when the matrix C in step 8 is being saved in "result.txt" it's garanteed that all the chunks from the 8 processes are already all gathered in matrix C.


obs: Changed the values on the file "jobscript" to allocate 2 nodes and 4ppn. To meet the assigment requirement to have 8 processes. 


 
