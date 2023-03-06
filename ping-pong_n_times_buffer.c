#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int rank, size, message, i, iteration_limit;
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

    int buffer_attached_size;

    iteration_limit = 100;

    start_time = MPI_Wtime();

    if (rank == 0)
    {
        for (i = 0; i < iteration_limit; i++)
        {
            buffer_attached_size = MPI_BSEND_OVERHEAD + sizeof(int);
            char *buffer_attached = (char *)malloc(buffer_attached_size);
            MPI_Buffer_attach(buffer_attached, buffer_attached_size);

            // Buffer
            message = 0;
            MPI_Bsend(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
            printf("Process %d received message %d from process %d\n", rank, message, status.MPI_SOURCE);

            MPI_Buffer_detach(&buffer_attached, &buffer_attached_size);
            free(buffer_attached);
        }
    }
    else
    {
        for (i = 0; i < iteration_limit; i++)
        {
            MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            printf("Process %d received message %d from process %d\n", rank, message, status.MPI_SOURCE);

            buffer_attached_size = MPI_BSEND_OVERHEAD + sizeof(int);
            char *buffer_attached = (char *)malloc(buffer_attached_size);
            MPI_Buffer_attach(buffer_attached, buffer_attached_size);
            message = 1;
            MPI_Bsend(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

            MPI_Buffer_detach(&buffer_attached, &buffer_attached_size);
            free(buffer_attached);
        }
    }
    end_time = MPI_Wtime();
    total_time = end_time - start_time;
    printf("Total time: %f seconds\n", total_time);
    MPI_Buffer_detach(&message, &size);
    MPI_Finalize();
    return 0;
}
