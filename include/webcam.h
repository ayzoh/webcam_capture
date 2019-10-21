/*
** EPITECH PROJECT, 2018
** my.h
** File description:
** lib
*/

#ifndef _WEBCAM_H_
#define _WEBCAM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <string>
#include <fstream>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <linux/videodev2.h>
#include <libv4l2.h>
#include <signal.h>
#include <stdint.h>
#include <inttypes.h>
#include <iostream>
#include <signal.h>

#define CLEAR(x) memset (&(x), 0, sizeof (x))
#define UNUSED(x) (void)(x)

struct buffer {
        void *start;
        size_t length;
        unsigned int n_buffers;
};


#endif
