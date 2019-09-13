BIN_DIR =	./
OBJ_DIR =	obj/
SRC_DIR =	./
INC_DIR =       ./
RES_DIR =       ../res/

#files
BIN =	${BIN_DIR}exec_cpp
SRC =	${wildcard ${SRC_DIR}*.cpp}
OBJ =   ${SRC:${SRC_DIR}%.cpp=${OBJ_DIR}%.o}
RES =   ${wildcard ${RES_DIR}*_histo.pgm}


#flags
CFLAGS =	-g -W -Wall
IFLAGS =	-I${INC_DIR}

#Compiler
CC =		g++

#rules
all:		${BIN}

${BIN}:		${OBJ}
		${CC} $^ -o ${BIN}

${OBJ_DIR}%.o:	${SRC_DIR}%.cpp		
		${CC} ${IFLAGS} ${CFLAGS} -c $< -o $@

.PHONY:
	clean
	clean-all

clean: 
	rm -f $(OBJ)
	rm -f ${BIN}

clean-all: clean
	rm -f *.*~
	rm -f ${RES}
