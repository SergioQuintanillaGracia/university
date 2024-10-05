#!/bin/bash

echo PID $'\t' PPID $'\t' STATE $'\t' CMD

for pid in $(./listAllPid.sh)
do
    ./infProcessMod.sh "$pid"
done
