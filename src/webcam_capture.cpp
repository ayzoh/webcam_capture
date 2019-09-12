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
static const char* const continuousFilenameFmt = "%s_%010"PRIu32"_%"PRId64".jpeg";
const extern void *save;

void StopContCapture(int sig_id)
{
    UNUSED(sig_id);
    printf("\nstopping continuous capture\n");
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
    string base(".jpeg");

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
    struct timeval timestamp = buf.timestamp;
    static uint32_t img_ind = 0;
    int64_t timestamp_long;
    timestamp_long = timestamp.tv_sec*1e6 +  timestamp.tv_usec;
    sprintf(jpegFilename,continuousFilenameFmt,jpegFilenamePart,img_ind++,timestamp_long);
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

    for (; i < n; ++i) {
        struct v4l2_buffer buf;
        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
            perror("VIDIOC_QBUF");
    }
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
        perror("VIDIOC_STREAMON");
    return (0);
}

int stop_record(int fd)
{
    v4l2_buf_type type;

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (xioctl(fd, VIDIOC_STREAMOFF, &type) < 0) {
        perror("Could not end streaming, VIDIOC_STREAMOFF");
        return (1);
    }
    return (0);
}

int capture(void)
{
    int fd = open("/dev/video0", O_RDWR);
    struct buffer *buffers = NULL;
    int n = init_all(fd, buffers);
    int max_name_len = snprintf(NULL,0,continuousFilenameFmt,jpegFilename,UINT32_MAX,INT64_MAX);
    jpegFilenamePart = jpegFilename;
    jpegFilename = (char *)calloc(max_name_len+1,sizeof(char));
    strcpy(jpegFilename,jpegFilenamePart);

    InstallSIGINTHandler();
    if (start_capture(fd, n) != 0)
        return (1);
    if (record_loop(fd) != 0)
        return (1);
    if (stop_record(fd) != 0)
        return (0);
    if (jpegFilename != 0)
        free(jpegFilename);
    close(fd);
    return (0);
}