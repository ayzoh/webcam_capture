#!/bin/bash
if [ "${BASH_SOURCE[0]}" -ef "$0" ]
then
    echo "Usage: source ./shell.sh"
    exit
fi
printf "Compiling webcam_capture files...\n"
make --quiet -f Makefile
./exec_cpp
result="$?"
if [ "$result" -ne 0 ]; then
    echo "Webcam_capture failed"
    return
fi
printf "Converting /img/*.jpeg in /res/*.pgm...\n"
mogrify -format pgm -compress none img/*.jpeg
cd HOG/
printf "Compiling HOG algrorithm files...\n"
make --quiet -f Makefile
printf "Converting /img/*.pgm to /res/*_histo.pgm...\n"
./exec_cpp
result="$?"
if [ "$result" -ne 0 ]; then
    echo "HOG conversion failed"
    return
fi
printf "Erasing all useless files...\n"
make --quiet clean
cd ..
make --quiet fclean
printf "Done !\n"
