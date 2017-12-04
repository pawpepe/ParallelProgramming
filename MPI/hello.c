#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <limits.h>

main(int argc, char** argv)
{
        int node;

        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &node);

        char hostname[HOST_NAME_MAX];
        if (! gethostname(hostname, sizeof hostname) == 0)
          perror("gethostname");

        printf("Hello World from Node %s %d\n",hostname, node);

        MPI_Finalize();
}

