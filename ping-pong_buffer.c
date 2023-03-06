#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int rank, size, message;
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

    if (rank == 0)
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
    else
    {
        MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printf("Process %d received message %d from process %d\n", rank, message, status.MPI_SOURCE);

        buffer_attached_size = MPI_BSEND_OVERHEAD + sizeof(int);
        char *buffer_attached = (char *)malloc(buffer_attached_size);
        MPI_Buffer_attach(buffer_attached, buffer_attached_size);
        message = 84;
        MPI_Bsend(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        MPI_Buffer_detach(&buffer_attached, &buffer_attached_size);
        free(buffer_attached);
    }
    MPI_Buffer_detach(&message, &size);
    MPI_Finalize();
    return 0;
}
