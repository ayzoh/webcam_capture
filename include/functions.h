/*
** EPITECH PROJECT, 2018
** my.h
** File description:
** lib
*/

#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

int capture(char *);
int init_format(int fd);
int init_all(int fd, struct buffer *buffers);
int xioctl(int fd, long unsigned int request, void* argp);
int start_hog(char *filename);

#endif