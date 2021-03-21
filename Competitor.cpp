#include <stdio.h>
#include "Competitor.h"

void Competitor::print_fields()
{
    printf("INFORMATION about (%s):\n", nick);
    printf("His BALANCE is %d$\n", money);
    printf("He has %d RAWS\n", raw);
    printf("He has %d PROD\n\n", prod);
}

void Competitor::update_fields(char *line)
{
    char r1, r2[7];
    sscanf(line, "%c%s%32s%d%d%d%d%d", &r1, r2, nick, &raw, &prod, &money,
           &plants, &auto_plants);
}
