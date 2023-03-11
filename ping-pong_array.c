#include <mpi.h>
#include <stdio.h>
#include <time.h>

#define ARRAY_SIZE 10

int getRandom(int lower, int upper)
{
    int num = (rand() %
               (upper - lower + 1)) +
              lower;
    // printf("%d ", num);
    return num;
}

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

    int iteration_limit = 100;

    int send_0_array[iteration_limit];
    int recv_1_array[iteration_limit];
    int recv_0_array[iteration_limit];

    srand(time(0));

    // Set up the array to be sent

    for (i = 0; i < iteration_limit; i++)
    {
        send_0_array[i] = i + rank * getRandom(1, 15);
    }

    i = 0;
    int j;
    for (i = 0; i <= iteration_limit; i++)
    {
        // printf("Iteration: %d \niteration limit: %d\n", i, iteration_limit);
        //  Send the array from rank 0
        if (rank == 0)
        {
            MPI_Barrier(MPI_COMM_WORLD);
            start_time = MPI_Wtime();

            MPI_Ssend(send_0_array, i, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(recv_0_array, i, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            end_time = MPI_Wtime();
            total_time = (end_time - start_time) / 2;
            printf("Rank: %d; Total time: %.15f seconds\n", rank, total_time);

            printf("Rank %d received array:", rank);
            for (j = 0; j < i; j++)
            {
                printf(" %d", recv_0_array[j]);
            }
            printf("\n");

            // Measurign and displaying time
                }
        else
        {
            MPI_Barrier(MPI_COMM_WORLD);
            MPI_Recv(recv_1_array, i, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            /*printf("Rank %d received array:", rank);
            for (j = 0; j < i; j++)
            {
                printf(" %d", recv_1_array[j]);
            }
            printf("\n");
            printf("\n");*/

            MPI_Ssend(recv_1_array, i, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
        // i++;
    }

    // Finalize MPI
    MPI_Finalize();

    return 0;
}
