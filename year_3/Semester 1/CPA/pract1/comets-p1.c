/* astro.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#define BINS 10
#define VMAX_KMS 100.0
#define G_CONST 6.67430e-11
#define M_SUN_CONST 1.98847e30
#define AU_CONST 1.495978707e11
#define GRAZING_AU_CONST 0.269
#define M_COMET_CONST 1.0e12
#define FIXED_SEED 0

typedef struct {
    int id;
    double r_AU;
    double v_kms;
    int step;
} Grazing;

int main(int argc, char **argv)
{
    int N, STEPS;
    int i, step, bin;
    double dt;
    double rx, ry, r2, r, inv_r3, ax, ay;
    double v_kms;
    double r_AU_i, theta, v, v2;
    unsigned int seed;
    double *x, *y, *vx, *vy;
    long long hist[BINS];
    Grazing *grazing;
    int grazing_count;
    double total_kinetic_energy;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <N_comets> <steps> [dt_s=20000] [seed=42]\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    STEPS = atoi(argv[2]);
    dt = (argc > 3) ? atof(argv[3]) : 20000.0;
    seed = (argc > 4) ? atof(argv[4]) : FIXED_SEED;

    /* Dynamic memory allocation */
    x  = (double*) malloc(sizeof(double) * (size_t)N);
    y  = (double*) malloc(sizeof(double) * (size_t)N);
    vx = (double*) malloc(sizeof(double) * (size_t)N);
    vy = (double*) malloc(sizeof(double) * (size_t)N);
    grazing = (Grazing*) malloc(sizeof(Grazing) * (size_t)N);

    if (!x || !y || !vx || !vy || !grazing) {
        fprintf(stderr, "Could no allocate memory\n");
        free(x); free(y); free(vx); free(vy); free(grazing);
        return 2;
    }

    grazing_count = 0;
    total_kinetic_energy = 0.0;
    memset(hist, 0, sizeof(hist));

    /* Reproducible initialization of initial conditions */
    srand(seed);
    for (i = 0; i < N; ++i) {
        r_AU_i = 0.3 + (5.0 - 0.3) * (rand() / (double)RAND_MAX); /* [0.3, 5] AU */
        theta  = 2.0 * M_PI * (rand() / (double)RAND_MAX);
        r = r_AU_i * AU_CONST;
        x[i] = r * cos(theta);
        y[i] = r * sin(theta);
        v = sqrt(G_CONST * M_SUN_CONST / r);
        vx[i] = v * -sin(theta);
        vy[i] = v *  cos(theta);
        /* small random eccentricity */
        {
            double eps = 0.1 * (rand() / (double)RAND_MAX) - 0.05;
            vx[i] *= (1.0 + eps);
            vy[i] *= (1.0 - eps);
        }
    }

    /**************************************************************
     * Start of the code to be parallelized
     **************************************************************/
    for (step = 0; step < STEPS; ++step) {
        #pragma omp parallel for private(rx, ry, r2, r, inv_r3, ax, ay, v2, v_kms, bin) reduction(+:total_kinetic_energy)
        for (i = 0; i < N; ++i) {
            rx = x[i]; 
			ry = y[i];
            r2 = rx*rx + ry*ry;
            r  = sqrt(r2);
            inv_r3 = 1.0 / (r2 * r + 1e-30);
            ax = -G_CONST * M_SUN_CONST * rx * inv_r3;
            ay = -G_CONST * M_SUN_CONST * ry * inv_r3;

            vx[i] += ax * dt;
            vy[i] += ay * dt;
            x[i]  += vx[i] * dt;
            y[i]  += vy[i] * dt;

            v2 = vx[i]*vx[i] + vy[i]*vy[i];
            v_kms = sqrt(v2) / 1000.0;

            total_kinetic_energy += 0.5 * M_COMET_CONST * v2;

            bin = (int)((v_kms / VMAX_KMS) * BINS);
            if (bin < 0) bin = 0;
            if (bin >= BINS) bin = BINS - 1;

            #pragma omp atomic
            hist[bin]++;

            if ((r/AU_CONST) < GRAZING_AU_CONST && grazing_count < N) {
                #pragma omp critical
                if (grazing_count < N) {
                    grazing[grazing_count].id = i;
                    grazing[grazing_count].r_AU = r / AU_CONST;
                    grazing[grazing_count].v_kms = v_kms;
                    grazing[grazing_count].step = step;
                    grazing_count++;
                }
            }
        }
    }
    /**************************************************************
     * End of the code to be parallelized
     **************************************************************/

    /* Output summary */
    printf("N=%d, steps=%d, dt=%.0f s, seed=%u\n", N, STEPS, dt, seed);
    printf("Total kinetic energy (J): %.6e\n", total_kinetic_energy);
    printf("List of grazing events: %d\n", grazing_count);
    printf("Histogram (bins=%d):\n", BINS);
    for (bin = 0; bin < BINS; ++bin) {
        double lo = (VMAX_KMS / BINS) * bin;
        double hi = (VMAX_KMS / BINS) * (bin + 1);
        printf("  [%.1f, %.1f): %lld\n", lo, hi, hist[bin]);
    }

    free(grazing); free(x); free(y); free(vx); free(vy);
    return 0;
}
