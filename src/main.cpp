/*
** EPITECH PROJECT, 2019
** main
** File description:
** cpp
*/

#include "functions.h"
#include "webcam.h"

using namespace std;

/**
     *  @brief main function
     * 
     *  @param ac   number of argument(s)
     *  @param av   argument
     *  @return     0 if everthing's fine, 1 if not.
*/

int main(int ac, char **av)
{
    UNUSED(av);
    if (ac != 1)
        return (1);
    if (capture() != 0)
        return (1);
    return (0);
}