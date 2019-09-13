#!/bin/bash
make -f Makefile
./exec_cpp
printf "converting /img/*.jpeg in /res/*.pgm\n"
mogrify -format pgm -compress none img/*.jpeg
cd HOG/
make -f Makefile
printf "converting /img/*.pgm to /res/*_histo.pgm\n"
./exec_cpp
make clean
cd ..
make fclean
