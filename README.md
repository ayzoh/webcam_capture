# webcam_capture

/!\ This project required the Video4Linux and ImageMagick library /!\

ImageMagick: https://imagemagick.org/script/install-source.php

The goal of this project is to record each frame per second of a webcam (default: /dev/video0), convert the "frame.jpg" to a "frame.pgm" (Portable gray map)
in the "img" directory. The recorded frame is next treated in an HOG algorithm and comes out as a "frame_histo.pgm" in the "res" directory.
The "frame.pgm" is then erased in order to only keep the treated frame.

