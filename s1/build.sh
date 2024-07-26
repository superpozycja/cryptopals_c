#!/bin/sh

for filename in *.c; do
    echo "compiling $filename"
    gcc -lba -o "bin/$(basename $filename .c)" $filename
done
