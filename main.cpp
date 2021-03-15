#include <stdio.h>
#include <sys/types.h>
#include "helpers.h"
#include "Robot.h"

void play(Robot& r)
{
    r.wait_for_start();
    printf("GAME HAS STARTED\n");
    while (r.no_winner())
    {
        r.get_info();
        r.parse_info();
        r.get_market();
        r.parse_market();
        r.me();
        r.sell();
        r.buy();
        r.make_prod();
        r.make_turn();
        r.wait_other();
        r.print_buffer();
    }
    r.define_winner();
}

int main(int argc, char *argv[])
{
    int next_action = check_params(argc, argv);
    if (next_action == quit)
    {
        printf("%s %s\n", invalid_params, usage);
        return 1;
    }
    Robot r(argv[1], argv[2]);
    if (next_action == join)
        r.join(argv[4]);
    else if (next_action == create)
        r.create();
    play(r);
    return 0;
}
