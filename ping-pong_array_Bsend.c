#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 100000

int main(int argc, char **argv)
{
    int rank, size, i, j;
    int send_size, recv_size;
    int *send_array;
    int *recv_array;
    double start_time, end_time, total_time, avg_time;

    int iteration_limit, iteration_per_;

    iteration_limit = 1000;
    iteration_per_ = 100;

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

    // Allocate the buffer for MPI_Buffered communication
    int buffer_attached_size;

    // Perform ping-pong communication between the two processes with varying array sizes
    for (send_size = 1; send_size <= iteration_limit; send_size += 1)
    {
        recv_size = send_size;
        send_array = (int *)malloc(send_size * sizeof(int));
        recv_array = (int *)malloc(recv_size * sizeof(int));

        buffer_attached_size = MPI_BSEND_OVERHEAD + sizeof(int) * send_size;
        char *buffer_attached = (char *)malloc(buffer_attached_size);
        MPI_Buffer_attach(buffer_attached, buffer_attached_size);

        total_time = 0.0;
        for (i = 0; i < iteration_per_; i++)
        {
            if (rank == 0)
            {
                // Initialize the send array
                for (j = 0; j < send_size; j++)
                {
                    send_array[j] = j + rank * send_size;
                }

                start_time = MPI_Wtime();
                MPI_Bsend(send_array, send_size, MPI_INT, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(recv_array, recv_size, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                end_time = MPI_Wtime();
                total_time += end_time - start_time;
            }
            else
            {
                MPI_Recv(recv_array, recv_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Bsend(send_array, send_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        }

        // Compute the average time for 100 iterations
        avg_time = total_time / (double)iteration_per_;

        // Print the average time and the array size
        if (rank == 0)
        {
            printf("Average time for array size %d: %.15f seconds\n", send_size, avg_time);
        }

        // Free the memory
        free(send_array);
        free(recv_array);
        MPI_Buffer_detach(&buffer_attached, &buffer_attached_size);
        free(buffer_attached);
    }

    // Finalize MPI
    MPI_Buffer_detach(&recv_array, &send_array, &size);
    MPI_Finalize();

    return 0;
}
