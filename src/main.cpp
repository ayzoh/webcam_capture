/*
** EPITECH PROJECT, 2019
** main
** File description:
** cpp
*/

#include "functions.h"
#include "webcam.h"

using namespace std;

int usage(void)
{
     printf("%s\n", "Usage: ./capture [link_to record_device]");
     printf("%s\n", "default: ./capture /dev/video0");
     return (1);
}

int main(int ac, char **av)
{
     if (ac == 1)
          return (usage());
     if (ac == 2) {
          if (strcmp("-h\0", av[1]) == 0)
               return (usage());
          if (capture(av[1]) != 0)
               return (1);
     }
     return (0);
}