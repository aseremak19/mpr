#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int rank, size, i, j;
    int send_size, recv_size;
    int *send_array;
    int *recv_array;
    double start_time, end_time, total_time, avg_time;

    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get the number of processes and the rank of the current process
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size != 2)
    {
        printf("This program should be run with exactly 2 processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Perform ping-pong communication between the two processes with varying array sizes
    for (send_size = 10; send_size <= 1000; send_size += 10)
    {
        recv_size = send_size;
        send_array = (int *)malloc(send_size * sizeof(int));
        recv_array = (int *)malloc(recv_size * sizeof(int));

        total_time = 0.0;
        for (i = 0; i < 100; i++)
        {
            if (rank == 0)
            {
                // Initialize the send array
                for (j = 0; j < send_size; j++)
                {
                    send_array[j] = j + rank * send_size;
                }

                start_time = MPI_Wtime();
                MPI_Ssend(send_array, send_size, MPI_INT, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(recv_array, recv_size, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                end_time = MPI_Wtime();
                total_time += end_time - start_time;
            }
            else
            {
                MPI_Recv(recv_array, recv_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Ssend(send_array, send_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        }

        // Compute the average time for 100 iterations
        avg_time = total_time / 100.0;

        // Print the average time and the array size
        if (rank == 0)
        {
            printf("Average time for array size %d: %.15f seconds\n", send_size, avg_time);
        }

        // Free the memory
        free(send_array);
        free(recv_array);
    }

    // Finalize MPI
    MPI_Finalize();

    return 0;
}
