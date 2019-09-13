##
## EPITECH PROJECT, 2019
## makefile
## File description:
## webcam
##

CXX = g++

CXXFLAGS = -lv4l2 -ljpeg -std=c++0x -Wall -Wextra -pedantic -g -Iinclude -Wno-literal-suffix -Wunused -Wno-missing-field-initializers

SRCS =  src/main.cpp src/webcam_capture.cpp src/init.cpp

OBJS = ${SRCS:.cpp=.o}

HEADERS = webcam.h functions.h

MAIN = exec_cpp

all: ${MAIN}
		@echo   Simple compilter named exec_cpp has been compiled

${MAIN}: ${OBJS}
		${CXX} ${CXXFLAGS} ${OBJS} -o ${MAIN}

.cpp.o:
		${CXX} ${CXXFLAGS} -c $< -o $@

clean:
		${RM} ${PROGS} ${OBJS} *.o *~.

fclean:	clean
	rm img/*.pgm
	rm img/*.jpeg
	rm exec_cpp

re: fclean all
