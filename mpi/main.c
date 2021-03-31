#include <stdio.h>
#include <mpi.h>

int find_index_of_max(int* ar, int size) {
    int max = 0;
    int max_index = 0;
    for (int i = 0; i < size; ++i) {
        if (ar[i] > max) {
            max = ar[i];
            max_index = i;
        }
    }
    return max_index;
}

void shift_up(int *ar, int size, int shift_times) {
    for (int j = 0; j < shift_times; ++j) {
        int temp = ar[size - 1];
        for (int i = size - 1; i != 0; --i) {
            ar[i] = ar[i - 1];
        }
        ar[0] = temp;
    }
}

int main(int argc, char** argv)
{
    int size_vector = 4;
    int vector[4][4] = {
        {1,2,5,4},
        {7,4,6,2},
        {7,3,5,9},
        {2,5,1,2}
    };
    
    int recvcounts[4] = {0,0,0,0};
    int disp[4];
    
    for (int i = 0, k = 0; i < 4; ++i, ++k) {
        if (k == 4) 
            k = 0;
        disp[i] = k * 4;
        recvcounts[i] += 4;
    }
    
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    
    int recvbuf[4];
    // printf("%ld %ld\n", sizeof(vector), sizeof(recvbuf));
    MPI_Scatterv(&vector[0][0], recvcounts, disp, MPI_INT, recvbuf, 4, MPI_INT, 0, MPI_COMM_WORLD);
    
    /*    for (int i = 0; i < 4; ++i) {
        recvbuf[i] += 1;
    }*/
    
    int index_of_max = find_index_of_max(recvbuf, size_vector);
    int range = size_vector + rank - index_of_max;
    shift_up(recvbuf, size_vector, range);
    
    // result
    int result[4][4];
    MPI_Gatherv(recvbuf, 4, MPI_INT, &result[0][0], recvcounts, disp, MPI_INT, 0, MPI_COMM_WORLD);
    
    if(rank == 0) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                printf("[%d][%d]: %d\n", i, j, result[i][j]);
            }
        }
    }

    MPI_Finalize();
    return 0;
}
