/*
** EPITECH PROJECT, 2019
** capture
** File description:
** webcam
*/

#include "webcam.h"
#include "functions.h"

using namespace std;

int goon = 0;
char name[11] = "img/webcam";
static char* jpegFilename = name;
static char* jpegFilenamePart = NULL;
static const char* const FilenameFmt = "%s_%010"PRIu32"_%"PRId64".jpeg";
const extern void *save;

void StopContCapture(int sig_id)
{
    UNUSED(sig_id);
    printf("\nStoping capture.\n");
    goon = 1;
}

void InstallSIGINTHandler()
{
    struct sigaction sa;
    CLEAR(sa);
    sa.sa_handler = StopContCapture;
    if (sigaction(SIGINT, &sa, 0) != 0)
        fprintf(stderr,"could not install SIGINT handler, continuous capture disabled");
}

static int frameRead(int fd)
{
    v4l2_buffer buf = {0};
    ofstream outFile;

    CLEAR(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
        switch (errno) {
        case EAGAIN:
            return 0;
        default:
            perror("VIDIOC_DQBUF");
        }
    }
    //init timeval struct for .jpeg name update
    struct timeval timestamp = buf.timestamp;
    static uint32_t img_ind = 0;
    int64_t timestamp_long;
    timestamp_long = timestamp.tv_sec*1e6 +  timestamp.tv_usec;
    sprintf(jpegFilename,FilenameFmt,jpegFilenamePart,img_ind++,timestamp_long);
    //write in .jpeg (open create the .jpeg, write fill it, and close)
    outFile.open(jpegFilename);
    outFile.write((char*)save, (double)buf.bytesused);
    outFile.close();
    if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
        perror("VIDIOC_QBUF");
    return (0);
}

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

int start_capture(int fd, int n)
{
    int i = 0;
    enum v4l2_buf_type type;

    //start capture for n_buffers
    for (; i < n; ++i) {
        struct v4l2_buffer buf;
        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        if (-1 == xioctl(fd, VIDIOC_QBUF, &buf)) {
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

int capture(void)
{
    int fd = open("/dev/video2", O_RDWR);
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
