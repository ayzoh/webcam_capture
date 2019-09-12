/*
** EPITECH PROJECT, 2019
** init
** File description:
** webcam
*/

#include "webcam.h"
#include "functions.h"

using namespace std;

const void *save = NULL;
static unsigned int fps = 30;

int xioctl(int fd, long unsigned int request, void* argp)
{
	int r;

	do r = v4l2_ioctl(fd, request, argp);
	while (-1 == r && EINTR == errno);

	return r;
}

int check_capabilities(int fd)
{
    v4l2_capability cap = {0};

    if (fd < 0) {
        perror("Failed to open device, OPEN");
        return (1);
    }
    if (xioctl(fd, VIDIOC_QUERYCAP, &cap) < 0) {
        perror("Failed to get device capabilities, VIDIOC_QUERYCAP");
        return (1);
    }
    return (0);
}

int init_fps(int fd)
{
    v4l2_streamparm frameint = {0};

    CLEAR(frameint);    
    frameint.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    frameint.parm.capture.timeperframe.numerator = 1;
    frameint.parm.capture.timeperframe.denominator = fps;
    if (-1 == xioctl(fd, VIDIOC_S_PARM, &frameint)) 
      fprintf(stderr,"Unable to set frame interval.\n");
    return (0);
}

int init_format(int fd)
{
    v4l2_format imageFormat = {0};
    v4l2_cropcap cropcap = {0};
	v4l2_crop crop = {0};

    CLEAR(imageFormat);
    imageFormat.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    imageFormat.fmt.pix.width = 400;
    imageFormat.fmt.pix.height = 200;
    imageFormat.fmt.pix.field = V4L2_FIELD_NONE;
    imageFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    if (xioctl(fd, VIDIOC_S_FMT, &imageFormat) < 0) {
        perror("Device could not set format, VIDIOC_S_FMT");
        return (1);
    }
    CLEAR(cropcap);
	cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (0 == xioctl(fd, VIDIOC_CROPCAP, &cropcap)) {
		crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		crop.c = cropcap.defrect;
    }
    return (0);
}

int init_buffers(int fd, struct buffer *buffers)
{
    v4l2_requestbuffers requestBuffer = {0};

    CLEAR(requestBuffer);
    requestBuffer.count = 1;
    requestBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    requestBuffer.memory = V4L2_MEMORY_MMAP;
    if (xioctl(fd, VIDIOC_REQBUFS, &requestBuffer) < 0) {
        perror("Could not request buffer from device, VIDIOC_REQBUFS");
        exit(0);
    }
    buffers = (struct buffer*)calloc(requestBuffer.count, sizeof(buffer));
	for (buffers->n_buffers = 0; buffers->n_buffers < requestBuffer.count; ++buffers->n_buffers) {
        v4l2_buffer queryBuffer = {0};
        CLEAR(queryBuffer);
        queryBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        queryBuffer.memory = V4L2_MEMORY_MMAP;
        queryBuffer.index = buffers->n_buffers;
        if (xioctl(fd, VIDIOC_QUERYBUF, &queryBuffer) < 0) {
            perror("Device did not return the buffer information, VIDIOC_QUERYBUF");
            exit(0);
        }
		buffers[buffers->n_buffers].length = queryBuffer.length;
		buffers[buffers->n_buffers].start = v4l2_mmap(NULL, queryBuffer.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, queryBuffer.m.offset);
        save = malloc(sizeof(buffers->start));
    }
    save = buffers->start;
    return (buffers->n_buffers);
}

int init_all(int fd, struct buffer *buffers)
{
    int i = 0;

    if (check_capabilities(fd) != 0)
        return (-1);
    if (init_format(fd) != 0)
        return (-1);
    if (init_fps(fd) != 0)
        return (-1);
    i = init_buffers(fd, buffers);
    if (i == 0)
        return (-1);
    return (i);
}