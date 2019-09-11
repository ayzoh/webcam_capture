/*
** EPITECH PROJECT, 2019
** main
** File description:
** cpp
*/

#include "functions.h"
#include "webcam.h"
// #include <Magick++.h>
// using namespace Magick;

using namespace std;

int main(int ac, char **av)
{
    UNUSED(av);
    if (ac != 1)
        return (1);
    if (capture() != 0)
        return (1);
}
