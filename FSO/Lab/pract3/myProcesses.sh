#!/bin/bash

process=$(ps u | grep $USER | wc -l)

if test $process -gt 1
then
    echo "More than 1 user processes active"
else
    echo "Equal or less than 1 user process active"
fi
