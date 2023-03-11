#include <mpi.h>
#include <stdio.h>

#define ARRAY_SIZE 10

int main(int argc, char **argv)
{
    int send_array[ARRAY_SIZE];
    int recv_array[ARRAY_SIZE];

    // Initialize MPI
    int rank, size, i;
    double start_time, end_time, total_time;
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

    // Set up the array to be sent
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        send_array[i] = i + rank * ARRAY_SIZE;
    }

    // Send the array from rank 0 to all other ranks
    if (rank == 0)
    {
        MPI_Send(send_array, ARRAY_SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Recv(recv_array, ARRAY_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank %d received array:", rank);
        for (i = 0; i < ARRAY_SIZE; i++)
        {
            printf(" %d", recv_array[i]);
        }
        printf("\n");
    }

    // Finalize MPI
    MPI_Finalize();

    return 0;
}
