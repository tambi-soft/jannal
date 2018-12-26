#!/usr/bin/bash

qmake-qt5 -o Makefile marbox.pro
make

if [ $? -eq 0 ]
then
    ./debug/marbox
fi
