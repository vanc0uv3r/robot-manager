#include <stdio.h>
#include <sys/types.h>
#include "helpers.h"
#include "Robot.h"

void play(Robot& r)
{

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
