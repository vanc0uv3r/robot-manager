#include <stdio.h>
#include <sys/types.h>
#include "helpers.h"
#include "Robot.h"
#include "Lex.h"

void open_program(const char *filename)
{
    int fd;
    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("file");
        exit(1);
    }
    dup2(fd, 0);
    close(fd);
}

int main(int argc, char *argv[])
{
    int c;
    Lex l;
    open_program(argv[1]);
    while ((c = getchar()) != EOF)
        l.analyze(c);
    return 0;
}
