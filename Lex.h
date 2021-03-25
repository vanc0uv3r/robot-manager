#ifndef ROBOT_MANAGER_LEX_H
#define ROBOT_MANAGER_LEX_H
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

class Lex
{
    int fd;

    Lex(char *filename)
    {
        fd = open(filename, O_RDONLY);
        if (fd == -1)
        {
            printf("Error\n");
            exit(1);
        }
    }

    ~Lex()
    {
        close(fd);
    }
};


#endif //ROBOT_MANAGER_LEX_H
