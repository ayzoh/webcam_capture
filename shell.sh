#!/bin/bash
make -f Makefile
./exec_cpp
printf "converting /img/*.jpeg in /res/*.pgm\n"
mogrify -format pgm -compress none -path /home/evenot/MERIM/3DCamera/Webcam/res /home/evenot/MERIM/3DCamera/Webcam/img/*.jpeg
make fclean
