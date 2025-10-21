#!/bin/bash
echo Arguments:
for i in $@
do
    echo $i
done

echo -e "\nDoing backups:"
for file in *
do
    # cp $file $file.bak
    echo \[$file\] backed up \(actually, no\)
done

echo -e "\nListing files:"
for i in $(ls)
do
    echo File: $i
done
