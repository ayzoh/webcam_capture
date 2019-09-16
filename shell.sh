#!/bin/bash
make -f Makefile
set -x
./exec_cpp
result="$?"
set +x
if [ "$result" -ne 0 ]; then
    echo "webcam_capture failed"
    return
fi
printf "converting /img/*.jpeg in /res/*.pgm\n"
mogrify -format pgm -compress none img/*.jpeg
cd HOG/
make -f Makefile
printf "converting /img/*.pgm to /res/*_histo.pgm\n"
set -x
./exec_cpp
result="$?"
set +x
if [ "$result" -ne 0 ]; then
    echo "HOG conversion failed"
    return
fi
make clean
cd ..
make fclean
