#include <mpi.h>
#include <stdio.h>
#define M 2000
#define N 1000

int main(int argc, char* argv[]) {
    double A[M][N], max, min, B[M][N], temp;
    int i, j, id, np, nb;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    nb = M / np;

    if (id == 0)
        read(A);

    MPI_Scatter(A, nb * N, MPI_DOUBLE, B, nb * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    max = -1e6;
    for (i = 0; i < nb; i++)
        for (j = 0; j < N; j++)
            if (B[i][j] > max) max = B[i][j];

    MPI_Allreduce(&max, &temp, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    max = temp;

    min = 1e6;
    for (i = 0; i < nb; i++) {
        for (j = 0; j < N; j++) {
            B[i][j] /= max;
            if (B[i][j] < min) min = B[i][j];
        }
    }

    MPI_Gather(B, nb * N, MPI_DOUBLE, A, nb * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Reduce(&min, &temp, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);

    if (id == 0) {
        write(A);
        printf("The smallest value is %g\n", min);
    }

    MPI_Finalize();

    return 0;
}

double infNormPar(double A[N][N], double Aloc[N][N]) {
    int i, j, id, np;
    double sum[N], sumloc[N], nrm = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    int colsPerProc = N / np;

    MPI_Datatype submat;

    MPI_Type_vector(N, colsPerProc, N, MPI_DOUBLE, &submat);
    MPI_Type_commit(&submat);

    if (id == 0) {
        for (int proc = 1; proc < np; proc++) {
            MPI_Send(&A[0][proc * colsPerProc], 1, submat, proc, 0, MPI_COMM_WORLD);
        }
        MPI_Sendrecv(A, 1, submat, 0 0, Aloc, 1, submat, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        MPI_Recv(Aloc, 1, submat, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (i = 0; i < N; i++) {
        sumloc[i] = 0.0;
        for (j = 0; j < colsPerProc; j++)
            sumloc[i] += fabs(A[i][j]);
    }

    MPI_Reduce(sumloc, sum, N, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (id == 0) {
        for (i = 0; i < N; i++) {
            if (sum[i] > nrm) nrm = sum[i];
        }
    }

    MPI_Type_free(submat);

    return nrm;
}

int main(int argc, char* argv[]) {
    double A[N][N], B[N][N], C[N][N];
    double nm;
    int id, np;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    if (id == 0) {
        readmat(A, B, C, N);   // T1
        MPI_Send(B, N * N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);

        pot(A, 2, N);          // T2
        pot(C, 2, N);          // T4
        
        nm = sumnor(A, C, N);  // T5
        MPI_Send(&nm, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);

    } else {
        MPI_Recv(B, N * N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        pot(B, 5, N);          // T3

        MPI_Recv(&nm, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        nm = norsc(B, nm, N);  // T6
        printf("%f\n", nm);    // T7
    }

    MPI_Finalize();

    return 0;
}