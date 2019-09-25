##
## EPITECH PROJECT, 2019
## makefile
## File description:
## webcam
##

CXX = c++ `Magick++-config --cxxflags --cppflags`

CXXFLAGS = -lv4l2 -std=c++0x -Wall -Wextra -pedantic -Iinclude -Wno-literal-suffix -Wunused -Wno-missing-field-initializers -g3

SRCS =  src/main.cpp src/webcam_capture.cpp src/init.cpp HOG/HOG.cpp HOG/Affine.cpp HOG/Histo.cpp HOG/Image.cpp

OBJS = ${SRCS:.cpp=.o}

HEADERS = webcam.h functions.h HOG/Affine.h HOG/Image.h HOG/HOG_Parameters.h HOG/Histo.h

DOC = doxygen

DOC_FILE = docs/config_html

MAIN = exec_cpp

all: ${MAIN}

${MAIN}:
		${CXX} ${CXXFLAGS} -O2 -o ${MAIN} ${SRCS} `Magick++-config --ldflags --libs`

clean:
		${RM} ${PROGS} ${OBJS} *.o *~.

html:
		$(DOC) $(DOC_FILE)


fclean:	clean
	rm img/*.pgm
	rm exec_cpp

re: fclean all
