#include <mpi.h>
#include <stdio.h>

#define NUM_MESSAGES 10

int main(int argc, char **argv)
{
    int rank, size, message;
    int buffer_size = NUM_MESSAGES * sizeof(int);
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
    MPI_Buffer_attach(malloc(buffer_size), buffer_size);
    if (rank == 0)
    {
        int i;
        for (i = 0; i < NUM_MESSAGES; i++)
        {
            message = 0;
            MPI_Bsend(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
            printf("Process %d received message %d from process %d (%d/%d)\n", rank, message, status.MPI_SOURCE, i + 1, NUM_MESSAGES);
        }
    }
    else
    {
        int i;
        for (i = 0; i < NUM_MESSAGES; i++)
        {
            MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            printf("Process %d received message %d from process %d (%d/%d)\n", rank, message, status.MPI_SOURCE, i + 1, NUM_MESSAGES);
            message = 1;
            MPI_Bsend(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    MPI_Buffer_detach(&message, &buffer_size);
    MPI_Finalize();
    return 0;
}
