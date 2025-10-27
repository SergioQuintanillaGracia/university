# Accessing cluster computer:
ssh -Y -l [username]@alumno.upv.es kahan.dsic.upv.es


# Accessing disk W (Linux path):
smb://nasupv/alumnos/[initial_username_letter]/[username]
User: ALUMNO\[username]@upv.edu.es
Password: Poliformat password


# Compile a file (pract1):
gcc -fopenmp -Wall -o [file] [file].c -lm


# Job file:
#!/bin/bash
#SBATCH --nodes=1
#SBATCH --time=5:00
#SBATCH --partition=cpa
OMP_NUM_THREADS=3 ./pintegral 1


# Submitting a job:
sbatch jobopenmp.sh


# Getting status of job
squeue