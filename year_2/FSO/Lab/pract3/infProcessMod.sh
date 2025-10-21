#!/bin/bash

pid=$1
ppid=$(awk '/PPid/ {print $2}' /proc/$1/status)
status=$(awk '/State/ {print $2}' /proc/$1/status)
cmd=$(tr -d '\0' < /proc/$1/cmdline)

echo $pid $'\t' $ppid $'\t' $status $'\t' $cmd
