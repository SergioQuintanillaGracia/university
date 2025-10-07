#!/bin/bash
#SBATCH --nodes=1
#SBATCH --time=2:00
#SBATCH --partition=cpa

OMP_NUM_THREADS=16 ./imagenes
