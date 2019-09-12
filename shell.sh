#!/bin/bash
make -f Makefile
./exec_cpp
printf "converting /img/*.jpeg in /res/*.pgm\n"
mogrify -format pgm -compress none -path /home/evenot/MERIM/3DCamera/Webcam/res /home/evenot/MERIM/3DCamera/Webcam/img/*.jpeg
make -C ./../HOG_project/cpp/
cd ../HOG_project/cpp/
printf "converting /img/*.pgm to /res/*_histo.pgm\n"
./exec_cpp
make clean
cd ../../Webcam/
make fclean
rm res/*
