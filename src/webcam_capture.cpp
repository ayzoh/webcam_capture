/*
** EPITECH PROJECT, 2019
** capture
** File description:
** webcam
*/

#include "webcam.h"
#include "functions.h"
#include <Magick++.h>

using namespace std;
using namespace Magick;

/// Capture loop variable. Change to 1 if Ctrl+C (SIGINT) is entered 
int goon = 0;
/// Defined the directory and the name of the capture.
char name[11] = "img/webcam";
/// Final name of the capture, depend on FilenameFmt and name.
static char* jpegFilename = name;
/// Set to assembly and get enough space for the jpegFilename.
static char* jpegFilenamePart = NULL;
/// Set output format of the jpegFilename.
static const char* const FilenameFmt = "%s_%010"PRIu32"_%"PRId64".pgm";
/// Saved framebuffer in init.cpp.
const extern void *save;

/**
     *  @brief Stop capture if sig_int signal is entered.
     * 
     *  @param sig_id   signal ID.
     *  @remark     pass goon variable to 1 to interrupt capture loop.
*/

void StopContCapture(int sig_id)
{
    UNUSED(sig_id);
    printf("\nStoping capture.\n");
    goon = 1;
}

/**
     *  @brief Catch sig_int signal if Ctrl+C is entered
     *
     *  @details
     * Sigaction strucure interpret the incoming signal (when it catch one)
     * and if sig_int is recognize, transmit the info to StopContCapture(SIGINT).
*/

void InstallSIGINTHandler()
{
    struct sigaction sa;
    CLEAR(sa);
    sa.sa_handler = StopContCapture;
    if (sigaction(SIGINT, &sa, 0) != 0)
        fprintf(stderr,"could not install SIGINT handler, continuous capture disabled");
}

/**
     *  @brief Write in a .pgm the saved buffer (frame) every seconds.
     *
     *  @details
     *  the VIDIOC_DQBUF ioctl is used to dequeue a filled (capturing) or displayed
     *  (output) buffer from the driver’s outgoing queue.\n
     *  The ofstream file is the jpg, that will be transformed in .pgm thank's to magick++ library
     * 
     *  @param fd   webcam file descriptor.
     *  @return     0 if everthing's fine, 1 if not.
*/

int frameRead(int fd)
{
    v4l2_buffer buf = {0};
    ofstream outFile;
    Image image;

    CLEAR(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    if (xioctl(fd, VIDIOC_DQBUF, &buf) == -1) {
        switch (errno) {
            case EAGAIN:
                return 0;
            default:
                perror("VIDIOC_DQBUF");
        }
    }
    //init timeval struct for .jpeg name update
    const static char *filename = ".pgm";
    struct timeval timestamp = buf.timestamp;
    static uint32_t img_ind = 0;
    int64_t timestamp_long;
    timestamp_long = timestamp.tv_sec*1e6 +  timestamp.tv_usec;
    sprintf(jpegFilename,FilenameFmt,jpegFilenamePart,img_ind++,timestamp_long);
    //write in .jpeg (open create the .jpeg, write fill it, and close)
    outFile.open(jpegFilename);
    outFile.write((char*)save, (double)buf.bytesused);
    outFile.close();
    //compress .pgm in the ascii form
    image.read(jpegFilename);
    image.compressType(CompressionType(NoCompression));
    image.write(jpegFilename);
    if (start_hog(jpegFilename) != 0)
        return (1);
    // remove file in /img to keep only histogramme in /res
    strcat(jpegFilename, filename);
    remove(jpegFilename);
    if (xioctl(fd, VIDIOC_QBUF, &buf) == -1)
        perror("VIDIOC_QBUF");
    sleep(1);
    return (0);
}

/**
     *  @brief Capture frames while Ctrl+C is not pressed.
     * 
     *  @param fd   webcam file descriptor.
     *  @return     0 if everthing's fine, 1 if not.
*/


int record_loop(int fd)
{
    std::cout<<"Capturing.. Press Ctrl+C to exit"<<std::endl;
    for (;goon == 0;) {
        if (frameRead(fd))
            break;
        if (goon == 1)
            break;
        goon = 0;
    }
    return (0);
}

/**
     *  @brief Start camera record (VIDIOC_STREAMON).
     *
     *  @details
     * Applications call the VIDIOC_QBUF ioctl to enqueue an empty (capturing)
     * or filled (output) buffer in the driver’s incoming queue.
     * 
     *  @param fd   webcam file descriptor.
     *  @param n   number of queued buffers needed.
     *  @return     0 if everthing's fine, 1 if not.
*/

int start_capture(int fd, int n)
{
    enum v4l2_buf_type type;

    //start capture for n_buffers
    for (int index = 0; index < n; ++index) {
        struct v4l2_buffer buf;
        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = index;
        if (xioctl(fd, VIDIOC_QBUF, &buf) == 1) {
            perror("VIDIOC_QBUF");
            return (1);
        }
    }
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == xioctl(fd, VIDIOC_STREAMON, &type)) {
        perror("VIDIOC_STREAMON");
        return (1);
    }
    return (0);
}

/**
     *  @brief Stop camera record (VIDIOC_STREAMOFF)
     *
     *  @param fd   webcam file descriptor.
     *  @return     0 if everthing's fine, 1 if not.
*/

int stop_record(int fd)
{
    v4l2_buf_type type;

    //stop streaming when SIGINT
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (xioctl(fd, VIDIOC_STREAMOFF, &type) < 0) {
        perror("Could not end streaming, VIDIOC_STREAMOFF");
        return (1);
    }
    return (0);
}

/**
     *  @brief Main function of the project
     *
     *  @details
     * Get the wanted file descriptor\n
     * init: buffers, filename, signal\n
     * check function returns of start_capture, record_loop, stop_record\n
     * free buffers\n
     * close fd\n
     * 
     *  @return 0 if everthing's fine, 1 if not.
*/

int capture(char *device_link)
{
    int fd = open(device_link, O_RDWR);
    struct buffer *buffers = NULL;
    buffers = (struct buffer*)calloc(1, sizeof(buffer));
    //do checks and init buffers, format | return nbr buffers
    int nbr_buffers = init_all(fd, buffers);
    if (nbr_buffers < 0)
        return (1);
    //get size of malloc for the file name .jpeg
    int max_name_len = snprintf(NULL,0,FilenameFmt,jpegFilename,UINT32_MAX,INT64_MAX);
    jpegFilenamePart = jpegFilename;
    jpegFilename = (char *)calloc(max_name_len+1,sizeof(char));
    strcpy(jpegFilename,jpegFilenamePart);

    //initialize the stop signal SIGINT to stop the record loop without buffering stdin
    InstallSIGINTHandler();
    //turn on the webcam if the buffers exists
    if (start_capture(fd, nbr_buffers) != 0)
        return (1);
    //start the record loop while goon == 0 and process each frames
    if (record_loop(fd) != 0)
        return (1);
    //stop the record if ctrl+c (SIGINT) is pressed
    if (stop_record(fd) != 0)
        return (1);
    //free all
    if (jpegFilename != 0)
        free(jpegFilename);
    free(buffers);
    close(fd);
    return (0);
}
