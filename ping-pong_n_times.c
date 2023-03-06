#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int rank, size, message, i;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size != 2)
    {
        printf("This program requires exactly 2 MPI processes\n");
        MPI_Finalize();
        return 1;
    }
    int iterations_limit = 100;

    i = 0;

    while (i < iterations_limit)
    {
        if (rank == 0)
        {
            message = 0;
            MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
            printf("Process %d received message %d from process %d\n", rank, message, status.MPI_SOURCE);
            i++;
        }
        else
        {
            MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            printf("Process %d received message %d from process %d\n", rank, message, status.MPI_SOURCE);
            message = 1;
            MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
    return 0;
}
