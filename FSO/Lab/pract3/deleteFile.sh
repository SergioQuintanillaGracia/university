#!/bin/bash

for i in $(ls toremove/)
do
    echo "toremove/$i"
    if test -d "toremove/$i"
    then
        echo "File is a directory and won't be removed"
    elif test -f "toremove/$i"
    then
        echo "Removing toremove/$i"
        rm "toremove/$i"
    else
        echo "The file toremove/$i doesn't exist"
    fi
done
