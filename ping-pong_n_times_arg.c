#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

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
    int iterations_limit = 100;

    i = 0;

    // start_time = MPI_Wtime();

    // message = 0;

    MPI_Barrier(MPI_COMM_WORLD);
    while (i < iterations_limit + 1)
    {
        int message[i + 1];
        if (rank == 0)
        {
            // message = 0;

            int j;
            printf("Iteraton: %d;  ", i);
            for (j = 0; j < i + 1; j++)
                ;
            {
                message[j] = 1;
                printf("%d", message[j]);
            }

            // message++;
            start_time = MPI_Wtime();
            MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
            printf("Process %d received message %d from process %d at iteration: %d\n", rank, *message, status.MPI_SOURCE, i);
            //  i++;
            end_time = MPI_Wtime();
            total_time = (end_time - start_time) / 2;
            printf("Rank: %d; Total time: %f seconds\n", rank, total_time);
        }
        else
        {
            MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            printf("Process %d received message %d from process %d at iteration: %d\n", rank, *message, status.MPI_SOURCE, i);
            // message = 1;
            MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            // i++;
        }
        i++;
    }
    // end_time = MPI_Wtime();
    // total_time = end_time - start_time;
    // printf("Rank: %d; Total time: %f seconds\n", rank, total_time);
    MPI_Finalize();
    return 0;
}
