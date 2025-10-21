#!/bin/bash

for pid in $(ls /proc/ | grep '[0-9]')
do
    echo $pid
done
