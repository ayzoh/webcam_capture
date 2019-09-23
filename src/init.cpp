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

/**
     *  @brief Do ioctl and retry if error was EINTR, same parameters as ioctl.
     * 
     *  @param fd   webcam file descriptor.
     *  @param request request(VIDIOC_)
     *  @param argp argument
     *  @return     0 if everthing's fine, 1 if not.
*/

int xioctl(int fd, long unsigned int request, void* argp)
{
    //system call to control device according to the request
    int r = 0;

    do r = v4l2_ioctl(fd, request, argp);
    while (r == -1 && EINTR == errno);

    return (r);
}

/**
     *  @brief Check if there is a camera and if it can record
     * 
     *  @param fd   webcam file descriptor.
     *  @return     0 if everthing's fine, 1 if not.
*/

int check_capabilities(int fd)
{
    v4l2_capability cap = {0};

    //check if a camera has been opened on /dev/video*
    if (fd < 0) {
        perror("Failed to open device, OPEN");
        return (1);
    }
    //check the camera capabilities
    if (xioctl(fd, VIDIOC_QUERYCAP, &cap) < 0) {
        perror("Failed to get device capabilities, VIDIOC_QUERYCAP");
        return (1);
    }
    return (0);
}

/**
     *  @brief Initialize fps option (not implemented yet)
     * 
     *  @param fd   webcam file descriptor.
     *  @return     0 if everthing's fine, 1 if not.
*/


int init_fps(int fd)
{
    v4l2_streamparm frameint = {0};

    CLEAR(frameint);
    frameint.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    frameint.parm.capture.timeperframe.numerator = 1;
    frameint.parm.capture.timeperframe.denominator = fps;
    if (xioctl(fd, VIDIOC_S_PARM, &frameint) == -1) 
        fprintf(stderr,"Unable to set frame interval.\n");
    return (0);
}

/**
     *  @brief Initialize capture format of the selected camera
     * 
     *  @param fd   webcam file descriptor.
     *  @return     0 if everthing's fine, 1 if not.
*/

int init_format(int fd)
{
    v4l2_format imageFormat = {0};
    v4l2_cropcap cropcap = {0};
    v4l2_crop crop = {0};

    //init record format
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
    //information about cropping and scaling abilities
    CLEAR(cropcap);
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (xioctl(fd, VIDIOC_CROPCAP, &cropcap) == 0) {
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect;
    }
    return (0);
}

/**
     *  @brief Request and initialize buffers
     * 
     *  @param fd   webcam file descriptor.
     *  @param *buffers pointer on buffer structure
     *  @return    number of buffers if everthing's fine, -1 if not.
*/


int init_buffers(int fd, struct buffer *buffers)
{
    v4l2_requestbuffers requestBuffer = {0};

    //request a memory mapping buffer
    CLEAR(requestBuffer);
    requestBuffer.count = 1;
    requestBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    requestBuffer.memory = V4L2_MEMORY_MMAP;
    if (xioctl(fd, VIDIOC_REQBUFS, &requestBuffer) < 0) {
        perror("Could not request buffer from device, VIDIOC_REQBUFS");
        return(-1);
    }
    //fill a memory mapping buffer with empty storage
    for (buffers->n_buffers = 0; buffers->n_buffers < requestBuffer.count; ++buffers->n_buffers) {
        v4l2_buffer queryBuffer = {0};
        CLEAR(queryBuffer);
        queryBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        queryBuffer.memory = V4L2_MEMORY_MMAP;
        queryBuffer.index = buffers->n_buffers;
        if (xioctl(fd, VIDIOC_QUERYBUF, &queryBuffer) < 0) {
            perror("Device did not return the buffer information, VIDIOC_QUERYBUF");
            return(-1);
        }
        buffers[buffers->n_buffers].length = queryBuffer.length;
        buffers[buffers->n_buffers].start = v4l2_mmap(NULL, queryBuffer.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, queryBuffer.m.offset);
    }
    save = buffers->start;
    return (buffers->n_buffers);
}

/**
     *  @brief Global initialize function (check functions return)
     * 
     *  @param fd   webcam file descriptor.
     *  @param *buffers pointer on buffer structure
     *  @return    number of buffers if everthing's fine, -1 if not.
*/

int init_all(int fd, struct buffer *buffers)
{
    int n_buffer = 0;

    //check if camera can record
    if (check_capabilities(fd) != 0)
        return (-1);
    //initialize the capture format of the webcam
    if (init_format(fd) != 0)
        return (-1);
    //initialize the fps option (not implemented yet)
    init_fps(fd);
    //initialize the capture buffers and return the nbr of buffers
    n_buffer = init_buffers(fd, buffers);
    if (n_buffer <= 0)
        return (-1);
    return (n_buffer);
}