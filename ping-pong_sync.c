#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    int iterations_limit = 10;

    i = 0;

    int message[iterations_limit + 1];
    int j;

    if (rank == 0)
    {
        size_t size_message = sizeof(message) / sizeof(message[0]);
        printf("Size of: %d\n ", size_message);
        printf("---- Array message:\n ");
        for (j = 0; j < size_message; j++)
        {
            message[j] = getRandom(1, 15);
            printf(" %d ", message[j]);
        }
        printf("\n \n");

        int *message_aloc;
        message_aloc = (int *)malloc(size_message + 1);

        printf("---- Array message_aloc:\n ");
        for (j = 0; j < size_message; j++)
        {
            message_aloc[j] = message[j];
            printf(" %d ", message_aloc[j]);
        }
    }

    // start_time = MPI_Wtime();

    // message = 0;

    // MPI_Barrier(MPI_COMM_WORLD);
    while (i < iterations_limit + 1)
    {
        /*int message[i + 1];
        int j;

        size_t size = sizeof(message) / sizeof(message[0]);
        printf("Size of: %d\n ", size);
        printf("---- Array:\n ");
        for (j = 0; j < size; j++)
        {
            message[j] = getRandom(1, 15);
            printf(" %d ", message[j]);
        }
        printf("\n \n");
*/
        if (rank == 0)
        {
            // message = 0;

            // int j;
            //  printf("Iteraton: %d;  ", i);
            /*for (j = 0; j < i + 1; j++)
            {
                message[j] = 1;
                // printf("%d", message[j]);
            }*/

            // message++;

            MPI_Barrier(MPI_COMM_WORLD);
            start_time = MPI_Wtime();
            MPI_Send(&message, i, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&message, i, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
            printf("Process %d received message %d from process %d at iteration: %d\n", rank, *message, status.MPI_SOURCE, i);
            //   i++;
            end_time = MPI_Wtime();
            total_time = (end_time - start_time) / 2;
            printf("Rank: %d; Total time: %.15f seconds\n", rank, total_time);
        }
        else
        {
            MPI_Barrier(MPI_COMM_WORLD);
            MPI_Recv(&message, i, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            // printf("Process %d received message %d from process %d at iteration: %d\n", rank, *message, status.MPI_SOURCE, i);
            //  message = 1;
            MPI_Send(&message, i, MPI_INT, 0, 0, MPI_COMM_WORLD);
            // i++;
        }
        i++;
    }
    // end_time = MPI_Wtime();
    // total_time = end_time - start_time;
    // printf("Rank: %d; Total time: %f seconds\n", rank, total_time);
    MPI_Finalize();
    free(message_aloc);
    return 0;
}
