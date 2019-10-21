##
## EPITECH PROJECT, 2019
## makefile
## File description:
## webcam
##

CXX = c++ `Magick++-config --cxxflags --cppflags`

CXXFLAGS = -lv4l2 -std=c++0x -Wall -Wextra -pedantic -Iinclude -Wno-literal-suffix -Wunused -Wno-missing-field-initializers -Wno-vla

SRCS =  src/main.cpp src/webcam_capture.cpp src/init.cpp HOG/HOG.cpp HOG/Affine.cpp HOG/Histo.cpp HOG/Image.cpp

OBJS = ${SRCS:.cpp=.o}

HEADERS = webcam.h functions.h HOG/Affine.h HOG/Image.h HOG/HOG_Parameters.h HOG/Histo.h

DOC = doxygen

DOC_FILE = docs/config_html

MAIN = capture

purple = /bin/echo -e "\x1b[35m**$1\x1b[0m"

green = /bin/echo -e "\x1b[32mSuccess !$1\x1b[0m"

WARN_STRING = IF BUILD FAIL because of include <Magick++.h>\nTRY: export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig

all: ${MAIN}


${MAIN}:
		@$(call purple, $(WARN_STRING))
		${CXX} ${CXXFLAGS} -O2 -o ${MAIN} ${SRCS} `Magick++-config --ldflags --libs`
		@$(call green)

clean:
		@${RM} ${PROGS} ${OBJS} *.o *~.

html:
		$(DOC) $(DOC_FILE)


fclean:	clean
	@$(RM) res/w*
	@$(RM) $(MAIN)

re: fclean all
