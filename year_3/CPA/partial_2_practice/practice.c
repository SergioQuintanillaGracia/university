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
        readmat(A, B, C, N);  // T1
        MPI_Send(B, N * N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);

        pot(A, 2, N);  // T2
        pot(C, 2, N);  // T4

        nm = sumnor(A, C, N);  // T5
        MPI_Send(&nm, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);

    } else {
        MPI_Recv(B, N * N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        pot(B, 5, N);  // T3

        MPI_Recv(&nm, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        nm = norsc(B, nm, N);  // T6
        printf("%f\n", nm);    // T7
    }

    MPI_Finalize();

    return 0;
}

void fun1(double A[N][N], double x[], double y[]) {
    int i, j, id, np, nb;
    double Aloc[N][N];
    double xloc[N];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    nb = N / np;

    MPI_Scatter(x, nb, MPI_DOUBLE, xloc, nb, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(y, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (i = 0; i < nb; i++) {
        for (j = 0; j < N; j++)
            Aloc[i][j] = xloc[i] * y[j];
    }

    MPI_Gather(Aloc, nb * N, MPI_DOUBLE, A, nb * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}

void send_perimeter_matrix(double A[M][N], int myid, int np, int root) {
    MPI_Datatype column;
    MPI_Datatype rows_first_last;
    MPI_Type_vector(M, 1, N, MPI_DOUBLE, &column);
    MPI_Type_vector(2, N - 2, N * (M - 1), MPI_DOUBLE, &rows_first_last);
    MPI_Type_commit(&column);
    MPI_Type_commit(&rows_first_last);

    if (myid == root) {
        for (int proc = 0; proc < np; proc++) {
            if (proc != root) {
                MPI_Send(A, 1, column, proc, 0, MPI_COMM_WORLD);
                MPI_Send(&A[0][N - 1], 1, column, proc, 0 MPI_COMM_WORLD);
                MPI_Send(&A[0][1], 1, rows_first_last, proc, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        MPI_Recv(A, 1, column, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&A[0][N - 1], 1, column, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&A[0][1], 1, rows_first_last, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    MPI_Type_free(&column);
    MPI_Type_free(&rows_first_last);
}

void func(double A[N][N], double w[N]) {
    double x[N], y[N], v[N], alpha;
    int id;

    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    if (id == 0) {
        T1(A, x);
        MPI_Send(x, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);

        T2(A, x, y);
        MPI_Sendrecv(y, N, MPI_DOUBLE, 1, 0, v, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        T5(A, y, v, &alpha);
        MPI_Send(&alpha, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);

    } else {
        MPI_Recv(x, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        T3(x, v);
        MPI_Sendrecv(v, N, MPI_DOUBLE, 0, 0, y, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        T4(y, v, w);

        MPI_Recv(&alpha, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        T6(alpha, w);
    }
}

#define n 1000
double function(double A[n][n], double B[n][n], double C[n][n]) {
    double a;
    int id;

    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    if (id == 0) {
        f2(B); /* T2 Cost = n^2 */
        f3(C); /* T3 Cost = n^2 */
        MPI_Sendrecv(B, n * n, MPI_DOUBLE, 1, 0, A, n * n, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        f5(A, B, C); /* T5 Cost = 2*n^2 */
        MPI_Recv(&a, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        a *= f7(C); /* T7 Cost = n */

    } else {
        f1(A); /* T1 Cost = n^2 */
        f4(A); /* T4 Cost = n^2 */
        MPI_Sendrecv(A, n * n, MPI_DOUBLE, 0, 0, B, n * n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        a = f6(A, B); /* T6 Cost = n^2 */
        MPI_Send(&a, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    return a;
}

#include <mpi.h>
#include <stdio.h>
#define M 2000
#define N 1000

int main(int argc, char* argv[]) {
    double A[M][N], x[M], y[N];
    double Aloc[M][N], xloc[M];
    int i, j, id, np;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    if (id == 0) {
        read(A, x, y);
    }

    int nb = M / np;

    MPI_Scatter(A, nb * N, MPI_DOUBLE, Aloc, nb * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(x, nb, MPI_DOUBLE, xloc, nb, MPI_DOUBLE, 0, MPI_COMM_WORLD)
        MPI_Bcast(y, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (i = 0; i < nb; i++)
        for (j = 0; j < N; j++)
            Aloc[i][j] += xloc[i] * y[j];

    MPI_Gather(Aloc, nb * N, MPI_DOUBLE, A, nb * N, 0, MPI_COMM_WORLD);

    if (id == 0) {
        write(A);
    }

    MPI_Finalize();

    return 0;
}

float sum_dif_par(float a, float A[M][N], float b, float B[M][N], int id) {
    int i, j, rank, np;
    float sum;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    MPI_Datatype cyclic_cols;
    MPI_Type_vector(M * N / np, 1, np, MPI_FLOAT, &cyclic_cols);
    MPI_Type_commit(&cyclic_cols);

    MPI_Bcast(&a, 1, MPI_FLOAT, id, MPI_COMM_WORLD);
    MPI_Bcast(&b, 1, MPI_FLOAT, id, MPI_COMM_WORLD);

    if (rank == id) {
        for (int proc = 0; proc < np; proc++) {
            if (proc != id) {
                MPI_Send(&A[0][proc], 1, cyclic_cols, proc, 0, MPI_COMM_WORLD);
                MPI_Send(&B[0][proc], 1, cyclic_cols, proc, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        MPI_Recv(&A[0][rank], 1, cyclic_cols, id, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&B[0][rank], 1, cyclic_cols, id, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    float sumloc = 0;

    for (j = rank; j < N; j += np) {
        for (i = 0; i < M; i++) {
            sumloc += fabs(a * A[i][j] - b * B[i][j]);
        }
    }

    MPI_Reduce(&sumloc, &sum, 1, MPI_FLOAT, MPI_SUM, id, MPI_COMM_WORLD);

    MPI_Type_free(cyclic_cols);

    return sum;
}

double func(double v[N]) {
    double x[N], y[N], z[N], r, s;
    int id;

    MPI_Comm_rank(MPI_COMM_WORLD, &id);

    if (id == 0) {
        T1(x, v);
        MPI_Send(x, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
        MPI_Send(x, N, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD);

        T4(v, x);

        MPI_Recv(y, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(z, N, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        s = T7(v, y, z);

    } else if (id == 1) {
        MPI_Recv(x, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        T2(y, x);

        MPI_Recv(&r, 1, MPI_DOUBLE, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        T5(y, r);
        MPI_Send(y, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

    } else {
        MPI_Recv(x, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        r = T3(z, x);
        MPI_Send(&r, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);

        T6(z);
        MPI_Send(z, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    return s;
}

void normalize(float v[N]) {
    int i, np, id, nb;
    float max = -FLT_MAX;
    float max_loc = max;
    float vloc[N];

    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    nb = N / np;

    MPI_Scatter(v, mb, MPI_FLOAT, vloc, mb, MPI_FLOAT, 0, MPI_COMM_WORLD);

    for (i = 0; i < mb; i++)
        if (vloc[i] > maxloc)
            maxloc = vloc[i];

    MPI_Allreduce(&maxloc, &max, 1, MPI_FLOAT, MPI_MAX, MPI_COMM_WORLD);

    for (i = 0; i < mb; i++)
        vloc[i] /= max;

    MPI_Gather(vloc, mb, MPI_FLOAT, v, mb, MPI_FLOAT, 0, MPI_COMM_WORLD);
}

void commun_matrix(int A[n][n], int B[n][n], int rank) {
    MPI_Datatype alt_columns;
    MPI_Type_vector(n * n / 2, 1, 2, MPI_INT, &alt_columns);
    MPI_Type_commit(&alt_columns);

    if (rank == 0) {
        MPI_Send(A, 1, alt_columns, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&A[0][1], 1, alt_columns, 1, 0, MPI_COMM_WORLD);
    } else if (rank == 1) {
        MPI_Recv(&B[0][1], 1, alt_columns, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(B, 1, alt_columns, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    MPI_Type_free(&alt_columns);
}

void comunicate(double x[], int sizes[], double xloc[], int nloc, int root) {
    int id, np;

    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    if (id == 0) {
        xindex = 0;

        for (int proc = 0; proc < np; proc++) {
            if (id == proc) {
                for (i = 0; i < nloc; i++) {
                    xloc[i] = x[xindex++];
                }
            } else {
                MPI_Send(&x[xindex], sizes[proc], MPI_DOUBLE, proc, 0, MPI_COMM_WORLD);
                xindex += sizes[proc];
            }
        }
    } else {
        MPI_Recv(xloc, nloc, MPI_DOUBLE, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}

void product_elements(double a, double A[M][N], double B[M][N], double C[M][N]) {
    int i, j, id, np, mb;
    double Aloc[M][N], double Bloc[M][N], double Cloc[M][N];

    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    mb = M / np;

    MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(A, mb * N, MPI_DOUBLE, Aloc, mb * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, mb * N, MPI_DOUBLE, Bloc, mb * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (i = 0; i < nb; i++) {
        for (j = 0; j < N; j++) {
            Cloc[i][j] = a * Aloc[i][j] * Bloc[i][j];
        }
    }

    MPI_Allgather(Cloc, mb * N, MPI_DOUBLE, C, mb * N, MPI_DOUBLE, MPI_COMM_WORLD);
}

double norma(double A[N * 3][N * 3]) {
    int i, j, id;
    double norm = 0.0, normloc = 0.0;
    double Aloc[N][N];

    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Datatype submat;
    MPI_Type_vector(N, N, 3 * N, MPI_DOUBLE, &submat);
    MPI_Type_commit(&submat);

    if (id == 0) {
            MPI_Sendrecv(A, 1, submat, 0, 0, Aloc, N * N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int proc = 1; proc < 9; proc++) {
            MPI_Send(&A[proc / 3 * N][proc % 3 * N], 1, submat, proc, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(Aloc, N * N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            normloc += Aloc[i][j] * Aloc[i][j];

    MPI_Reduce(&normloc, &norm, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (id == 0) {
        norm = sqrt(norm);
    }

    MPI_Type_free(&submat);

    return norm;
}