#include <stdio.h>
#include <sys/types.h>
#include "helpers.h"
#include "Robot.h"

void play(Robot& r)
{
    int round = 1;
    printf("GAME HAS STARTED\n\n");
    while (r.no_winner())
    {
        printf("ROUND %d:\n\n", round);
        r.get_info();
        r.get_market();
        r.me();
        r.sell();
        r.buy();
        r.make_prod();
        r.make_turn();
        r.wait_other();
        round++;
    }
}

int main(int argc, char *argv[])
{
    int next_action = check_params(argc, argv);
    if (next_action == quit)
        return 1;
    Robot r(argv[1], argv[2], argv[3]);
    if (next_action == join)
        r.join(argv[5]);
    else if (next_action == create)
        r.create(argv[5]);
    play(r);
    return 0;
}
