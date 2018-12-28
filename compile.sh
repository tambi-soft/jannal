#!/usr/bin/bash

qmake-qt5 -o Makefile jannal.pro
make

if [ $? -eq 0 ]
then
    ./debug/jannal
fi
