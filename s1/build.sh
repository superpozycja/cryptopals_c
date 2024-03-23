#!/bin/zsh

source ~/.zshrc

for filename in *.c; do
    echo "compiling $filename"
    cc -o "bin/$(basename $filename .c)" $filename
done
