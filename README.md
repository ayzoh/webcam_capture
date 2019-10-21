# webcam_capture


/!\ This project required the Video4Linux and ImageMagick library /!\


Video4Linux: https://ubuntu.pkgs.org/18.04/ubuntu-universe-amd64/v4l-utils_1.14.2-1_amd64.deb.html

ImageMagick: https://imagemagick.org/script/install-source.php


The goal of this project is to record each frame per second of a webcam (default: /dev/video0), convert the "frame.jpg" to a "frame.pgm" (Portable gray map)
in the "img" directory. The recorded frame is next treated in an HOG algorithm and comes out as a "frame_histo.pgm" in the "res" directory.
The "frame.pgm" is then erased in order to only keep the treated frame (res/frame_histo.pgm).


/!\ Don't forget to install delegates libraries (e.g. libpng16, libjpeg62..) /!\


This program has 2 ways of compilation, depending of your environment. Remove the one you don't need between Fedora or Ubuntu Makefile, and rename the one you need in "Makefile".
