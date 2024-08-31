#!/bin/sh

for filename in *.c; do
    echo "compiling $filename"
    gcc -lba -lcryptanalysis -g -o "bin/$(basename $filename .c)" $filename -lcrypto
done
