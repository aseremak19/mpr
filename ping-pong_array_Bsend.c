#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 100000

int main(int argc, char **argv)
{
    int rank, size, i, j;
    int send_size, recv_size;
    int *send_0_array;
    int *recv_1_array;
    int *recv_0_array;
    double start_time, end_time, total_time, avg_time;

    char hostname[MPI_MAX_PROCESSOR_NAME];
    MPI_Get_processor_name(hostname, &len);

    int iteration_limit, iteration_per_;

    iteration_limit = 1000 * 3;
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

    // creating CSV file
    FILE *file = NULL;
    if (rank == 0)
    {
        file = fopen("output_Bsend_2nodes.csv", "w");
        if (file == NULL)
        {
            printf("Error opening output file.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        // Write the header row to the output file
        fprintf(file, "Array Size,Average Time\n");
    }

    // Allocate the buffer for MPI_Buffered communication
    int buffer_attached_size;

    // Perform ping-pong communication between the two processes with varying array sizes
    for (send_size = 1; send_size <= iteration_limit; send_size += 1)
    {
        recv_size = send_size;
        send_0_array = (int *)malloc(send_size * sizeof(int));
        recv_1_array = (int *)malloc(recv_size * sizeof(int));
        recv_0_array = (int *)malloc(recv_size * sizeof(int));

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
                    send_0_array[j] = j + rank * send_size;
                }

                start_time = MPI_Wtime();
                MPI_Bsend(send_0_array, send_size, MPI_INT, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(recv_0_array, recv_size, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                end_time = MPI_Wtime();
                total_time += (end_time - start_time) / 2;
            }
            else
            {
                MPI_Recv(recv_1_array, recv_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Bsend(recv_1_array, recv_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        }

        // Compute the average time for 100 iterations
        avg_time = total_time / (double)iteration_per_;

        // Print the average time and the array size
        if (rank == 0)
        {
            printf("Average time for array size %d: %.15f seconds\n", send_size, avg_time);
            fprintf(file, "%d,%.15f\n", send_size, avg_time);
        }

        // Free the memory
        free(send_0_array);
        free(recv_1_array);
        free(recv_0_array);
        MPI_Buffer_detach(&buffer_attached, &buffer_attached_size);
        free(buffer_attached);
    }

    // Close the output file
    if (rank == 0)
    {
        fclose(file);
    }
    else
    {
    }

    // Finalize MPI
    MPI_Buffer_detach(&recv_1_array, &size);
    MPI_Buffer_detach(&send_0_array, &size);

    printf("Rank: %d; executed on host: ", rank);
    for (i = 0; i < MPI_MAX_PROCESSOR_NAME; i++)
    {
        printf("%s", hostname[i]);
    }
    MPI_Finalize();

    return 0;
}
