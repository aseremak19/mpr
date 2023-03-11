#include <mpi.h>
#include <stdio.h>

#define ARRAY_SIZE 10

int main(int argc, char **argv)
{

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

    int iteration_limit = 10;

    int send_0_array[iteration_limit];
    int recv_1_array[iteration_limit];
    int recv_0_array[iteration_limit];

    // Set up the array to be sent

    for (i = 0; i < iteration_limit; i++)
    {
        send_0_array[i] = i + rank * iteration_limit;
    }

    i = 0;
    while (i < iteration_limit)
    {
        printf("Iteration: %d \niteration limit: %d\n", i, iteration_limit);
        // Send the array from rank 0
        if (rank == 0)
        {
            MPI_Send(send_0_array, ARRAY_SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(recv_0_array, ARRAY_SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Rank %d received array:", rank);
            for (i = 0; i < ARRAY_SIZE; i++)
            {
                printf(" %d", recv_0_array[i]);
            }
            printf("\n");
        }
        else
        {
            MPI_Recv(recv_1_array, ARRAY_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Rank %d received array:", rank);
            for (i = 0; i < ARRAY_SIZE; i++)
            {
                printf(" %d", recv_1_array[i]);
            }
            printf("\n");
            MPI_Send(recv_1_array, ARRAY_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
        i++;
    }

    // Finalize MPI
    MPI_Finalize();

    return 0;
}
