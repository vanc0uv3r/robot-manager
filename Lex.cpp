#include "Lex.h"

Lex::Lex(const char *filename)
{
    buffer = new char [128];
    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("file");
        exit(1);
    }
    dup2(fd, 0);
    close(fd);
}

int Lex::next_lexeme() {
    return 0;
}

int Lex::define_lexeme_type() {
    return 0;
}
